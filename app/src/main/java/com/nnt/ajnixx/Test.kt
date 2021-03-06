package com.nnt.ajnixx

class Info {
    var abc: String? = "abc"
    var cde: Int = 123
    var nul: Any? = null

    fun proc(msg: String) {
        println("Info::Proc " + msg)
    }
}

class Test {

    fun test0(): String {
        return "Test0 Member Function String"
    }

    val test0 = "Test0 Member Variable String"

    fun info(): Info {
        // var info = null as Info // 放开注释测试jni对java层异常的捕获
        // info.abc = "123"
        return Info()
    }

    fun value_async(cb: Callback) {
        cb.grab()
        Thread {
            // 不需要于主线程中回调
            //MainThread.Invoke {
            cb(true, "abc")
            cb.drop()
            //}
        }.start()
    }

    fun null_async(cb: Callback) {
        cb.grab()
        Thread {
            cb(null)
            cb.drop()
        }.start()
    }

    fun info_async(cb: Callback, str: String, cb2: Callback) {
        cb.grab()
        Thread {
            val info = Info()
            info.abc = "cde"
            info.cde = 456
            cb(info)
            cb.drop()
        }.start()
    }

    fun info_async(cb: Callback) {
        cb("0")
    }

    companion object {

        // 返回测试结果
        external fun Test(): String

        fun Test0(): String {
            return "Test0 Static Function String"
        }

        val Test0 = "Test0 Static Variable String"
    }
}

fun GetInfo(): Info {
    return Info()
}

var abc = 123
