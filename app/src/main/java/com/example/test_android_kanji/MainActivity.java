package com.example.test_android_kanji;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.bigfishgames.kanji.KanjiActivity;
import com.example.test_android_kanji.databinding.ActivityMainBinding;

public class MainActivity extends KanjiActivity {

    private ActivityMainBinding binding;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //binding = ActivityMainBinding.inflate(getLayoutInflater());
        //setContentView(binding.getRoot());

        // Example of a call to a native method
        //TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'test_android_kanji' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}