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
#define AJNI_USE using namespace AJNI_NS;

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

#define AJNI_CHECKEXCEPTION ExceptionGuard _NNT_COMBINE(__exception_guard_, __LINE__)

#define AJNI_API(ret) extern "C" JNIEXPORT ret JNICALL
#define AJNI_FUNC(name) Java_com_nnt_ajnixx_##name

AJNI_BEGIN

using ::std::make_shared;
using ::std::shared_ptr;
using ::std::string;
using ::std::cout;
using ::std::cerr;
using ::std::endl;

// 定义全局空对象
extern const jobject jnull;

// 定义java类的路径，例如 com/google/gson/Gson
typedef string JClassPath;

// 定义java类的名称，例如 com.google.gson.Gson
typedef string JClassName;

// 定义签名类型
typedef string JTypeSignature;

class JValues;

class JEnv
{
public:

    // 设置vm对象，ajni++不使用JNI_OnLoad的形式获取vm避免干扰其他库的工作
    // \@env 已经存在的env，不传则即时获取一个
    void BindVM(JavaVM*, JNIEnv* env = nullptr);

    jclass FindClass(string const&);

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

    jmethodID GetMethodID(jclass, string const& name, string const& sig);
    jmethodID GetStaticMethodID(jclass, string const& name, string const& sig);

    jobject NewObject(jclass, jmethodID, JValues const&);

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
} // namespace TypeSignature

class ExceptionGuard
{
public:
    ~ExceptionGuard();
};

AJNI_END

#endif