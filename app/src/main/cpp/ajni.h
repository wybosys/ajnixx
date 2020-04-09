#ifndef _AJNI_H
#define _AJNI_H

#include <jni.h>
#include <string>
#include <exception>

#define AJNI_BEGIN namespace ajni {
#define AJNI_END }

#define AJNI_API(ret) extern "C" JNIEXPORT ret JNICALL
#define AJNI_FUNC(name) Java_com_wybosys_ajni_##name

AJNI_API(void) AJNI_FUNC(AJni_Init)(JNIEnv *env, jobject self);
AJNI_API(void) AJNI_FUNC(AJni_Main)(JNIEnv *env, jobject self);

AJNI_BEGIN

using namespace ::std;

// 获得全局env
JNIEnv *Env();

// 定义java类的名称类型，例如 com/google/gson/Gson
typedef string JClassName;

// 映射到Java的类
class JClass {
    JClass(JClass&);

public:

    typedef shared_ptr<JClass> instance_type;

    JClass(const JClassName &name);
    virtual ~JClass();

    // 实例化
    virtual instance_type instance() throw(exception);

private:
    JClassName _clazzname;
    jclass _clazz;
    jobject _instance;
};

AJNI_END

#endif