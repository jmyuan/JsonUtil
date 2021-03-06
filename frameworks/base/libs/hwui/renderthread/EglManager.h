/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EGLMANAGER_H
#define EGLMANAGER_H

#include <cutils/compiler.h>
#include <EGL/egl.h>
#include <ui/GraphicBuffer.h>
#include <utils/StrongPointer.h>

namespace android {
namespace uirenderer {
namespace renderthread {

class RenderThread;

// This class contains the shared global EGL objects, such as EGLDisplay
// and EGLConfig, which are re-used by CanvasContext
class EglManager {
public:
    // Returns true on success, false on failure
    void initialize();

    bool hasEglContext();
    void requireGlContext();

    void usePBufferSurface();
    EGLSurface createSurface(EGLNativeWindowType window);
    void destroySurface(EGLSurface surface);

    void destroy();

    bool isCurrent(EGLSurface surface) { return mCurrentSurface == surface; }
    // Returns true if the current surface changed, false if it was already current
    bool makeCurrent(EGLSurface surface);
    void beginFrame(EGLSurface surface, EGLint* width, EGLint* height);
    bool swapBuffers(EGLSurface surface);

    bool enableDirtyRegions(EGLSurface surface);

    void setTextureAtlas(const sp<GraphicBuffer>& buffer, int64_t* map, size_t mapSize);
    
    /// M: set program atlas to share program bianries
    void setProgramAtlas(int fd, int length, int64_t* programMap, int programCount);

private:
    friend class RenderThread;

    EglManager(RenderThread& thread);
    // EglContext is never destroyed, method is purposely not implemented
    ~EglManager();

    void loadConfig();
    void createContext();
    void initAtlas();
    
    /// M: For program binaries
    void initProgramAtlas();

    RenderThread& mRenderThread;

    EGLDisplay mEglDisplay;
    EGLConfig mEglConfig;
    EGLContext mEglContext;
    EGLSurface mPBufferSurface;

    const bool mRequestDirtyRegions;
    bool mCanSetDirtyRegions;

    EGLSurface mCurrentSurface;

    sp<GraphicBuffer> mAtlasBuffer;
    int64_t* mAtlasMap;
    size_t mAtlasMapSize;

    /// M: For program binaries
    int mProgramFd;
    int mProgramLength;
    int64_t* mProgramMap;
    int mProgramCount;
};

} /* namespace renderthread */
} /* namespace uirenderer */
} /* namespace android */

#endif /* EGLMANAGER_H */
