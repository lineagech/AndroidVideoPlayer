package xplay.xplay;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements Runnable, SeekBar.OnSeekBarChangeListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button button;
    private SeekBar seekbar;
    private Thread curr_thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    
        /* Remove title */
        supportRequestWindowFeature( Window.FEATURE_NO_TITLE );

        /* Full Screen and Hiding status */
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN,
                              WindowManager.LayoutParams.FLAG_FULLSCREEN);
        /* Landscape orientation */
        setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );

        setContentView(R.layout.activity_main);

        button = findViewById( R.id.open_button );
        button.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick( View view )
            {
                Log.e( "XPlay", "open button click" );
                /* Open path selection window */
                Intent intent = new Intent(); // for opening the window
                intent.setClass( MainActivity.this, OpenURL.class );
                startActivity( intent );
            }
        } );

        seekbar = findViewById( R.id.aplayseek );
        seekbar.setMax(1000);
        seekbar.setOnSeekBarChangeListener( this );

        /* Run the thread */
        curr_thread = new Thread(this);
        curr_thread.start();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void jni_seek( double progress );
    public native double GetCurrProgress();

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        jni_seek( (double)seekBar.getProgress()/(double)seekBar.getMax() );
    }

    @Override
    public void run() {
        /* Running this thread is to update SeekBar's progress */
        while( true )
        {
            seekbar.setProgress((int) GetCurrProgress() * 1000);
            try{
                Thread.sleep( 30 );
            } catch ( InterruptedException e ) {
                e.printStackTrace();
            }
        }
    }
}
