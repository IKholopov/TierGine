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

#include <Drawable.h>
#include <Material.h>
#include <Mesh.h>
#include <Context.h>

namespace TG = TierGine;

class OnScreen: public TG::IDrawable {
public:
    OnScreen(int x, int y, int width, int height, TG::IMaterial* material, TG::IContext& context);
    OnScreen(int x, int y, int width, int height, TG::IMesh* mesh, TG::IMaterial* material, TG::IContext& context);

    // IDrawable interface
    virtual TG::IPipeline* GetPipeline() const override { return pipeline; }
    virtual void SetPipeline(TierGine::IPipeline* pipeline, TierGine::UniformVariable textureVar,
                             TierGine::UniformVariable normalMapVar=TG::UniformVariable{},
                             TierGine::UniformVariable materialVar=TG::UniformVariable{}) override
    {
        this->pipeline = pipeline;
        this->textureVar = textureVar;
    }
    virtual void SetRenderingMode(TierGine::TRenderingMode mode, TierGine::TPolygonRenderStyle style) override {}
    virtual void Draw() override;

protected:
    void BindViewport();
    void UnbindViewport();

private:
    TG::IMesh* mesh;
    TierGine::UniformVariable textureVar;
    int x;
    int y;
    int width;
    int height;
    int oldViewport[4];

    TG::IPipeline* pipeline;
    TG::IMaterial* material;
};

class Map: public OnScreen {
public:
    Map(int x, int y, int width, int height, TG::IMesh* walls, TG::IContext& context);

    void SetPosition(float x, float y) {
        posX = x;
        posY = y;
    }
    void SetAngle(float a) { angle = a; }

    virtual void Draw() override;

private:
    TG::IMesh* player;
    float posX;
    float posY;
    float angle;
    float scale;
};
