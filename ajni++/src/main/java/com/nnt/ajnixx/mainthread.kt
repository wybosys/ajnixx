package com.nnt.ajnixx

class MainThread
{
    companion object {

        fun Invoke(fn: () -> Unit) {
            val cur = Activity.Current()

            if (cur == null) {
                // 没有绑定主activity，只能直接运行
                fn()
                return
            }

            // 运行于UI线程
            cur.runOnUiThread(fn)
        }

    }
}