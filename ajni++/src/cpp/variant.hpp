#ifndef __AJNI_VARIANT_H_INCLUDED
#define __AJNI_VARIANT_H_INCLUDED

#include "com++.hpp"
#include <vector>

AJNI_BEGIN

class JObject {
public:
    // 自动引用计数
    JObject(jobject = nullptr);

    // 复制，动引用计数
    JObject(JObject const &);

    // 释放引用计数
    ~JObject();

    inline operator jobject () const {
        return _obj;
    }

    JObject &operator=(JObject const &);

private:
    jobject _obj;
};

class JGlobalObject {
public:
    // 自动引用计数
    JGlobalObject(jobject = nullptr);

    // 复制，动引用计数
    JGlobalObject(JGlobalObject const &);

    // 释放引用计数
    ~JGlobalObject();

    inline operator jobject () const {
        return _obj;
    }

    JGlobalObject &operator=(JGlobalObject const &);

private:
    jobject _obj;
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
    bool _free = false;
};

class JValues {
public:

    JValues(::std::vector<JVariant> const &);

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

public:

    typedef ::com::Variant<JComVariantTypes> variant_type;

    enum struct VT {
        NIL,
        OBJECT,
        BOOLEAN,
        INTEGER,
        NUMBER,
        STRING
    };

    const VT vt;

    JVariant();

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

    inline JObject toObject() const {
        return _var.toObject();
    }

    inline operator JObject () const {
        return toObject();
    }

    static JTypeSignature FromVT(variant_type::VT);

    // 获得签名
    JTypeSignature signature() const;

private:

    variant_type const _var;
};

extern void grab(jobject);
extern bool drop(jobject);

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

#endif