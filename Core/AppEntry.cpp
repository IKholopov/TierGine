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

#include <AppEntry.h>

#include <assert.h>

#include <EngineException.h>

namespace TierGine {

std::unique_ptr<BaseApp> CurrentApp::app;
std::mutex CurrentApp::critical;

TG_Status CurrentApp::RunApp()
{
    std::lock_guard<std::mutex> guard(CurrentApp::critical);
    assert(currentApp != nullptr);
    if(!currentApp->InitializeApp()) {
        return TG_Critical;
    }
    TG_Status status = currentApp->Main();
    currentApp->DeinitializeApp();
    currentApp.reset(nullptr);
    return status;
}

CurrentApp::CurrentApp()
{
    currentApp = std::move(CurrentApp::app);
}

void CurrentApp::SetCurrentApp(TierGine::BaseApp* app)
{
    std::lock_guard<std::mutex> guard(CurrentApp::critical);
    assert(CurrentApp::app == nullptr);
    assert(app != nullptr);
    CurrentApp::app.reset(app);
}

}

int main(int argc, const char** argv)
{
    TierGine::CurrentApp app;
    try {
        return app.RunApp();
    } catch(TierGine::EngineException& exception) {
        TierGine::Log::Error() << exception.what() << std::endl;
        return 1;
    }
}
