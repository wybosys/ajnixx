package com.nnt.ajnixx

import android.app.Activity
import android.content.Context
import android.os.Bundle
import java.lang.ref.WeakReference

open class Activity : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // 之前必须已经将so加载成功

        // 绑定当前上下文至jni层
        Bind(this)
    }

    companion object {
        private var _current = WeakReference<Activity?>(null)

        fun Bind(act: Activity?) {
            _current = WeakReference<Activity?>(act)
            if (act == null) {
                jni_bind(null, null)
            } else {
                jni_bind(act, act.applicationContext)
            }
        }

        private external fun jni_bind(act: Activity?, ctx: Context?)

        fun Unbind() {
            Bind(null)
        }

        fun Get(): Activity {
            return _current.get() as Activity
        }

        fun Current(): Activity? {
            return _current.get()
        }

        fun Clear() {
            Bind(null)
        }
    }
}