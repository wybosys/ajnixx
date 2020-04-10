package com.wybosys.ajni

import android.graphics.Color
import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout

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

        //val bv = BlockView(this)
        //bv.setBackgroundColor(Color.RED)

        val lyr = findViewById<ConstraintLayout>(R.id.main)
        //lyr.addView(bv, ViewGroup.LayoutParams(100, 100))

        // sample_text.text = stringFromJNI()
    }
}
