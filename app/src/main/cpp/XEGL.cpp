//
// Created by Chia-Hao on 2018/8/27.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include "XEGL.h"
#include "XLog.h"

class CXEGL : public XEGL
{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    virtual void Draw()
    {
        if( display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE ) return;
        eglSwapBuffers(display, surface);
    }

    virtual bool Init( void* win ) /* ANativeWindow */
    {
        ANativeWindow* nwin = (ANativeWindow*)(win);

        /* Initialize EGL */

        /* Get EGL display */
        display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        if( display == EGL_NO_DISPLAY )
        {
            XLOGE("eglGetDisplay Failed.");
            return false;
        }
        XLOGI("eglGetDisplay Succeeded.");

        /* Initialize EGL Display */
        if( eglInitialize(display, 0, 0) != EGL_TRUE )
        {
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
            XLOGE("eglCreateContext Failed.");
            return false;
        }
        XLOGI("eglCreateContext Succeeded.");

        if( eglMakeCurrent( display, surface/*draw*/, surface/*read*/, context ) != EGL_TRUE )
        {
            XLOGE("eglMakeCurrent Failed.");
            return false;
        }

        return true;
    }
};

XEGL* XEGL::Get()
{
    static CXEGL egl;
    return (&egl);
}