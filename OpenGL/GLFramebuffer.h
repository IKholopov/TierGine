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
#include <Framebuffer.h>
#include <GLTexture.h>

namespace TierGine {

class GLFramebuffer: public IFramebuffer {
public:
    class BufferTexture: public ITexture {
    public:
        BufferTexture(IContext& context, int width, int height);

        // ILoadableResource interface
        virtual void LoadFromPath(std::string) override { assert(false); }

        // ITexture interface
        virtual IContext& GetContext() const override { return context; }
        virtual void Activate() const override;

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }

    private:
        IContext& context;
        GLuint id;
        int width;
        int height;
    };

    GLFramebuffer(IContext& context, int width, int height);
    ~GLFramebuffer();

    // IFramebuffer interface
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual ITexture* GetTexture() const override;
    virtual IContext& GetContext() const override;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    IContext& context;
    std::unique_ptr<ITexture> texture;
    GLuint framebufferId;
    GLuint depthBuffer;
    GLint oldViewportValues[4];
    int width;
    int height;
};

}
