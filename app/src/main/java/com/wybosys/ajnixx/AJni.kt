package com.wybosys.ajni

object AJni {
    init {
        // 自动加载so
        System.loadLibrary("ajni++")
    }
}