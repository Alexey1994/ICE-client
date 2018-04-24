package com.example.abar0217.stun;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView   tv            = (TextView) findViewById(R.id.sample_text);
        final EditText   dataEditText  = (EditText) findViewById(R.id.data);
        final Button     sendButton    = (Button) findViewById(R.id.send);
        final Button     receiveButton = (Button) findViewById(R.id.receive);

        dataEditText.setText("data");

        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv.setText( sendData(dataEditText.getText().toString()) );
            }
        });

        receiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv.setText( getData() );
            }
        });
    }

    public native String sendData(String data);
    public native String getData();
}
