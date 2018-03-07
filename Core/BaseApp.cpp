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
