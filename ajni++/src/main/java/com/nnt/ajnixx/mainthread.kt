package com.nnt.ajnixx

import java.util.*

class MainThread {
    companion object {

        // 主线程执行
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

        private var _tmr_satick = false
        private external fun jni_tick()

        // 启动独立JNI主线程调度
        fun StandaloneTick() {
            if (_tmr_satick)
                return
            _tmr_satick = true

            // 主线程执行一次，初始化 cross::MainThread 执行环境
            jni_tick()

            // 启动定时器模拟帧循环
            Timer().schedule(object : TimerTask() {
                override fun run() {
                    Invoke {
                        jni_tick()
                    }
                }
            }, 100, 100);
        }
    }
}
