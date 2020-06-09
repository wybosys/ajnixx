#ifndef __AJNI_VARIANT_H_INCLUDED
#define __AJNI_VARIANT_H_INCLUDED

#include "com++.hpp"
#include "../nlua++/com++.hpp"
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

        inline operator jobject() const {
            return _obj;
        }

        JObject &operator=(JObject const &);

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

    private:
        jvalue _val;
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
            typedef JObject object_type;
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

        JVariant(string const &);

        JVariant &operator=(JVariant const &);

        string const &toString() const;

        inline operator string const &() const {
            return toString();
        }

        inline operator variant_type const &() const {
            return _var;
        }

        static JTypeSignature FromVT(variant_type::VT);

        // 获得签名
        JTypeSignature signature() const;

    private:

        variant_type const _var;
    };

AJNI_END

#endif