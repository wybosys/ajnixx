#include "core.hpp"
#include "variant.hpp"
#include <sstream>

AJNI_BEGIN

JObject::JObject(jobject obj)
    : _obj(obj)
{
    if (_obj)
        _obj = Env.NewLocalRef(_obj);
}

JObject::JObject(JObject const &r)
    : _obj(r._obj)
{
    if (_obj)
        _obj = Env.NewLocalRef(_obj);
}

JObject::~JObject()
{
    if (_obj)
    {
        Env.DeleteLocalRef(_obj);
        _obj = nullptr;
    }
}

JObject &JObject::operator=(JObject const &r)
{
    if (_obj == r._obj)
        return *this;

    if (_obj)
        Env.DeleteLocalRef(_obj);

    _obj = r._obj;
    if (_obj)
    {
        _obj = Env.NewLocalRef(_obj);
    }

    return *this;
}

JGlobalObject::JGlobalObject(jobject obj)
    : _obj(obj)
{
    if (_obj)
        _obj = Env.NewGlobalRef(_obj);
}

JGlobalObject::JGlobalObject(JGlobalObject const &r)
    : _obj(r._obj)
{
    if (_obj)
        _obj = Env.NewGlobalRef(_obj);
}

JGlobalObject::~JGlobalObject()
{
    if (_obj)
    {
        Env.DeleteGlobalRef(_obj);
        _obj = nullptr;
    }
}

JGlobalObject &JGlobalObject::operator=(JGlobalObject const &r) {
    if (_obj == r._obj)
        return *this;

    if (_obj)
        Env.DeleteGlobalRef(_obj);

    _obj = r._obj;
    if (_obj) {
        _obj = Env.NewGlobalRef(_obj);
    }

    return *this;
}

JString::JString(jstring v)
{
    _str = Env.GetStringUTFChars(v);
}

JString::JString(JString const &r)
    : _str(r._str)
{
    // pass
}

JString::JString(string const &str)
    : _str(str)
{
    // pass
}

JString::~JString()
{
    // pass
}

jstring JString::asReturn() const {
    return Env.NewStringUTF(_str);
}

JValue::JValue(JValue const& r)
: _val(r._val), _free(r._free)
{
    if (_free && _val.l) {
        _val.l = Env.NewLocalRef(_val.l);
    }
}

JValue::JValue(JVariant const& var)
{
    JVariant::variant_type const& comvar = var;
    typedef JVariant::variant_type::VT VT;
    switch (comvar.vt)
    {
        case VT::INT:
        case VT::UINT:
            _val.i = comvar.toInt();
            break;
        case VT::FLOAT:
            _val.f = comvar.toFloat();
            break;
        case VT::DOUBLE:
            _val.d = comvar.toDouble();
            break;
        case VT::SHORT:
        case VT::USHORT:
            _val.s = comvar.toShort();
            break;
        case VT::LONG:
        case VT::ULONG:
            _val.j = comvar.toLong();
            break;
        case VT::LONGLONG:
        case VT::ULONGLONG:
            _val.j = (jlong)comvar.toLonglong();
            break;
        case VT::CHAR:
        case VT::UCHAR:
            _val.c = comvar.toChar();
            break;
        case VT::BOOLEAN:
            _val.z = comvar.toBool();
            break;
        case VT::OBJECT: {
            jobject obj = *comvar.toObject();
            if (obj) {
                obj = Env.NewLocalRef(obj);
            }
            _val.l = obj;
            _free = true;
        } break;
        case VT::STRING: {
            _val.l = Env.NewStringUTF(comvar.toString());
            _free = true;
        } break;
        case VT::NIL:
            break;
        case VT::BYTES:
        case VT::FUNCTION:
        case VT::POINTER:
            AJNI_LOGE("ajnixx: 不支持类型转换");
            break;
    }
}

JValue::~JValue()
{
    if (_free && _val.l) {
        Env.DeleteLocalRef(_val.l);
        _val.l = nullptr;
    }
}

JValues::JValues(::std::vector<JVariant> const& vars)
{
    for (auto &e:vars) {
        auto t = make_shared<JValue>(e);
        _vals.emplace_back(t);
        _jvals.emplace_back(*t);
    }
}

JVariant::JVariant()
    : vt(VT::NIL)
{
}

JVariant::JVariant(bool v)
    : vt(VT::BOOLEAN), _var(v)
{
}

JVariant::JVariant(jchar v)
    : vt(VT::INTEGER), _var(v)
{
}

JVariant::JVariant(jbyte v)
    : vt(VT::INTEGER), _var(v)
{
}

JVariant::JVariant(jshort v)
    : vt(VT::INTEGER), _var(v)
{
}

JVariant::JVariant(jint v)
    : vt(VT::INTEGER), _var(v)
{
}

JVariant::JVariant(jlong v)
    : vt(VT::INTEGER), _var(v)
{
}

JVariant::JVariant(jfloat v)
    : vt(VT::NUMBER), _var(v)
{
}

JVariant::JVariant(jdouble v)
    : vt(VT::NUMBER), _var(v)
{
}

JVariant::JVariant(jobject v)
    : vt(VT::OBJECT), _var(v)
{
}

JVariant::JVariant(jstring v)
    : vt(VT::STRING), _var(JString(v))
{
}

JVariant::JVariant(string const &v)
    : vt(VT::STRING), _var(JString(v))
{
}

string const& JVariant::toString() const {
    return _var.toString();
}

JTypeSignature JVariant::FromVT(variant_type::VT vt)
{
    switch (vt)
    {
        case variant_type::VT::BOOLEAN:
            return TypeSignature::BOOLEAN;
        case variant_type::VT::INT:
        case variant_type::VT::UINT:
            return TypeSignature::INT;
        case variant_type::VT::SHORT:
        case variant_type::VT::USHORT:
            return TypeSignature::SHORT;
        case variant_type::VT::LONG:
        case variant_type::VT::ULONG:
        case variant_type::VT::LONGLONG:
        case variant_type::VT::ULONGLONG:
            return TypeSignature::LONG;
        case variant_type::VT::FLOAT:
            return TypeSignature::FLOAT;
        case variant_type::VT::DOUBLE:
            return TypeSignature::DOUBLE;
        case variant_type::VT::CHAR:
        case variant_type::VT::UCHAR:
            return TypeSignature::CHAR;
        case variant_type::VT::STRING:
            return TypeSignature::STRING;
        case variant_type::VT::OBJECT:
            return TypeSignature::OBJECT;
        case variant_type::VT::NIL:
            return TypeSignature::VOID;
        case variant_type::VT::FUNCTION:
        case variant_type::VT::POINTER:
        case variant_type::VT::BYTES:
            return TypeSignature::OBJECT;
    }
}

JTypeSignature JVariant::signature() const {
    return FromVT(_var.vt);
}

AJNI_END
