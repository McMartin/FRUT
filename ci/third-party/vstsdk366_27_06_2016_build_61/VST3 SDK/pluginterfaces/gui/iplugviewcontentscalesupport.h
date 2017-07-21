//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK GUI Interfaces
// Filename    : pluginterfaces/gui/iplugviewcontentscalesupport.h
// Created by  : Steinberg, 06/2016
// Description : Plug-in User Interface Scaling
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
//------------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"

//------------------------------------------------------------------------
#include "pluginterfaces/base/falignpush.h"
//------------------------------------------------------------------------

//------------------------------------------------------------------------
namespace Steinberg {

//------------------------------------------------------------------------
/** Plug-in view content scale support
\ingroup pluginGUI
- [plug impl]
- [extends IPlugView]
- [optional]

This interface communicates the content scale factor from the host to the plug-in view on
systems where plug-ins cannot get this information directly like Microsoft Windows.

The host calls setContentScaleFactor directly after the plug view was attached and when the scale
factor changes (system change or window moved to another screen with different scaling settings)
When a plug-in handles this, it needs to scale the width and height of its view by the scale factor
and inform the host via a IPlugView::resizeView().
 */
class IPlugViewContentScaleSupport : public FUnknown
{
public:
//------------------------------------------------------------------------
	typedef float ScaleFactor;

	virtual tresult PLUGIN_API setContentScaleFactor (ScaleFactor factor) = 0;
//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugViewContentScaleSupport, 0x65ED9690, 0x8AC44525, 0x8AADEF7A, 0x72EA703F)

//------------------------------------------------------------------------
} // namespace Steinberg

//------------------------------------------------------------------------
#include "pluginterfaces/base/falignpop.h"
//------------------------------------------------------------------------
