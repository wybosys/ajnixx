#include "core.hpp"
#include "ajni++.hpp"
#include "variant.hpp"
#include "jre.hpp"
#include <sstream>
#include <cmath>

#include <cross/cross.hpp>
#include <cross/str.hpp>

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

jobject JObject::asReturn() const
{
    return Env.NewLocalRef(_obj);
}

shared_ptr<JVariant> ReadToVariant(jobject _obj)
{
    auto& ctx = Env.context();
    auto obj = make_shared<JObject>(_obj);

    auto STD_NUMBER = ctx.register_class<jre::Number>();
    if (Env.IsInstanceOf(*obj, *STD_NUMBER)) {
        auto STD_DOUBLE = ctx.register_class<jre::Double>();
        if (Env.IsInstanceOf(*obj, *STD_DOUBLE)) {
            JEntry<jre::Double> ref(obj);
            return ref->doubleValue(ref);
        }

        auto STD_FLOAT = ctx.register_class<jre::Float>();
        if (Env.IsInstanceOf(*obj, *STD_FLOAT)) {
            JEntry<jre::Float> ref(obj);
            return ref->floatValue(ref);
        }

        JEntry<jre::Number> ref(obj);
        return ref->longValue(ref);
    }

    auto STD_STRING = ctx.register_class<jre::String>();
    if (Env.IsInstanceOf(*obj, *STD_STRING)) {
        JEntry<jre::String> ref(obj);
        return ref->getBytes(ref);
    }

    return _V(_obj);
}

shared_ptr<JVariant> JObject::toVariant() const
{
    return ReadToVariant(_obj);
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

jobject JGlobalObject::asReturn() const
{
    return Env.NewLocalRef(_obj);
}

shared_ptr<JVariant> JGlobalObject::toVariant() const
{
    return ReadToVariant(_obj);
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

JArray::JArray(jarray arr)
{
    if (arr) {
        _arr = Env.NewLocalRef(arr);
        _sz = Env.GetArrayLength(arr);
    }
}

JArray::~JArray()
{
    if (_arr) {
        Env.DeleteLocalRef(_arr);
    }
}

string JArray::toString() const
{
    if (!_arr)
        return "";
    jbyte const* cs = Env.GetBytes((jbyteArray)_arr);
    return string((char const*)cs, _sz);
}

JValue::JValue(JValue const& r)
: _val(r._val), _free(r._free), _fnidx(r._fnidx)
{
    if (_free && _val.l) {
        _val.l = Env.NewLocalRef(_val.l);
    }

    if (_fnidx) {
        Env.context().function_grab(_fnidx);
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
            jobject obj = comvar.toObject();
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
        case VT::FUNCTION: {
            auto cls = Env.context().register_class<jre::Callback>();
            JEntry<jre::Callback> cb(*cls->construct());
            // 将当前的函数保存到全局监听，执行结束后进行释放
            _fnidx = Env.context().add(var.toFunction());
            cb->id(cb, (jlong)_fnidx);
            _val.l = cb.asReturn();
            _free = true;
        } break;
        default:
            Logger::Error("ajnixx: 不支持类型转换 " + ::CROSS_NS::tostr((int)comvar.vt));
            break;
    }
}

JValue::~JValue()
{
    if (_free && _val.l) {
        Env.DeleteLocalRef(_val.l);
        _val.l = nullptr;
    }

    if (_fnidx) {
        Env.context().function_drop(_fnidx);
        _fnidx = 0;
    }
}

JValues::JValues(args_type const& vars)
{
    for (auto &e:vars) {
        auto t = make_shared<JValue>(*e);
        _vals.emplace_back(t);
        _jvals.emplace_back(*t);
    }
}

JVariant::JVariant()
    : vt(VT::NIL)
{
}

JVariant::JVariant(const char s[])
    : vt(VT::STRING), _var(s)
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

JVariant::JVariant(function_type::fun0_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun1_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun2_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun3_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun4_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun5_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun6_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun7_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun8_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

JVariant::JVariant(function_type::fun9_type fn)
        : vt(VT::FUNCTION), _fun(make_shared<function_type>(fn)), _var((variant_type::func_type)0)
{
}

string const& JVariant::toString() const {
    return _var.toString();
}

integer JVariant::toInteger() const
{
    switch (_var.vt) {
        case variant_type::VT::INT:
            return _var.toInt();
        case variant_type::VT::UINT:
            return _var.toUInt();
        case variant_type::VT::LONG:
            return _var.toLong();
        case variant_type::VT::ULONG:
            return _var.toULong();
        case variant_type::VT::SHORT:
            return _var.toShort();
        case variant_type::VT::USHORT:
            return _var.toUShort();
        case variant_type::VT::LONGLONG:
            return (integer) _var.toLonglong();
        case variant_type::VT::ULONGLONG:
            return (integer) _var.toULonglong();
        case variant_type::VT::CHAR:
            return _var.toChar();
        case variant_type::VT::UCHAR:
            return _var.toUChar();
        case variant_type::VT::BOOLEAN:
            return _var.toBool();
        case variant_type::VT::FLOAT:
            return (integer) round(_var.toFloat());
        case variant_type::VT::DOUBLE:
            return (integer) round(_var.toDouble());
        case variant_type::VT::STRING:
            return ::CROSS_NS::toint(_var.toString());
        default:
            break;
    }
    return 0;
}

number JVariant::toNumber() const
{
    switch (_var.vt) {
        case variant_type::VT::FLOAT:
            return _var.toFloat();
        case variant_type::VT::DOUBLE:
            return _var.toDouble();
        case variant_type::VT::STRING:
            return ::CROSS_NS::todouble(_var.toString());
        default:
            break;
    }
    return toInteger();
}

bool JVariant::toBool() const
{
    if (_var.vt == variant_type::VT::BOOLEAN)
        return _var.toBool();
    return toNumber() != 0;
}

JTypeSignature JVariant::signature() const {
    // 函数类型
    if (vt == VT::FUNCTION)
    {
        return jre::TypeSignature::CALLBACK;
    }

    // 普通pod数据类型
    switch (_var.vt) {
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
        case variant_type::VT::NIL:
            return TypeSignature::VOID;
        default:
            break;
    }

    return TypeSignature::OBJECT;
}

void grab(jobject obj)
{
    Env.NewLocalRef(obj);
}

bool drop(jobject obj)
{
    Env.DeleteLocalRef(obj);
    return false;
}

AJNI_END
