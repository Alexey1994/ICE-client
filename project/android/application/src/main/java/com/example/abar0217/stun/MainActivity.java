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
        final ScrollView scrollView    = (ScrollView) findViewById(R.id.scroll_view);
        final EditText   hostEditText  = (EditText) findViewById(R.id.host);
        final EditText   portEditText  = (EditText) findViewById(R.id.port);
        final Button     refreshButton = (Button) findViewById(R.id.refresh);

        scrollView.smoothScrollTo(0, 0);

        refreshButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String log = null;

                String host = hostEditText.getText().toString();
                int    port;

                try{
                    port = Integer.parseInt( portEditText.getText().toString() );
                } catch(NumberFormatException e){
                    port = 3478;
                }

                String arguments[] = { host, String.valueOf(port) };

                //tv.setText( stringFromJNI(log) );
                tv.setText( stringFromJNI(arguments) );
            }
        });
    }

    public native String stringFromJNI(String []arguments);
}
