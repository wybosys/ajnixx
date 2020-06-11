package com.nnt.ajnixx

class Callback {

    // 调用callback
    operator fun invoke() {
        jni_invoke(id)
    }

    private external fun jni_invoke(id: Long): Void

    operator fun invoke(v0: Any) {
        jni_invoke1(id, v0)
    }

    private external fun jni_invoke1(id: Long, v0: Any): Void

    operator fun invoke(v0: Any, v1: Any) {
        jni_invoke2(id, v0, v1)
    }

    private external fun jni_invoke2(id: Long, v0: Any, v1: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any) {
        jni_invoke3(id, v0, v1, v2)
    }

    private external fun jni_invoke3(id: Long, v0: Any, v1: Any, v2: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any) {
        jni_invoke4(id, v0, v1, v2, v3)
    }

    private external fun jni_invoke4(id: Long, v0: Any, v1: Any, v2: Any, v3: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any) {
        jni_invoke5(id, v0, v1, v2, v3, v4)
    }

    private external fun jni_invoke5(id: Long, v0: Any, v1: Any, v2: Any, v3: Any, v4: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any) {
        jni_invoke6(id, v0, v1, v2, v3, v4, v5)
    }

    private external fun jni_invoke6(id: Long, v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any, v6: Any) {
        jni_invoke7(id, v0, v1, v2, v3, v4, v5, v6)
    }

    private external fun jni_invoke7(
        id: Long,
        v0: Any,
        v1: Any,
        v2: Any,
        v3: Any,
        v4: Any,
        v5: Any,
        v6: Any
    ): Void

    operator fun invoke(v0: Any, v1: Any, v2: Any, v3: Any, v4: Any, v5: Any, v6: Any, v7: Any) {
        jni_invoke8(id, v0, v1, v2, v3, v4, v5, v6, v7)
    }

    private external fun jni_invoke8(
        id: Long,
        v0: Any,
        v1: Any,
        v2: Any,
        v3: Any,
        v4: Any,
        v5: Any,
        v6: Any,
        v7: Any
    ): Void

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
        jni_invoke9(id, v0, v1, v2, v3, v4, v5, v6, v7, v8)
    }

    private external fun jni_invoke9(
        id: Long,
        v0: Any,
        v1: Any,
        v2: Any,
        v3: Any,
        v4: Any,
        v5: Any,
        v6: Any,
        v7: Any,
        v8: Any
    ): Void

    // 增加计数，防止native层调用之后释放
    fun grab() {
        jni_grab(id)
    }

    private external fun jni_grab(id: Long): Void

    // 释放计数，如果计数=0则会释放native层的函数
    fun drop(): Boolean {
        return jni_drop(id)
    }

    private external fun jni_drop(id: Long): Boolean

    // 对应的计数函数
    var id: Long = 0
}