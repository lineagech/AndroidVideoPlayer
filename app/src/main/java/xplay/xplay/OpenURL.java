package xplay.xplay;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class OpenURL extends AppCompatActivity {

    private Button button_video;
    private Button button_rtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView( R.layout.openurl );

        button_video = findViewById(R.id.playvideo);
        button_rtmp = findViewById(R.id.playrtmp);

        button_video.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    EditText edit_text = findViewById(R.id.fileurl);
                    String file_url = edit_text.getText().toString();
                    Open( file_url );
                    finish();
                }
            }
        );

        button_rtmp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    EditText edit_text = findViewById(R.id.rtmpurl);
                    String rtmp_url = edit_text.getText().toString();
                    Open( rtmp_url );
                    finish();
                }
            }
        );

    }

    public native void Open(String url);
}
