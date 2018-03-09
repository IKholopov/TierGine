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
#include <Formats.Model.h>

#include <EngineException.h>
#include <Format.Model.Obj.h>
#include <algorithm>

namespace TierGine {

std::unique_ptr<TierGine::IMeshLoader> Formats::GetLoaderFor(std::string path)
{
    std::string ext = path.substr(path.find_last_of(".") + 1);
    if(ext.size() == 0) {
        throw EngineException("Cannot determing extension of path: " + path);
    }
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if(ext.compare("obj") == 0) {
        return std::make_unique<Formats::ModelObjLoader>(path);
    }
    throw EngineException("Cannot open file of format: " + ext + "\nPath: " + path);
}

}
