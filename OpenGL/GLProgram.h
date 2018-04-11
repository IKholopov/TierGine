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

#include <Pipeline.h>
#include <GLShader.h>
#include <unordered_map>

namespace TierGine {

class GLProgram : public IPipeline {
public:
    GLProgram(IContext& context);
    ~GLProgram();

    // IPipeline interface
    virtual IContext& GetContext() const override { return context; }
    virtual void BindShader(const IShader* shader) override;
    virtual void Build() override;
    virtual void Activate() const override;
    virtual UniformVariable GetUniformVariable(std::string name) const override;
    virtual void SetUniformVariable(std::string name, Tensor value) const override;
    virtual void SetUniformVariable(std::string name, int value) const override;
    virtual const std::unordered_map<std::string, UniformVariable>& GetUniformVariables() const override
    { return uniforms; }

    void BindShader(const GLShader& shader);

private:
    GLuint programId;
    IContext& context;
    std::unordered_map<std::string, UniformVariable> uniforms;

    void setUniform(GLint uniformLoc, Tensor value) const;
};

}
