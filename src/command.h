#pragma once

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

/* Example usage:

// observe an existing command
PPL::Command left_brake_cmd_{"sim/flight_controls/left_brake"};
...
if(left_brake_cmd_.phase()==PPL::Command::Phase::Continue) ...

// disable an existing command
PPL::Command left_brake_cmd_{"sim/flight_controls/left_brake",
                             PPL::Command::halt};

// hijack an existing command
PPL::Command brake_toggle_cmd_{"sim/flight_controls/brakes_toggle_regular",
                               [this](PPL::Command::Phase phase) {
                                 if (phase == PPL::Command::Phase::Begin) {
                                   if (handbrake_lever_ratio_ < 0.5f)
                                     handbrake_lever_ratio_ = 0.5f;
                                   else
                                     handbrake_lever_ratio_ = 0.f;
                                 }
                                 return PPL::Command::Outcome::Halt;
                               }};

// create a new command
class CompileMessage {
 public:
  CompileMessage(
      std::string command_name = "YourName/YourProject/show_compile_msg",
      std::string desc = "Announces compilation date/time of plugin")
      : cmd_{command_name, desc, [](PPL::Command::Phase phase) {
               if (phase == PPL::Command::Phase::Begin)
                 XPLMSpeakString(compile_message().c_str());
               return PPL::Command::Outcome::Halt;
             }} {}

  static std::string compile_message() {
    std::stringstream msg;
    msg << "Compiled " << __DATE__ << " at " << __TIME__;
    return msg.str();
  }

 private:
  PPL::Command cmd_;
};
*/

#include <XPLMUtilities.h>

#include <functional>
#include <string>

namespace PPL {

class Command {
public:
  enum class Outcome { Halt, Pass_On };
  enum class Phase { Begin, Continue, End };

  // Finds a command by reference and attaches a new callback to it
  Command(XPLMCommandRef ref,
          std::function<Outcome(Phase)> callback_ = Command::pass_on,
          bool run_before = true);

  // Finds a command by name and attaches a new callback to it
  Command(std::string cmd_to_replace,
          std::function<Outcome(Phase)> callback_ = Command::pass_on,
          bool run_before = true);

  // Creates a new command and attaches a callback to it
  Command(std::string new_cmd,
          std::string description,
          std::function<Outcome(Phase)> callback_ = Command::pass_on,
          bool run_before = true);

  ~Command();

  Phase phase() const;

  // Call XPLMCommandBegin/End/Once
  void begin();
  void end();
  void once();

  static Outcome halt(...) { return Outcome::Halt; }
  static Outcome pass_on(...) { return Outcome::Pass_On; }

private:
  XPLMCommandRef ref_;
  XPLMCommandCallback_f cb_ptr_;
  std::function<Outcome(Phase)> callback_;
  int before_;
  Phase phase_{Phase::End};
};

}  // namespace
