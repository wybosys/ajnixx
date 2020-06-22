#ifndef __AJNI_VARIANT_H_INCLUDED
#define __AJNI_VARIANT_H_INCLUDED

#include "com++.hpp"
#include "ast.hpp"
#include <vector>
#include <functional>
#include <atomic>

AJNI_BEGIN

namespace kotlin
{
class JClass;
}

class JVariant;

class JArray;

typedef ::std::vector<JVariant const *> args_type;

class JObject
{
public:

    JObject();

    JObject(JObject const &);

    // 释放引用计数
    virtual ~JObject();

    // 作为程序返回值输出
    virtual jobject asReturn() const;

    // 转换成具体的Variant类型，和Variant(JObject)不同，转换会读取具体对象内部信息，返回C++数据构成的Variant对象
    static shared_ptr <JVariant> Extract(jobject);

    // 是否时空对象
    bool isnil() const;

    JObject &operator=(JObject const &);

protected:

    // 只允许特殊情况使用
    class _JGlobalObject
    {
    public:

        _JGlobalObject(JObject const &);

        void grab();

        bool drop();

        shared_ptr <JObject> gobj; // 全局对象

    private:
        ::std::atomic<int> _refs; // 引用计数
    };

    // 自动引用计数
    explicit JObject(jobject, bool local = true);

    void _reset(jobject, bool local = true);

    jobject _obj = nullptr;
    bool _local = true;

    // 提权到global
    virtual void _asglobal();

    friend class JEnv;

    friend class JEnvPrivate;

    friend class JClass;

    friend class JArray;

    friend class JVariant;

    friend class kotlin::JClass;

    template<typename T> friend
    class JEntry;
};

class JString
{
public:

    JString();

    JString(string const &);

    JString(JString const &);

    ~JString();

    inline operator const string &() const
    {
        return _str;
    }

    // 作为程序返回值输出
    jstring asReturn() const;

protected:

    void _reset(jstring);

    string _str;

    friend class JEnv;

    friend class JVariant;
};

class JArray
{
public:

    JArray();

    ~JArray();

    inline size_t size() const
    {
        return _sz;
    }

    string toString() const;

protected:

    void _reset(jarray, size_t);

private:

    JObject _arr;
    size_t _sz;

    friend class JEnv;
};

class JVariant;

class JValue
{
public:

    JValue(JVariant const &);

    JValue(JValue const &);

    ~JValue();

    inline operator jvalue() const
    {
        return _val;
    }

private:

    jvalue _val = {0};
    bool _free = false;
    size_t _fnidx = 0; // 如果是函数对象，保存函数的本地索引
};

class JValues
{
public:

    JValues() = default;

    JValues(::std::initializer_list<args_type::value_type> const &);

    JValues(args_type const &);

    typedef shared_ptr <JValue> value_type;

    inline size_t size() const
    {
        return _vals.size();
    }

private:

    ::std::vector<value_type> _vals;
    ::std::vector<jvalue> _jvals;

    // 返回jni函数需要的参数列表
    inline jvalue const *_args() const
    {
        return &_jvals[0];
    }

    friend class JEnv;
};

typedef ptrdiff_t integer;
typedef double number;

class JCallback;

class JVariant
{
private:

    class JComVariantTypes : public ::com::VariantTypes<>
    {
    public:
        typedef _jobject object_type;
        typedef JString string_type;
    };

public:

    typedef ::COMXX_NS::Variant<JComVariantTypes> variant_type;

    enum struct VT
    {
        NIL,
        OBJECT,
        BOOLEAN,
        INTEGER,
        NUMBER,
        STRING,
        CALLBACK
    };

    const VT vt;

    JVariant();

    JVariant(nullptr_t);

    JVariant(const char s[]);

    JVariant(jstring);

    JVariant(string const &);

    JVariant(bool);

    JVariant(jchar);

    JVariant(jbyte);

    JVariant(jshort);

    JVariant(jint);

    JVariant(jlong);

    JVariant(jfloat);

    JVariant(jdouble);

    JVariant(jobject);

    JVariant(JCallback const &);

    string const &toString() const;

    integer toInteger() const;

    number toNumber() const;

    bool toBool() const;

    inline operator string const &() const
    {
        return toString();
    }

    inline operator variant_type const &() const
    {
        return _var;
    }

    inline shared_ptr <JCallback> toCallback() const
    {
        return _callback;
    }

    shared_ptr <JObject> toObject() const;

    static shared_ptr <JVariant> FromObject(JObject const &);

    inline bool isnil() const
    {
        return vt == VT::NIL;
    }

    // 获得签名
    JTypeSignature signature() const;

private:

    variant_type _var;
    shared_ptr <JCallback> _callback;

};

class JCallback
{
    class JComFunctionTypes : public ::COMXX_NS::FunctionTypes<
            JVariant,
            shared_ptr < JVariant>,
                              JVariant

    > {};

public:

    typedef ::COMXX_NS::Function<JComFunctionTypes> function_type;
    typedef function_type::return_type return_type;
    typedef function_type::arg_type arg_type;

    JCallback(function_type::fun0_type);

    JCallback(function_type::fun1_type);

    JCallback(function_type::fun2_type);

    JCallback(function_type::fun3_type);

    JCallback(function_type::fun4_type);

    JCallback(function_type::fun5_type);

    JCallback(function_type::fun6_type);

    JCallback(function_type::fun7_type);

    JCallback(function_type::fun8_type);

    JCallback(function_type::fun9_type);

    void operator()() const;

    void operator()(arg_type const &) const;

    void operator()(arg_type const &, arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &) const;

    void
    operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &, arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &, arg_type const &, arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &) const;

    void operator()(arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &, arg_type const &, arg_type const &, arg_type const &,
                    arg_type const &) const;

    // 通过在业务中修改此处，可以达到让所有callback统一以固定模式被调用，默认为false
    static bool ASYNC;

    // 异步调用
    bool async = ASYNC;

protected:

    shared_ptr <function_type> _fn;
};

template<typename T>
inline shared_ptr <JVariant> _V(T const &v)
{
    return make_shared<JVariant>(v);
}

template<typename _CharT, typename _Traits>
static ::std::basic_ostream<_CharT, _Traits> &
operator<<(::std::basic_ostream<_CharT, _Traits> &stm, JVariant const &v)
{
    switch (v.vt) {
        default:
            break;
        case JVariant::VT::STRING:
            stm << v.toString();
            break;
        case JVariant::VT::INTEGER:
            stm << v.toInteger();
            break;
        case JVariant::VT::NUMBER:
            stm << v.toNumber();
            break;
        case JVariant::VT::BOOLEAN:
            stm << v.toBool();
            break;
        case JVariant::VT::OBJECT:
            stm << v.toObject();
            break;
    }
    return stm;
}

template<typename _CharT, typename _Traits>
static ::std::basic_ostream<_CharT, _Traits> &
operator<<(::std::basic_ostream<_CharT, _Traits> &stm, shared_ptr <JVariant> const &v)
{
    if (!v)
        return stm;
    return stm << *v;
}

AJNI_END

COMXX_BEGIN

template<>
inline jobject grab<jobject>(jobject obj)
{
    return ::AJNI_NS::Env.NewLocalRef(obj);
}

template<>
inline bool drop<jobject>(jobject obj)
{
    ::AJNI_NS::Env.DeleteLocalRef(obj);
    return true;
}

COMXX_END

#endif
