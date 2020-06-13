#ifndef __AJNI_CORE_H_INCLUDED
#define __AJNI_CORE_H_INCLUDED

#include "macro.hpp"

#include <jni.h>
#include <android/log.h>

#ifndef AJNI_NS
#define AJNI_NS ajni
#endif

#define AJNI_BEGIN    \
    namespace AJNI_NS \
    {
#define AJNI_END }
#define USE_AJNI using namespace AJNI_NS;

#define AJNI_BEGIN_NS(ns)   \
    AJNI_BEGIN namespace ns \
    {
#define AJNI_END_NS \
    }               \
    AJNI_END

#define _AJNI_LOG_IDR "ajni"
#define AJNI_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGI(...) __android_log_print(ANDROID_LOG_INFO, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGW(...) __android_log_print(ANDROID_LOG_WARN, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGF(...) __android_log_print(ANDROID_LOG_FATAL, _AJNI_LOG_IDR, __VA_ARGS__)

// #define AJNI_CHECKEXCEPTION ::AJNI_NS::ExceptionGuard _NNT_COMBINE(__exception_guard_, __LINE__)

#define AJNI_API(ret) extern "C" JNIEXPORT ret JNICALL
#define AJNI_FUNC(cls, name) Java_com_nnt_ajnixx_##cls##_##name
#define AJNI_COMPANION_FUNC(cls, name) AJNI_FUNC(cls, 00024Companion_##name)

// 如果没有实现，需要再so里实现JNI的初始化函数
#define AJNI_IMP_LOADED(exp) \
AJNI_API(jint) JNI_OnLoad(JavaVM *vm, void *reserved) \
{ \
::AJNI_NS::Env.BindVM(vm); \
exp \
return JNI_VERSION_1_4; \
}

#define AJNI_IMP_UNLOADED(exp) \
AJNI_API(void) JNI_OnUnload(JavaVM* vm, void* reserved) \
{ \
        ::AJNI_NS::Env.UnbindVM(); \
        exp \
}

AJNI_BEGIN

using ::std::make_shared;
using ::std::shared_ptr;
using ::std::string;
using ::std::endl;

// 定义全局空对象
extern const jobject jnull;

#define _STRING_LIKE_IMPL(left, right) \
    using left::right; \
    using left::operator =; \
    using left::operator +=;

template <typename T>
class StringLike : public string
{
public:
    _STRING_LIKE_IMPL(string, string)

    StringLike(string const& r): string(r) {}
};

// 定义java类的路径，例如 com/google/gson/Gson
class JClassPath : public StringLike<JClassPath> {
public:
    _STRING_LIKE_IMPL(StringLike<JClassPath>, StringLike)
};

// 定义java类的名称，例如 com.google.gson.Gson
class JClassName : public StringLike<JClassName> {
public:
    _STRING_LIKE_IMPL(StringLike<JClassName>, StringLike)

    JClassName(JClassPath const&);
};

// 定义签名类型
class JTypeSignature : public StringLike<JTypeSignature> {
public:
    _STRING_LIKE_IMPL(StringLike<JTypeSignature>, StringLike)

    JTypeSignature(JClassPath const&);

    JTypeSignature& operator = (JClassPath const&);
};

class JValues;
class JContext;

NNT_CLASS_PREPARE(JEnv);

class JEnv
{
    NNT_CLASS_DECL(JEnv);

public:

    JEnv();
    ~JEnv();

    // 设置vm对象，ajni++不使用JNI_OnLoad的形式获取vm避免干扰其他库的工作
    // \@env 已经存在的env，不传则即时获取一个
    void BindVM(JavaVM*, JNIEnv* env = nullptr);

    // 取消注册
    void UnbindVM();

    // 检查线程安全行
    void Check();

    // 绑定当前上下文，用于在JNI环境中获得资源访问
    void BindContext(jobject act, jobject ctx);

    // 外部业务层提供的创建线程中JNIEnv的实现
    typedef ::std::function<JNIEnv*()> jnienv_retrieve_impl;
    jnienv_retrieve_impl GetCurrentJniEnv;

    // 获得上下文，之后类均从该对象获得
    JContext& context();

    // 跨线程安全获得classid，FindClass只能运行在主线程
    jclass SearchClass(string const&);

    jclass FindClass(string const&);
    bool IsAssignableFrom(jclass, jclass);
    bool IsInstanceOf(jobject, jclass);
    bool IsSameObject(jobject, jobject);

    jfieldID GetStaticFieldID(jclass, string const& name, string const& typ);
    jobject GetStaticObjectField(jclass, jfieldID);
    jboolean GetStaticBooleanField(jclass, jfieldID);
    jbyte GetStaticByteField(jclass, jfieldID);
    jchar GetStaticCharField(jclass, jfieldID);
    jshort GetStaticShortField(jclass, jfieldID);
    jint GetStaticIntField(jclass, jfieldID);
    jlong GetStaticLongField(jclass, jfieldID);
    jfloat GetStaticFloatField(jclass, jfieldID);
    jdouble GetStaticDoubleField(jclass, jfieldID);

    void SetStaticObjectField(jclass, jfieldID, jobject);
    void SetStaticBooleanField(jclass, jfieldID, jboolean);
    void SetStaticByteField(jclass, jfieldID, jbyte);
    void SetStaticCharField(jclass, jfieldID, jchar);
    void SetStaticShortField(jclass, jfieldID, jshort);
    void SetStaticIntField(jclass, jfieldID, jint);
    void SetStaticLongField(jclass, jfieldID, jlong);
    void SetStaticFloatField(jclass, jfieldID, jfloat);
    void SetStaticDoubleField(jclass, jfieldID, jdouble);

    jfieldID GetFieldID(jclass, string const& name, string const& sig);
    jobject GetObjectField(jobject, jfieldID);
    jboolean GetBooleanField(jobject, jfieldID);
    jbyte GetByteField(jobject, jfieldID);
    jchar GetCharField(jobject, jfieldID);
    jshort GetShortField(jobject, jfieldID);
    jint GetIntField(jobject, jfieldID);
    jlong GetLongField(jobject, jfieldID);
    jfloat GetFloatField(jobject, jfieldID);
    jdouble GetDoubleField(jobject, jfieldID);

    void SetObjectField(jobject, jfieldID, jobject);
    void SetBooleanField(jobject, jfieldID, jboolean);
    void SetByteField(jobject, jfieldID, jbyte);
    void SetCharField(jobject, jfieldID, jchar);
    void SetShortField(jobject, jfieldID, jshort);
    void SetIntField(jobject, jfieldID, jint);
    void SetLongField(jobject, jfieldID, jlong);
    void SetFloatField(jobject, jfieldID, jfloat);
    void SetDoubleField(jobject, jfieldID, jdouble);

    jmethodID GetMethodID(jclass, string const& name, string const& sig);
    jmethodID GetStaticMethodID(jclass, string const& name, string const& sig);

    jobject NewObject(jclass, jmethodID, JValues const&);
    jclass GetObjectClass(jobject);

    jboolean CallStaticBooleanMethod(jclass, jmethodID, JValues const&);
    jbyte CallStaticByteMethod(jclass, jmethodID, JValues const&);
    jchar CallStaticCharMethod(jclass, jmethodID, JValues const&);
    jshort CallStaticShortMethod(jclass, jmethodID, JValues const&);
    jint CallStaticIntMethod(jclass, jmethodID, JValues const&);
    jlong CallStaticLongMethod(jclass, jmethodID, JValues const&);
    jfloat CallStaticFloatMethod(jclass, jmethodID, JValues const&);
    jdouble CallStaticDoubleMethod(jclass, jmethodID, JValues const&);
    jobject CallStaticObjectMethod(jclass, jmethodID, JValues const&);
    void CallStaticVoidMethod(jclass, jmethodID, JValues const&);

    jboolean CallBooleanMethod(jobject, jmethodID, JValues const&);
    jbyte CallByteMethod(jobject, jmethodID, JValues const&);
    jchar CallCharMethod(jobject, jmethodID, JValues const&);
    jshort CallShortMethod(jobject, jmethodID, JValues const&);
    jint CallIntMethod(jobject, jmethodID, JValues const&);
    jlong CallLongMethod(jobject, jmethodID, JValues const&);
    jfloat CallFloatMethod(jobject, jmethodID, JValues const&);
    jdouble CallDoubleMethod(jobject, jmethodID, JValues const&);
    jobject CallObjectMethod(jobject, jmethodID, JValues const&);
    void CallVoidMethod(jobject, jmethodID, JValues const&);

    size_t GetArrayLength(jarray);
    jbyte const* GetBytes(jbyteArray);
    jchar const* GetChars(jcharArray);

    jobject NewLocalRef(jobject);
    void DeleteLocalRef(jobject);

    jobject NewGlobalRef(jobject);
    void DeleteGlobalRef(jobject);

    template <typename T>
    inline T NewLocalRef(T v) { return (T)NewLocalRef((jobject)v); }

    template <typename T>
    inline T NewGlobalRef(T v) { return (T)NewGlobalRef((jobject)v); }

    jsize GetStringUTFLength(jstring);
    string GetStringUTFChars(jstring);
    jstring NewStringUTF(string const&);

    void ExceptionClear();
};

// 获得全局env
extern JEnv Env;

// Jni对象签名
namespace TypeSignature
{
    extern const JTypeSignature CLASS;
    extern const JTypeSignature STRING;
    extern const JTypeSignature OBJECT;
    extern const JTypeSignature BOOLEAN;
    extern const JTypeSignature BYTE;
    extern const JTypeSignature CHAR;
    extern const JTypeSignature SHORT;
    extern const JTypeSignature INT;
    extern const JTypeSignature LONG;
    extern const JTypeSignature FLOAT;
    extern const JTypeSignature DOUBLE;
    extern const JTypeSignature VOID;
    extern const JTypeSignature BYTEARRAY;
} // namespace TypeSignature

class ExceptionGuard
{
public:

    ExceptionGuard(bool print = true) : _print(print) {}
    ~ExceptionGuard();

private:
    bool _print;
};

class Logger
{
public:

    static void Debug(string const&);
    static void Info(string const&);
    static void Warn(string const&);
    static void Error(string const&);
    static void Fatal(string const&);
};

AJNI_END

#endif