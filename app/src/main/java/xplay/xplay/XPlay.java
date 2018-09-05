package xplay.xplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/* Inherit Window Interface */
public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        InitView(holder.getSurface());

        /* Need to set on Android 8.0 */
        setRenderer(this);
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {

    }

    public native void InitView( Object surface );

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }
}
