package xplay.xplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

/* Inherit Window Interface */
public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        InitView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {

    }

    public native void InitView( Object surface );
}
