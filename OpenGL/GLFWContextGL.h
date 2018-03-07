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

#include <assert.h>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <Context.h>
#include <WindowGLFW.h>
#include <GLShader.h>
#include <GLProgram.h>

namespace TierGine {

class GLFWContextGL : public IContext {
public:
    GLFWContextGL(WindowGLFW& window) : window(window) {}

    // IContext interface
    virtual TG_Status Activate() override;

    virtual IPipeline* CreatePipeline() override;
    virtual void DeletePipeline(IPipeline* pipeline) override;

    virtual IShader* CreateShader(IShader::Type shaderType) override;
    virtual void BindShader(const IShader* shader, IPipeline* pipeline) override;
    virtual void DeleteShader(IShader* shader) override;

private:
    WindowGLFW& window;
    std::unordered_map<const IShader*, std::unique_ptr<GLShader>> shaders;
    std::unordered_map<const IPipeline*, std::unique_ptr<GLProgram>> pipelines;
};

}
