//
// Created by Chia-Hao on 2018/8/27.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include "XEGL.h"
#include "XLog.h"
#include <mutex>

class CXEGL : public XEGL
{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mutex;

    virtual void Draw()
    {   
        mutex.lock();
        if( display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE ) {
            mutex.unlock();
            return;
        }
        eglSwapBuffers(display, surface);
        mutex.unlock();
    }

    virtual void Close()
    {      
        mutex.lock();
        if( display == EGL_NO_DISPLAY ){ 
            mutex.unlock();
            return; 
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if( surface != EGL_NO_SURFACE )
            eglDestroySurface(display, surface);
        if( context != EGL_NO_CONTEXT )
            eglDestroyContext(display, context);
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mutex.unlock();
    }

    virtual bool Init( void* win ) /* ANativeWindow */
    {   
        ANativeWindow* nwin = (ANativeWindow*)(win);
        Close();
        /* Initialize EGL */
        mutex.lock();
        /* Get EGL display */
        display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        if( display == EGL_NO_DISPLAY )
        {   
            mutex.unlock();
            XLOGE("eglGetDisplay Failed.");
            return false;
        }
        XLOGI("eglGetDisplay Succeeded.");

        /* Initialize EGL Display */
        if( eglInitialize(display, 0, 0) != EGL_TRUE )
        {   
            mutex.unlock();
            XLOGE("eglInitialize Failed.");
            return false;
        }
        XLOGI("eglInitialize Succeeded.");

        EGLint configSpec [] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfigs = 0;
        if( eglChooseConfig(display, configSpec, &config, 1, &numConfigs) != EGL_TRUE )
        {   
            mutex.unlock();
            XLOGE("eglChooseConfig Failed.");
            return false;
        }
        XLOGI("eglChooseConfig Succeeded.");

        surface = eglCreateWindowSurface(display, config, nwin, NULL);

        /* Create and Open Context */
        const EGLint ctxAttr [] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr );
        if( context == EGL_NO_CONTEXT )
        {   
            mutex.unlock();
            XLOGE("eglCreateContext Failed.");
            return false;
        }
        XLOGI("eglCreateContext Succeeded.");

        if( eglMakeCurrent( display, surface/*draw*/, surface/*read*/, context ) != EGL_TRUE )
        {   
            mutex.unlock();
            XLOGE("eglMakeCurrent Failed.");
            return false;
        }
        mutex.unlock();
        return true;
    }
};

XEGL* XEGL::Get()
{
    static CXEGL egl;
    return (&egl);
}