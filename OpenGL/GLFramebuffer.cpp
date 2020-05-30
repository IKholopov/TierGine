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
#include <GLFramebuffer.h>

#include <EngineException.h>

namespace TierGine {

GLFramebuffer::GLFramebuffer(IContext& context, int width, int height): context(context),
    framebufferId(0),
    depthBuffer(0),
    width(width),
    height(height)
{
    for(int i = 0; i < 4; ++i) {
        oldViewportValues[i] = -1;
    }

    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    texture.reset(new BufferTexture(context, width, height));

    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw EngineException("Failed to setup framebuffer\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLFramebuffer::~GLFramebuffer()
{
    glDeleteFramebuffers(1, &framebufferId);
}

void GLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    glGetIntegerv(GL_VIEWPORT, oldViewportValues);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(oldViewportValues[0], oldViewportValues[1],
            oldViewportValues[2], oldViewportValues[3]);
}

ITexture* GLFramebuffer::GetTexture() const
{
    return texture.get();
}

IContext& GLFramebuffer::GetContext() const
{
    return context;
}

GLFramebuffer::BufferTexture::BufferTexture(IContext& context, int width, int height):
    context(context),
    width(width),
    height(height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
}

void GLFramebuffer::BufferTexture::Activate() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

}

