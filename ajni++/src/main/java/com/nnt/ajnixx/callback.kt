package com.nnt.ajnixx

class Callback
{
    // 调用callback
    operator fun invoke() {

    }

    operator fun invoke(v0:Any) {

    }

    operator fun invoke(v0:Any, v1:Any) {

    }

    // 增加计数，防止native层调用之后释放
    fun grab()
    {

    }

    // 释放计数，如果计数=0则会释放native层的函数
    fun drop()
    {

    }
}