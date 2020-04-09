package com.wybosys.ajni

import android.app.Activity

object AJni {

    // 初始化
    external fun Init()

    // 执行入口函数
    external fun Main()

    // 测试入口
    external fun Test(act:Activity)

    init {
        // 自动加载so
        System.loadLibrary("ajni-test")
    }
}