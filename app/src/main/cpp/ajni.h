#ifndef _AJNI_H
#define _AJNI_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <algorithm>
#include <functional>

#define AJNI_BEGIN namespace ajni {
#define AJNI_END }
#define AJNI_USE using namespace ajni;

#define AJNI_BEGIN_NS(ns) AJNI_BEGIN namespace ns {
#define AJNI_END_NS } AJNI_END

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

#define AJNI_STATIC_DECL(cls) static const cls& shared();
#define AJNI_STATIC_IMPL(cls) shared_ptr<cls> __gs_##cls; \
const cls& cls::shared() { \
    if (!__gs_##cls) \
        __gs_##cls = make_shared<cls>(); \
    return *__gs_##cls; }

#define __AJNI_COMBINE(L, R) L##R
#define _AJNI_COMBINE(L, R) __AJNI_COMBINE(L, R)

#define AJNI_CHECKEXCEPTION ExceptionGuard _AJNI_COMBINE(__exception_guard_, __LINE__);

#define AJNI_API(ret) extern "C" JNIEXPORT ret JNICALL
#define AJNI_FUNC(name) Java_com_wybosys_ajni_##name

AJNI_API(void) AJNI_FUNC(AJni_Init)(JNIEnv *env, jobject self);
AJNI_API(void) AJNI_FUNC(AJni_Main)(JNIEnv *env, jobject self);

AJNI_BEGIN

using namespace ::std;

// 获得全局env
JNIEnv *Env();

// 定义java类的路径，例如 com/google/gson/Gson
typedef string JClassPath;

// 定义java类的名称，例如 com.google.gson.Gson
typedef string JClassName;

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
    extern const string Void;
}

class JClass;

class JObject
{
public:

    // attach=true，代表实例化jobject时不增加应用计数
    JObject(jobject = nullptr, bool attach=true);
    JObject(JObject&);
    ~JObject();

    inline operator jobject () const {
        return _obj;
    }

    void reset(jobject= nullptr, bool attatch=true);

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
    JString(jstring, bool attach=true);
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
    JVariant(bool);
    JVariant(jchar);
    JVariant(jbyte);
    JVariant(jshort);
    JVariant(jint);
    JVariant(jlong);
    JVariant(jfloat);
    JVariant(jdouble);
    JVariant(jobject);
    JVariant(jstring);
    JVariant(const string&);

#define _JVARIANT_GET_IMPL(typ, obj) \
inline operator typ() { return obj; } \
inline operator const typ() const { return obj; }

    _JVARIANT_GET_IMPL(jobject, _v.l)
    _JVARIANT_GET_IMPL(jboolean, _v.z)
    _JVARIANT_GET_IMPL(jchar, _v.c)
    _JVARIANT_GET_IMPL(jbyte, _v.b)
    _JVARIANT_GET_IMPL(jshort, _v.s)
    _JVARIANT_GET_IMPL(jint, _v.i)
    _JVARIANT_GET_IMPL(jlong, _v.j)
    _JVARIANT_GET_IMPL(jfloat, _v.f)
    _JVARIANT_GET_IMPL(jdouble, _v.d)
    _JVARIANT_GET_IMPL(string, *_vs)
    _JVARIANT_GET_IMPL(jstring, *_vs)

    // 返回jni类型定义
    string jt() const;

    // 返回jni数据
    inline jvalue jv() const {
        return _v;
    }

private:
    TYPE _typ = UNKNOWN;
    jvalue _v;
    shared_ptr<JString> _vs;
    shared_ptr<JObject> _vo;
};

class JMethod {
    JMethod(JMethod&);

public:

    JMethod(JClass& cls): _cls(cls) {}

    string name; // 函数名
    string returntyp; // 返回类型
    bool is_static; // 是否是静态函数
    bool is_construct; // 是否是构造函数

    // 参数类型, 设置则代表不使用自动推导，手动指定入参表
    vector<string> argtyps;

    // 执行java函数
    JVariant operator ()() const;
    JVariant operator ()(const JVariant&) const;
    JVariant operator ()(const JVariant&, const JVariant&) const;
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&) const;
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&, const JVariant&) const;
    JVariant operator ()(const JVariant&, const JVariant&, const JVariant&, const JVariant&, const JVariant&) const;
    JVariant invoke(const vector<const JVariant*>&) const;

    // 成员函数
    JVariant operator ()(jobject) const;
    JVariant operator ()(jobject, const JVariant&) const;
    JVariant operator ()(jobject, const JVariant&, const JVariant&) const;
    JVariant operator ()(jobject, const JVariant&, const JVariant&, const JVariant&) const;
    JVariant operator ()(jobject, const JVariant&, const JVariant&, const JVariant&, const JVariant&) const;
    JVariant operator ()(jobject, const JVariant&, const JVariant&, const JVariant&, const JVariant&, const JVariant&) const;
    JVariant invoke(jobject, const vector<const JVariant*>&) const;

    // 生成函数标记
    string signature(const vector<const JVariant*>&, const vector<string>& = {}) const;

protected:

    JClass& _cls;
};

class JClass {
    AJNI_NOCOPY(JClass);

public:

    JClass(const JClassPath &path = "");
    virtual ~JClass();

    // jni类路径，java/lang/Class的格式
    inline const JClassPath& path() const {
        return _clazzpath;
    }

    // 类名，java.lang.Class的格式
    inline JClassName name() const {
        string t = _clazzpath;
        replace(t.begin(), t.end(), '/', '.');
        return t;
    }

    inline jclass clazz() const {
        return _clazz;
    }

    // 构造函数
    JMethod construct;

protected:

    JClassPath _clazzpath;
    jclass _clazz;
};

// 实例对象
template <typename Clazz = JClass>
class JEntry: public JObject {
    AJNI_NOCOPY(JEntry);

public:

    typedef shared_ptr<Clazz> clazz_type;
    typedef JEntry<Clazz> self_type;

    JEntry(clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct());
    }

    JEntry(const JVariant& v, clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct(v));
    }

    JEntry(const JVariant& v, const JVariant& v1, clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct(v, v1));
    }

    JEntry(const JVariant& v, const JVariant& v1, const JVariant& v2, clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct(v, v1, v2));
    }

    JEntry(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3, clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct(v, v1, v2, v3));
    }

    JEntry(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3, const JVariant& v4, clazz_type clz = make_shared<Clazz>(), bool ins=true)
    : JObject(nullptr, true), _clazz(clz) {
        if (ins)
            reset(clz->construct(v, v1, v2, v3, v4));
    }

    inline const Clazz* operator -> () const {
        return _clazz.get();
    }

    static shared_ptr<self_type> Attach(jobject obj) {
        auto r = make_shared<self_type>(make_shared<Clazz>(), false);
        r->reset(obj);
        return r;
    }

private:
    clazz_type _clazz;
};

class ExceptionGuard
{
public:
    ~ExceptionGuard();
};

AJNI_END

#endif