package com.wybosys.ajni

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.getkeepsafe.relinker.ReLinker

class MainActivity() : AppCompatActivity() {

    init {
        ReLinker.loadLibrary(this, "ajni++")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }
}
