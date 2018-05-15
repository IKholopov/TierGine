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

#include <Physics/PhysicsEngine.h>
#include <Tensor.h>
#include <vector>

namespace TG = TierGine;

const Byte FrontWall = BIT(1);
const Byte BackWall = BIT(2);
const Byte RightWall = BIT(3);
const Byte LeftWall = BIT(4);
const Byte Floor = BIT(5);
const Byte Ceiling = BIT(6);

inline Byte NextWall(Byte b) {
    if( b < LeftWall ) {
        return b << 1;
    }
    return FrontWall;
}

inline Byte NextWall(Byte b, Byte filter) {
    assert((filter & (FrontWall | BackWall | RightWall | LeftWall))!=0);
    Byte n;
    for(n = NextWall(b); (n & filter) == 0; n = NextWall(n))
    {}
    return n;
}

struct MeshData {
    TG::Tensor v, norm, uv;
    MeshData(const std::initializer_list<TG::Tensor>& init);
    MeshData(MeshData&& other);

    void Append(MeshData&& other);
};

class GridEntry {
public:
    GridEntry(float x, float y, float z) : x(x), y(y), z(z) {}
    GridEntry(const GridEntry& other): x(other.x), y(other.y), z(other.z) {}
    GridEntry(GridEntry&& other): x(other.x), y(other.y), z(other.z) {}

    glm::vec3 GetPosition() const { return glm::vec3(x, y, z); }
    virtual void GetCollisionSources(std::vector<TG::ICollisionSource*>& sources) = 0;


    virtual MeshData GetMesh() const = 0;

private:
    float x;
    float y;
    float z;
};

class WalledEntry : public GridEntry {
public:
    WalledEntry(float x, float y, float z, Byte directions) : GridEntry(x, y, z), directions(directions) {}
    WalledEntry(const WalledEntry& other, Byte directions): GridEntry(other), directions(directions) {}

    const Byte& GetWalls() const { return directions; }
    void Add(Byte wall) { directions &= wall; }
    void Remove(Byte wall) { directions &= (~wall); }
    void GenerateCollisions();
    glm::vec2 GetDirectionFrom(Byte wall, bool forward) const;
    glm::vec3 GetPositionFrom(Byte wall, bool forward) const;

    virtual void GetCollisionSources(std::vector<TG::ICollisionSource*>& sources) override;
    virtual TG::ICollisionSource* GetCollisionFor(Byte wall);

    virtual MeshData GetMesh() const override;

private:
    std::vector<std::unique_ptr<TG::ICollisionSource>> collisions;
    Byte directions;
};

class HorizontalCell : public GridEntry {
public:
    HorizontalCell(const glm::vec3& lower, const glm::vec3& higher, Byte directions) :
        GridEntry(lower.x, lower.y, lower.z),
        lower(lower),
        higher(higher),
        directions(directions)
    {}

    virtual void GetCollisionSources(std::vector<TierGine::ICollisionSource*>& sources) override {}
    virtual MeshData GetMesh() const override;

    const Byte& GetWalls() const { return directions; }
    void Add(Byte wall) { directions &= wall; }
    void Remove(Byte wall) { directions &= (~wall); }
    const glm::vec3& GetLower() const { return lower; }
    const glm::vec3& GetHigher() const { return higher; }

private:
    const glm::vec3 lower;
    const glm::vec3 higher;
    Byte directions;
};

struct WallTarget {
  WalledEntry* entry;
  Byte wall;
  bool forward;

  WallTarget(): entry(nullptr), wall(0), forward(false) {}
  WallTarget(WalledEntry* entry, Byte wall, bool forward):
      entry(entry), wall(wall), forward(forward)
  {}
};

class Grid {
public:
    Grid(int x, int y, int z);

    std::vector<std::vector<std::unique_ptr<WalledEntry>>>& operator[](int x) {
        assert(this->x > x && x >=0);
        return grid[x];
    }

    std::unique_ptr<HorizontalCell>& Horizontal(int y) {
        assert(this->y >= y && y >= 0);
        return horizontal[y];
    }

    std::unique_ptr<TierGine::PhysicsWorld::ICollisionsIterator> GetCollisions(TG::IPhysicsObject* obj) const;
    WallTarget GetTarget(const glm::vec3& point, const glm::vec3& direction) const;

    int XLength() const { return x; }
    int YLength() const { return y; }
    int ZLength() const { return z; }

private:
    std::vector<std::vector<std::vector<std::unique_ptr<WalledEntry>>>> grid;
    std::vector<std::unique_ptr<HorizontalCell>> horizontal;
    int x;
    int y;
    int z;
};
