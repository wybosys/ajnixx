package com.nnt.ajnixx

import android.app.Activity
import java.lang.ref.WeakReference

class Activity {
    companion object {
        private var _current = WeakReference<Activity?>(null)

        fun Bind(act: Activity?) {
            _current = WeakReference<Activity?>(act)
        }

        fun Get(): Activity {
            return _current.get() as Activity
        }

        fun Clear() {
            Bind(null)
        }

        fun Invoke(fn: () -> Unit) {
            if (_current.get() == null) {
                // 没有绑定主activity，只能直接运行
                fn()
                return
            }

            // 运行于UI线程
            val cur = _current.get() as Activity
            cur.runOnUiThread(fn)
        }
    }
}