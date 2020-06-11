#include <ajni++/ajni++.hpp>

USE_AJNI

AJNI_API(void) AJNI_FUNC(Callback, jni_1grab)(JNIEnv *env, jobject thiz, jlong fnidx)
{
    Env.context().function_grab(fnidx);
}

AJNI_API(jboolean) AJNI_FUNC(Callback, jni_1drop)(JNIEnv *env, jobject thiz, jlong fnidx)
{
    return Env.context().function_drop(fnidx);
}

AJNI_API(void) AJNI_FUNC(Callback, jni_1invoke)(JNIEnv *env, jobject thiz, jlong fnidx)
{
    auto fn = Env.context().find_function(fnidx);
    if (!fn) {
        Logger::Error("没有找到回调函数");
        return;
    }

    (*fn)();
}

AJNI_API(void) AJNI_FUNC(Callback, jni_1invoke1)(JNIEnv *env, jobject thiz, jlong fnidx, jobject v0)
{
    auto fn = Env.context().find_function(fnidx);
    if (!fn) {
        Logger::Error("没有找到回调函数");
        return;
    }

    (*fn)();
}

AJNI_API(void) AJNI_FUNC(Callback, jni_1invoke2)(JNIEnv *env, jobject thiz, jlong fnidx, jobject v0, jobject v1)
{
    auto fn = Env.context().find_function(fnidx);
    if (!fn) {
        Logger::Error("没有找到回调函数");
        return;
    }

    (*fn)();
}
