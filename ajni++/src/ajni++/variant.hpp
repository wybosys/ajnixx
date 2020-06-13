#ifndef __AJNI_VARIANT_H_INCLUDED
#define __AJNI_VARIANT_H_INCLUDED

#include "com++.hpp"
#include "ajni++.hpp"
#include <vector>
#include <functional>

AJNI_BEGIN

class JVariant;
typedef ::std::vector<JVariant const*> args_type;

class JWeakObject {
public:

    JWeakObject(jobject obj) : _obj(obj) {}
    virtual ~JWeakObject() {}

    inline operator jobject () const {
        return _obj;
    }

    virtual jobject asReturn() const {
        return _obj;
    }

protected:

    jobject _obj;
};

class JObject : public JWeakObject {
public:

    // 自动引用计数
    JObject(jobject = nullptr);

    // 复制，动引用计数
    JObject(JObject const &);

    // 释放引用计数
    virtual ~JObject();

    JObject &operator = (jobject);

    // 作为程序返回值输出
    virtual jobject asReturn() const;

    // 转换成具体的Variant类型，和Variant(JObject)不同，转换会读取具体对象内部信息，返回C++数据构成的Variant对象
    shared_ptr<JVariant> toVariant() const;

    static shared_ptr<JWeakObject> make_shared(jobject);
};

class JString {
public:
    JString(jstring = nullptr);

    JString(JString const &);

    JString(string const &);

    ~JString();

    inline operator const string &() const {
        return _str;
    }

    // 作为程序返回值输出
    jstring asReturn() const;

private:
    string _str;
};

class JArray
{
public:

    JArray(jarray);
    ~JArray();

    inline size_t size() const {
        return _sz;
    }

    string toString() const;

private:
    jarray _arr = nullptr;
    size_t _sz;
};

class JVariant;

class JValue {
public:

    JValue(JVariant const &);

    JValue(JValue const &);

    ~JValue();

    inline operator jvalue() const {
        return _val;
    }

    inline operator jobject() const {
        return _val.l;
    }

private:
    jvalue _val = {0};
    bool _free = false; // 是否需要释放
    bool _local = true; // 是否是局部对象
    size_t _fnidx = 0; // 如果是函数对象，保存函数的本地索引
};

class JValues {
public:

    JValues() = default;
    JValues(::std::initializer_list<args_type::value_type> const&);
    JValues(args_type const &);

    typedef shared_ptr <JValue> value_type;

    inline size_t size() const {
        return _vals.size();
    }

    inline jvalue operator[](size_t idx) const {
        return _jvals[idx];
    }

private:
    ::std::vector<value_type> _vals;
    ::std::vector<jvalue> _jvals;
};

typedef ptrdiff_t integer;
typedef double number;

class JVariant {
private:

    class JComVariantTypes : public ::com::VariantTypes<> {
    public:
        typedef _jobject object_type;
        typedef JString string_type;
    };

    class JComFunctionTypes : public ::COMXX_NS::FunctionTypes<
            JVariant,
            shared_ptr<JVariant>,
            JVariant const&
            > {};

public:

    typedef ::COMXX_NS::Variant<JComVariantTypes> variant_type;
    typedef ::COMXX_NS::Function<JComFunctionTypes> function_type;

    enum struct VT {
        NIL,
        OBJECT,
        BOOLEAN,
        INTEGER,
        NUMBER,
        STRING,
        FUNCTION
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

    JVariant(function_type::fun0_type);
    JVariant(function_type::fun1_type);
    JVariant(function_type::fun2_type);
    JVariant(function_type::fun3_type);
    JVariant(function_type::fun4_type);
    JVariant(function_type::fun5_type);
    JVariant(function_type::fun6_type);
    JVariant(function_type::fun7_type);
    JVariant(function_type::fun8_type);
    JVariant(function_type::fun9_type);

    string const &toString() const;

    integer toInteger() const;

    number toNumber() const;

    bool toBool() const;

    inline operator string const &() const {
        return toString();
    }

    inline operator variant_type const &() const {
        return _var;
    }

    inline shared_ptr<JWeakObject> toObject() const {
        return make_shared<JWeakObject>(_var.toObject());
    }

    inline operator shared_ptr<JWeakObject> () const {
        return make_shared<JWeakObject>(_var.toObject());
    }

    inline shared_ptr<function_type> toFunction() const {
        return _fun;
    }

    inline bool isnil() const {
        return vt == VT::NIL;
    }

    // 获得签名
    JTypeSignature signature() const;

private:

    variant_type _var;
    shared_ptr<function_type> _fun;

};

template <typename T>
inline shared_ptr<JVariant> _V(T const& v) {
    return make_shared<JVariant>(v);
}

template<typename _CharT, typename _Traits>
static ::std::basic_ostream <_CharT, _Traits> &operator<<(::std::basic_ostream <_CharT, _Traits> &stm, JVariant const &v) {
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
static ::std::basic_ostream <_CharT, _Traits> &operator<<(::std::basic_ostream <_CharT, _Traits> &stm, shared_ptr <JVariant> const &v) {
    if (!v)
        return stm;
    return stm << *v;
}

AJNI_END

COMXX_BEGIN

template <>
inline jobject grab<jobject>(jobject obj) {
    return AJNI_NS::Env.NewLocalRef(obj);
}

template <>
inline bool drop<jobject>(jobject obj) {
    AJNI_NS::Env.DeleteLocalRef(obj);
    return true;
}

COMXX_END

#endif