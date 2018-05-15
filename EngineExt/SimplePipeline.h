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
// Simple proxy for creation of pipeline with vertex and fragment shader

#include <Pipeline.h>
#include <Context.h>


namespace TierGine {

class SimplePipeline : public IPipeline {
public:
    SimplePipeline(IContext& context, std::string vertexShaderPath,
                   std::string fragmentShaderPath);

    IPipeline* GetPipeline() { return pipeline; }

    // IPipeline interface
    virtual IContext& GetContext() const override { return pipeline->GetContext(); }
    virtual void BindShader(const IShader* shader) override { GetContext().BindShader(shader, pipeline); }
    virtual void Build() override { pipeline->Build(); }
    virtual void Activate() const override { pipeline->Activate(); }
    virtual bool HasUniformVariable(std::string name) const override { return pipeline->HasUniformVariable( name ); }
    virtual UniformVariable GetUniformVariable(std::string name) const override { return pipeline->GetUniformVariable( name ); }
    virtual void SetUniformVariable(std::string name, Tensor value) const override { pipeline->SetUniformVariable(name, value); }
    virtual void SetUniformVariable(std::string name, int value) const override { pipeline->SetUniformVariable(name, value); }
    const std::unordered_map<std::string, UniformVariable>& GetUniformVariables() const override
    { return pipeline->GetUniformVariables(); }

private:
    IPipeline* pipeline;
};

}
