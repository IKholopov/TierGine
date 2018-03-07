/*
   Copyright 2018 Igor Kholopv, All rights reserved.

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

#include <assert.h>

#include <Context.h>
#include <Window.h>
#include <Initializable.h>
#include <GLFW/glfw3.h>

namespace TierGine {

class WindowGLFW : public IWindow {
public:
    struct Config {
      int Width;
      int Height;
      const std::string Title;
      bool Fullscreen;

      Config(int width = 640, int height = 480,
             const std::string title = "OpenGL", bool fullscreen = false) :
          Width(width),
          Height(height),
          Title(title),
          Fullscreen(fullscreen)
      {}
    };

    class Initializer : public IInitializable {
    public:
        Initializer(WindowGLFW& window, bool withContext) :
            window(window),
            withContext(withContext)
        {}

        // IInitializable interface
        virtual bool Initialize() override { return window.Create(withContext) == TG_Ok; }
        virtual bool InitializeThread() override { return true; }
        virtual void Deinitialize() override {}
        virtual void DeinitializeThread() override {}
        virtual bool IsInitialized() override { return true; }
        virtual bool IsInitializedOnThread() override { return  true; }

    private:
        WindowGLFW& window;
        bool withContext;
    };

    // Takes ownership of context
    WindowGLFW(const Config& config, IContext* context);
    WindowGLFW(const Config& config, const WindowGLFW& otherContext);

    std::unique_ptr<Initializer> GetContextInitializer();
    std::unique_ptr<Initializer> GetNoContextInitializer();
    GLFWwindow* GetWindow() const { return window; }

    // IWindow interface
    virtual void Resize(int width, int height) override {} //TODO
    virtual void Move(int x, int y) override {} //TODO
    virtual void Fullscreen() override {} //TODO
    virtual void Update() const override;
    virtual bool ToClose() const override { assert(window); return glfwWindowShouldClose(window); }

    TG_Status Create(bool useContext);
    IContext* GetContext() const { return context.get(); }

private:
    Config config;
    std::shared_ptr<IContext> context;
    GLFWwindow* window;
};

}
