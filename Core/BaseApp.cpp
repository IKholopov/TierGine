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

#include <BaseApp.h>
#include <CoutLog.h>

namespace TierGine {

BaseApp::BaseApp() :
    baseLog(new CoutLogger(), Log::LL_All),
    errorLog(new CoutLogger(CoutLogger::LC_Red), Log::LL_Error)
{
}

bool BaseApp::InitializeApp()
{
    for( auto initializer = initializers.begin();
         initializer != initializers.end(); ++initializer)
    {
        if(!(*initializer)->Initialize()) {
            for(auto rev = std::reverse_iterator<std::vector<std::unique_ptr<IInitializable>>::iterator>(initializer) + 1;
                rev != initializers.rend(); ++rev)
            {
                (*rev)->Deinitialize();
            }
            return false;
        }
    }
    return true;
}

void BaseApp::DeinitializeApp()
{
    for( auto initializer = initializers.rbegin();
         initializer != initializers.rend(); ++initializer )
    {
        (*initializer)->Deinitialize();
    }
}

TG_Status BaseApp::Main()
{
    while(!ShouldTerminate()) {
        TG_Status status = MainLoop();
        if( status != TG_Ok ) {
            return status;
        }
    }
    return TG_Ok;
}

}
