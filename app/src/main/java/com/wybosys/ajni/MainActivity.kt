package com.wybosys.ajni

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity {

    constructor() {
        AJni.Init()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        AJni.Main()

        // 测试函数
        AJni.Test(this)

        // sample_text.text = stringFromJNI()
    }
}
