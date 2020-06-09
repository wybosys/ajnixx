package com.nnt.ajnixx

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.getkeepsafe.relinker.ReLinker
import com.nnt.ajnixx.Test
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity() : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 加载so
        ReLinker.loadLibrary(this, "tester")

        // 测试
        sample_text.text = Test.Test()
    }
}
