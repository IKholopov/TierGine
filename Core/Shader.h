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

#include <TierGine.CoreDefs.h>

#include <LoadableResource.h>

namespace TierGine {

interface IContext;

interface IShader : public ILoadableResource {
    enum Type {
        T_Vertex,
        T_TessellationControl,
        T_TessellationEvaluation,
        T_Geometry,
        T_Fragment,
        T_Compute
    };
    \
    IShader() {}
    virtual ~IShader() {}

    virtual bool IsValid() const = 0;
    virtual Type GetType() const = 0;
    virtual void LoadFromBuffer(const char* buffer) = 0;

    virtual IContext& GetContext() const = 0;

private:
    IShader(const IShader&) = delete;
    IShader& operator=(const IShader&) = delete;
};

}
