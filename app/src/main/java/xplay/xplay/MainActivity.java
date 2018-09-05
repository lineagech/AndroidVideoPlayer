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
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button bt;

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

        bt = findViewById( R.id.open_button );
        bt.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick( View view )
            {
                Log.e( "XPlay", "open button click" );
                /* Open path selction window */
                Intent intent = new Intent(); // for opening the window
                intent.setClass( MainActivity.this, OpenURL.class );
                startActivity( intent );
            }
        } );
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
