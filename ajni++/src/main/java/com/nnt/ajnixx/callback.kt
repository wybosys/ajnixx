package com.nnt.ajnixx

class Callback {

    // 调用callback
    operator fun invoke() {
        jni_invoke(id)
    }

    external fun jni_invoke(id: Long): Void

    operator fun invoke(v0: Any) {
        jni_invoke1(id, v0)
    }

    external fun jni_invoke1(id: Long, v0: Any): Void

    operator fun invoke(v0: Any, v1: Any) {
        jni_invoke2(id, v0, v1)
    }

    external fun jni_invoke2(id: Long, v0: Any, v1: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any) {

    }

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any) {

    }

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any) {

    }

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any) {

    }

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any, v6: Any) {

    }

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any, v6: Any, v7: Any) {

    }

    operator fun invoke(
        v0: Any,
        v1: Any,
        v2: Any,
        v3: Any,
        v4: Any,
        v5: Any,
        v6: Any,
        v7: Any,
        v8: Any
    ) {

    }

    // 增加计数，防止native层调用之后释放
    fun grab() {
        jni_grab(id)
    }

    external fun jni_grab(id: Long): Void

    // 释放计数，如果计数=0则会释放native层的函数
    fun drop(): Boolean {
        return jni_drop(id)
    }

    external fun jni_drop(id: Long): Boolean

    // 对应的计数函数
    var id: Long = 0
}