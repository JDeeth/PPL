/* Copyright (c) 2018, Jack Deeth github@jackdeeth.org.uk
All rights reserved

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#pragma once

#include <XPLMDisplay.h>

namespace PPL {

class DrawCallback {
public:
  enum class Phase {
    FirstScene = xplm_Phase_FirstScene,
    Terrain = xplm_Phase_Terrain,
    Airports = xplm_Phase_Airports,
    Vectors = xplm_Phase_Vectors,
    Objects = xplm_Phase_Objects,
    Airplanes = xplm_Phase_Airplanes,
    LastScene = xplm_Phase_LastScene,
    FirstCockpit = xplm_Phase_FirstCockpit,
    Panel = xplm_Phase_Panel,
    Gauges = xplm_Phase_Gauges,
    Window = xplm_Phase_Window,
    LastCockpit = xplm_Phase_LastCockpit,
    LocalMap3D = xplm_Phase_LocalMap3D,
    LocalMap2D = xplm_Phase_LocalMap2D,
    LocalMapProfile = xplm_Phase_LocalMapProfile
  };

  DrawCallback(Phase phase, bool in_before);
  virtual ~DrawCallback();

  virtual int callback(Phase phase, bool is_before) = 0;

private:
  XPLMDrawCallback_f cb_ptr_{nullptr};
  XPLMDrawingPhase phase_{0};
  int in_before_{0};
};

}  // namespace
