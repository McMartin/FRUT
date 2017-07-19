//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.6
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstspeakerarray,.h
// Created by  : Steinberg, 04/2015
// Description : Helper class representing speaker arrangement as array of speaker types.
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

#include "pluginterfaces/vst/vsttypes.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SpeakerArray
/** Helper class representing speaker arrangement as array of speaker types. */
//------------------------------------------------------------------------
class SpeakerArray
{
public:
//------------------------------------------------------------------------
	SpeakerArray (SpeakerArrangement arr = 0)
	{
		setArrangement (arr);
	}

	enum { kMaxSpeakers = 64 };

	typedef uint64 SpeakerType;

	int32 total () const { return count; }
	SpeakerType at (int32 index) const { return speaker[index]; }

	void setArrangement (SpeakerArrangement arr)
	{
		count = 0;
		memset (speaker, 0, sizeof (speaker));

		for (int32 i = 0; i < kMaxSpeakers; i++)
		{
			SpeakerType mask = 1ll << i;
			if (arr & mask)
				speaker[count++] = mask;
		}
	}

	SpeakerArrangement getArrangement () const
	{
		SpeakerArrangement arr = 0;
		for (int32 i = 0; i < count; i++)
			arr |= speaker[i];
		return arr;
	}

	int32 getSpeakerIndex (SpeakerType which) const
	{
		for (int32 i = 0; i < count; i++)
			if (speaker[i] == which)
				return i;
		return -1;
	}
//------------------------------------------------------------------------
protected:
	int32 count;
	SpeakerType speaker[kMaxSpeakers];
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
