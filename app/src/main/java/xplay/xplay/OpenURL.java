package xplay.xplay;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class OpenURL extends AppCompatActivity {

    private Button button_video;
    private Button button_rtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        button_video = findViewById(R.id.playvideo);
        button_rtmp = findViewById(R.id.playrtmp);

        button_video.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                }
            }
        );

    }

    public native void Open(String url);
}
