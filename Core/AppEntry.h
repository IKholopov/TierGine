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
