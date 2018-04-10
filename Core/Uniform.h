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

#include <vector>
#include <assert.h>

namespace TierGine {

interface IPipeline;

class UniformVariable {
public:
    enum TUniformType {
        UT_FLOAT,
        UT_INT,
        UT_VEC_2,
        UT_VEC_3,
        UT_VEC_4,
        UT_MAT_2,
        UT_MAT_3,
        UT_MAT_4,
        UT_USERSTRUCT,
        UT_INVALID
    };

    UniformVariable();

    UniformVariable(IPipeline* pipeline, TUniformType type, std::string name) :
        pipeline(pipeline),
        type(type),
        name(name)
    { assert(pipeline != nullptr); }
    UniformVariable(const UniformVariable& other);
    UniformVariable(const UniformVariable&& other);
    UniformVariable& operator=(const UniformVariable& other);
    UniformVariable& operator=(const UniformVariable&& other);

    std::string GetName() const { return name; }
    bool IsValid() const { return type != UT_INVALID; }

    template <class T>
    void Set(T& value);

private:
    IPipeline* pipeline;
    TUniformType type;
    std::string name;
};

typedef std::vector<UniformVariable> UniformArray;

}
