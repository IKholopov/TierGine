/*
   Copyright 2018 Igor Kholopov, All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   ==============================================================================
*/

#include <iostream>

#include <Log.h>
#include <Context.h>

namespace TierGine {

class CoutLogger : public ILogger {
public:
    enum T_LogColor {
        LC_None = 0,
        LC_Red = 31,
        LC_Green = 32,
        LC_Yellow = 33,
        LC_Cyan = 36
    };

    CoutLogger(T_LogColor color = LC_None) : color(color) {}

    virtual std::ostream& Write(const std::string& input) override
        { return std::cout << "\033[" << color << "m" << input << "\033[" << LC_None << "m"; }

private:
    T_LogColor color;
};

}
