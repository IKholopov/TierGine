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
#include <MazePhysics.h>

namespace TG = TierGine;

std::unique_ptr<TierGine::PhysicsWorld::ICollisionsIterator> MazePhysicsEngine::GetCollisions(TierGine::IPhysicsObject* obj)
{
    return grid->GetCollisions(obj);
}

void MazePhysicsEngine::AddCollisionToResolve(std::pair<TierGine::ICollisionSource*, TierGine::IPhysicsObject*> collision)
{
    auto [source, object] = collision;
    std::unique_ptr<TG::ICollisionSolver> solver = source->GetSolverFor(*object);
    object->ApplyWorldUpdate(solver.get());
}
