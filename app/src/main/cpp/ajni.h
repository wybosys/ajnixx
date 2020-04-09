#ifndef _AJNI_H
#define _AJNI_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <map>
#include <exception>

#define AJNI_BEGIN namespace ajni {
#define AJNI_END }
#define AJNI_USE using namespace ajni;

#define _AJNI_LOG_IDR "ajni"
#define AJNI_LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,_AJNI_LOG_IDR,__VA_ARGS__)
#define AJNI_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,_AJNI_LOG_IDR,__VA_ARGS__)
#define AJNI_LOGW(...)  __android_log_print(ANDROID_LOG_WARN,_AJNI_LOG_IDR,__VA_ARGS__)
#define AJNI_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,_AJNI_LOG_IDR,__VA_ARGS__)
#define AJNI_LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,_AJNI_LOG_IDR,__VA_ARGS__)

#define AJNI_FRIEND(cls) friend class cls;
#define AJNI_NOCOPY(cls) private: cls(cls&);
#define AJNI_PRIVATECLASS(cls) cls##Private

#define AJNI_CLASS_PREPARE(cls) \
class cls; \
class AJNI_PRIVATECLASS(cls);

#define AJNI_CLASS_DECL(cls) \
protected: typedef AJNI_PRIVATECLASS(cls) private_class_type; \
friend class AJNI_PRIVATECLASS(cls); \
private_class_type *d_ptr = nullptr; \
friend class Magdle; \
private: AJNI_NOCOPY(cls);

#define AJNI_CLASS_CONSTRUCT(...) \
d_ptr = new private_class_type(__VA_ARGS__);
#define AJNI_CLASS_DESTORY() \
delete d_ptr;

#define AJNI_SINGLETON_DECL(cls) \
public: static cls& shared();
#define AJNI_SINGLETON_IMPL(cls) \
static cls* _shared = nullptr; \
cls& cls::shared() { \
if (_shared == nullptr) { \
_shared = new cls(); \
} \
return *_shared; \
}

#define __AJNI_COMBINE(L, R) L##R
#define _AJNI_COMBINE(L, R) __AJNI_COMBINE(L, R)

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

// java的类型
namespace jt {
    extern const string Class;
    extern const string String;
}

class JClass;

class JVariant {
public:
    typedef enum { UNKNOWN, OBJECT, BOOLEAN, BYTE, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, STRING } TYPE;

    JVariant(JVariant&r);
    JVariant(JClass&r);
    JVariant(bool);
    JVariant(jchar);
    JVariant(jbyte);
    JVariant(jshort);
    JVariant(jint);
    JVariant(jlong);
    JVariant(jfloat);
    JVariant(jdouble);
    JVariant(const string&);

#define _JVARIANT_GET_IMPL(typ, obj) \
inline operator typ() { return obj; } \
inline operator const typ() const { return obj; }

    _JVARIANT_GET_IMPL(jobject, _vobj)
    _JVARIANT_GET_IMPL(jboolean, _vbl)
    _JVARIANT_GET_IMPL(jchar, _vc)
    _JVARIANT_GET_IMPL(jbyte, _vb)
    _JVARIANT_GET_IMPL(jshort, _vs)
    _JVARIANT_GET_IMPL(jint, _vi)
    _JVARIANT_GET_IMPL(jlong, _vl)
    _JVARIANT_GET_IMPL(jfloat, _vf)
    _JVARIANT_GET_IMPL(jdouble, _vd)
    _JVARIANT_GET_IMPL(string, _vss)

private:
    TYPE _typ = UNKNOWN;
    jobject _vobj;
    jboolean _vbl;
    jchar _vc;
    jbyte _vb;
    jshort _vs;
    jint _vi;
    jlong _vl;
    jfloat _vf;
    jdouble _vd;
    string _vss;
};

class JMethod {
    JMethod(JMethod&);

public:

    JMethod(JClass& cls): _cls(cls) {}

    string name; // 函数名
    string returntyp; // 返回类型
    vector<string> argtyps; // 参数类型
    bool is_static; // 是否是静态函数

    // 执行java函数
    JVariant operator ()();
    JVariant operator ()(JVariant const&);

protected:

    JClass& _cls;

private:
    friend class JClass;
};

// 映射到Java的类
class JClass {
    JClass(JClass&);

public:

    typedef shared_ptr<JClass> instance_type;

    JClass(const JClassName &name);
    virtual ~JClass();

    // 实例化
    virtual instance_type instance() throw(exception);

protected:

    JClassName _clazzname;
    jclass _clazz;
    jobject _instance;

    friend class JInspect;
    friend jobject JClassGetInstance(JClass&);
};

AJNI_END

#endif