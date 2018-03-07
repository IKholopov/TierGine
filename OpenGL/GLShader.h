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

#include <Shader.h>
#include <GLInclude.h>

namespace TierGine {

class GLShader : public IShader {
public:
    GLShader(IShader::Type type, IContext& context);
    ~GLShader();

    // IShader interface
    virtual bool IsValid() const override;
    virtual Type GetType() const override;
    virtual void LoadFromBuffer(const char* buffer) override;
    virtual void LoadFromPath(std::string path) override;

    virtual IContext& GetContext() const override { return context; }

    GLuint GetShaderId() const { return id; }

private:
    GLuint id;
    IShader::Type type;
    IContext& context;
};

}
