package com.nnt.ajnixx

class Info {
    var abc: String? = "abc"
    var cde: Int = 123
    var nul: Any? = null

    fun proc(msg: String)
    {
        println("Info::Proc " + msg)
    }
}

class Test {

    fun test0(): String {
        return "Test0 Member Function String"
    }

    val test0 = "Test0 Member Variable String"

    fun info(): Info {
        return Info()
    }

    fun value_async(cb: Callback) {
        cb.grab()
        Thread {
            // 不需要于主线程中回调
            //MainThread.Invoke {
            cb(123, "abc")
            cb.drop()
            //}
        }.start()
    }

    fun null_async(cb: Callback) {
        cb.grab()
        cb(null)
        cb.drop()
    }

    fun info_async(cb: Callback) {
        val info = Info()
        info.abc = "cde"
        info.cde = 456
        cb(info)
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
