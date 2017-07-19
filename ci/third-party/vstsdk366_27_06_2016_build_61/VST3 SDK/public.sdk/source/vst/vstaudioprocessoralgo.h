//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.6
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstaudioprocessoralgo.h
// Created by  : Steinberg, 04/2015
// Description : Helper algo for AudioBusBuffers
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2016, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety
// without prior written agreement by Steinberg Media Technologies GmbH.
// This SDK must not be used to re-engineer or manipulate any technology used
// in any Steinberg or Third-party application or software module,
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------------------

#pragma once

#define USE_XMM_INTRIN 1

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>

#if USE_XMM_INTRIN
#include <xmmintrin.h>
#endif

namespace Steinberg {
namespace Vst {
namespace Algo {

//------------------------------------------------------------------------
template <typename T>
inline void foreach (AudioBusBuffers* audioBusBuffers, int32 busCount, const T& func)
{
	if (!audioBusBuffers)
		return;

	for (int32 busIndex = 0; busIndex < busCount; ++busIndex)
	{
		func (audioBusBuffers[busIndex]);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach32 (AudioBusBuffers& audioBuffer, const T& func)
{
	for (int32 channelIndex = 0; channelIndex < audioBuffer.numChannels; ++channelIndex)
	{
		if (!audioBuffer.channelBuffers32[channelIndex])
			return;

		func (audioBuffer.channelBuffers32[channelIndex]);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach64 (AudioBusBuffers& audioBuffer, const T& func)
{
	for (int32 channelIndex = 0; channelIndex < audioBuffer.numChannels; ++channelIndex)
	{
		if (!audioBuffer.channelBuffers64[channelIndex])
			return;

		func (audioBuffer.channelBuffers64[channelIndex]);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach32 (AudioBusBuffers& buffer1, AudioBusBuffers& buffer2, const T& func)
{
	int32 numChannels = std::min<int32> (buffer1.numChannels, buffer2.numChannels);

	for (int32 channelIndex = 0; channelIndex < numChannels; ++channelIndex)
	{
		func (buffer1.channelBuffers32[channelIndex], buffer2.channelBuffers32[channelIndex],
		      channelIndex);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach64 (AudioBusBuffers& buffer1, AudioBusBuffers& buffer2, const T& func)
{
	int32 numChannels = std::min<int32> (buffer1.numChannels, buffer2.numChannels);

	for (int32 channelIndex = 0; channelIndex < numChannels; ++channelIndex)
	{
		func (buffer1.channelBuffers64[channelIndex], buffer2.channelBuffers64[channelIndex],
		      channelIndex);
	}
}

//------------------------------------------------------------------------
inline void copy32 (AudioBusBuffers* src, AudioBusBuffers* dest, int32 sliceSize, int32 startIndex)
{
	if (!src || !dest)
		return;

	int32 numChannels = std::min<int32> (src->numChannels, dest->numChannels);
	size_t numBytes = sliceSize * sizeof (Sample32);
	for (int32 chIdx = 0; chIdx < numChannels; ++chIdx)
	{
		memcpy (&dest->channelBuffers32[chIdx][startIndex], src->channelBuffers32[chIdx], numBytes);
	}
}

//------------------------------------------------------------------------
inline void copy64 (AudioBusBuffers* src, AudioBusBuffers* dest, int32 sliceSize, int32 startIndex)
{
	if (!src || !dest)
		return;

	int32 numChannels = std::min<int32> (src->numChannels, dest->numChannels);
	size_t numBytes = sliceSize * sizeof (Sample64);
	for (int32 chIdx = 0; chIdx < numChannels; ++chIdx)
	{
		memcpy (&dest->channelBuffers64[chIdx][startIndex], src->channelBuffers64[chIdx], numBytes);
	}
}

//------------------------------------------------------------------------
inline void clear32 (AudioBusBuffers* audioBusBuffers, int32 sampleCount, int32 busCount = 1)
{
	if (!audioBusBuffers)
		return;

	const int32 numBytes = sampleCount * sizeof (Sample32);
	foreach (audioBusBuffers, busCount, [&] (AudioBusBuffers& audioBuffer) {
		foreach32 (audioBuffer, [&] (Sample32* channelBuffer) {
			memset (channelBuffer, 0, numBytes);
		});
	});
}

//------------------------------------------------------------------------
inline void clear64 (AudioBusBuffers* audioBusBuffers, int32 sampleCount, int32 busCount = 1)
{
	if (!audioBusBuffers)
		return;

	const int32 numBytes = sampleCount * sizeof (Sample64);
	foreach (audioBusBuffers, busCount, [&](AudioBusBuffers& audioBuffer) {
		foreach64 (audioBuffer, [&](Sample64* channelBuffer) {
			memset (channelBuffer, 0, numBytes);
		});
	});
}

//------------------------------------------------------------------------
inline void mix32 (AudioBusBuffers& src, AudioBusBuffers& dest, int32 sampleCount)
{
	foreach32 (src, dest, [&] (Sample32* srcBuffer, Sample32* destBuffer, int32 /*channelIndex*/) {
#if USE_XMM_INTRIN
		// TODO check alignment!!!

		static const int32 kVectorSize = 4;
		if ((sampleCount & (kVectorSize - 1)) == 0) 
		{
			for (int32 sampleIdx = 0; sampleIdx < sampleCount; sampleIdx += kVectorSize)
			{
				_mm_store_ps (destBuffer,
				              _mm_add_ps (_mm_load_ps (destBuffer), _mm_load_ps (srcBuffer)));

				srcBuffer += kVectorSize;
				destBuffer += kVectorSize;
			}
		}
		else
#endif
		{
			for (int32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex)
				destBuffer[sampleIndex] += srcBuffer[sampleIndex];
		}
	});
}

//------------------------------------------------------------------------
inline void mix64 (AudioBusBuffers& src, AudioBusBuffers& dest, int32 sampleCount)
{
	foreach64 (src, dest, [&] (Sample64* srcBuffer, Sample64* destBuffer, int32 /*channelIndex*/) {
		for (int32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex)
			destBuffer[sampleIndex] += srcBuffer[sampleIndex];
	});
}

//------------------------------------------------------------------------
inline bool isSilent32 (AudioBusBuffers& audioBuffer, int32 sampleCount, int32 startIndex = 0)
{
	// TODO 64
	static const float epsilon = 1e-10f; // under -200dB...

	sampleCount += startIndex;
	for (int32 channelIndex = 0; channelIndex < audioBuffer.numChannels; ++channelIndex)
	{
		if (!audioBuffer.channelBuffers32[channelIndex])
			return true;

		for (int32 sampleIndex = startIndex; sampleIndex < sampleCount; ++sampleIndex)
		{
			float val = audioBuffer.channelBuffers32[channelIndex][sampleIndex];
			if (fabsf (val) > epsilon)
				return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
template <typename T>
inline void foreach (IEventList* eventList, const T& func)
{
	if (!eventList)
		return;

	auto eventCount = eventList->getEventCount ();
	for (int32 eventIndex = 0; eventIndex < eventCount; ++eventIndex)
	{
		Event event = {0};
		if (eventList->getEvent (eventIndex, event) != kResultOk)
			continue;

		func (event);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach (IParamValueQueue& paramQueue, const T& func)
{
	auto paramId = paramQueue.getParameterId ();
	auto numPoints = paramQueue.getPointCount ();
	for (int32 pointIndex = 0; pointIndex < numPoints; ++pointIndex)
	{
		int32 sampleOffset = 0;
		ParamValue value = 0;
		if (paramQueue.getPoint (pointIndex, sampleOffset, value) != kResultOk)
			continue;

		func (paramId, sampleOffset, value);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreachLast (IParamValueQueue& paramQueue, const T& func)
{
	auto paramId = paramQueue.getParameterId ();
	auto numPoints = paramQueue.getPointCount ();
	int32 sampleOffset = 0;
	ParamValue value = 0;
	if (paramQueue.getPoint (numPoints - 1, sampleOffset, value) == kResultOk)
		func (paramId, sampleOffset, value);
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach (IParameterChanges* changes, const T& func)
{
	if (!changes)
		return;

	auto paramCount = changes->getParameterCount ();
	for (int32 paramIndex = 0; paramIndex < paramCount; ++paramIndex)
	{
		auto paramValueQueue = changes->getParameterData (paramIndex);
		if (!paramValueQueue)
			continue;

		func (*paramValueQueue);
	}
}

//------------------------------------------------------------------------
} // namespace Algo
} // namespace Vst
} // namespace Steinberg
