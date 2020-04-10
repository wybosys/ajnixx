#ifndef _AJNI_H
#define _AJNI_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <algorithm>
#include <numeric>

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
typedef ::std::string JClassName;

// java的类型
namespace jt {
    extern const string Class;
    extern const string String;
    extern const string Object;
    extern const string Boolean;
    extern const string Byte;
    extern const string Char;
    extern const string Short;
    extern const string Int;
    extern const string Long;
    extern const string Float;
    extern const string Double;
}

class JClass;

class JObject
{
public:

    // attach=true，代表实例化jobject时不增加应用计数
    JObject(jobject = nullptr, bool attach=true);
    JObject(JObject&);
    ~JObject();

    inline operator jobject () {
        return _obj;
    }

private:
    jobject _obj;
};

template <typename T>
class _JObject {
    _JObject(_JObject&);

public:
    _JObject(): _obj(nullptr) {}

    inline operator T () {
        return _obj;
    }

protected:
    T _obj;
};

class JString: public _JObject<jstring>
{
public:
    JString(const string&);
    ~JString();

    inline operator const string& () const {
        return _str;
    }

private:
    string _str;
};

class JVariant {
public:
    typedef enum { UNKNOWN, OBJECT, BOOLEAN, BYTE, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, STRING } TYPE;

    JVariant() = default;
    JVariant(JClass&r);
    JVariant(bool);
    JVariant(jchar);
    JVariant(jbyte);
    JVariant(jshort);
    JVariant(jint);
    JVariant(jlong);
    JVariant(jfloat);
    JVariant(jdouble);
    JVariant(const ::std::string&);

#define _JVARIANT_GET_IMPL(typ, obj) \
inline operator typ() { return obj; } \
inline operator const typ() const { return obj; }

    _JVARIANT_GET_IMPL(jobject, _v.obj)
    _JVARIANT_GET_IMPL(jboolean, _v.bl)
    _JVARIANT_GET_IMPL(jchar, _v.c)
    _JVARIANT_GET_IMPL(jbyte, _v.b)
    _JVARIANT_GET_IMPL(jshort, _v.s)
    _JVARIANT_GET_IMPL(jint, _v.i)
    _JVARIANT_GET_IMPL(jlong, _v.l)
    _JVARIANT_GET_IMPL(jfloat, _v.f)
    _JVARIANT_GET_IMPL(jdouble, _v.d)
    _JVARIANT_GET_IMPL(::std::string, *_vss)

    // 返回jni类型定义
    string jt() const;

private:
    TYPE _typ = UNKNOWN;

    union {
        jobject obj;
        jboolean bl;
        jchar c;
        jbyte b;
        jshort s;
        jint i;
        jlong l;
        jfloat f;
        jdouble d;
    } _v;
    shared_ptr<JString> _vss;
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
    JVariant operator ()(const JVariant&);
    JVariant operator ()(const JVariant&, const JVariant&);
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&);
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&, const JVariant&);
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&, const JVariant&, const JVariant&);
    JVariant invoke(const vector<const JVariant*>&);

    // 生成函数标记
    string signature(const vector<const JVariant*>&) const;

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
    friend class JMethod;
    friend jobject JClassGetInstance(JClass&);
};

AJNI_END

#endif