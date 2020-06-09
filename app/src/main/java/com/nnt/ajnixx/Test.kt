package com.nnt.ajnixx

class Test {

    fun test0(): String
    {
        return "Test0 Member Function String"
    }

    val test0 = "Test0 Member Variable String"

    companion object {

        // 返回测试结果
        external fun Test(): String

        fun Test0(): String {
            return "Test0 Static Function String"
        }

        val Test0 = "Test0 Static Variable String"
    }
}