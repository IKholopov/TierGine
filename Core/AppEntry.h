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

#pragma once

#include <type_traits>
#include <mutex>

#include <BaseApp.h>

namespace TierGine {

template<class TAppClass, typename = std::enable_if_t<std::is_base_of<TierGine::BaseApp, TAppClass>::value>>
class AppEntry {
public:
    AppEntry();
};

class CurrentApp {
public:
    CurrentApp();
    TG_Status RunApp();

private:
    static void SetCurrentApp(BaseApp* app);
    static std::unique_ptr<BaseApp> app;
    static std::mutex critical;

    std::unique_ptr<BaseApp> currentApp;

    template<class TAppClass, typename _>
    friend class AppEntry;
};


template<class TAppClass, typename _>
AppEntry<TAppClass, _>::AppEntry()
{
    CurrentApp::SetCurrentApp(new TAppClass());
}

}
