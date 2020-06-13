#include "core.hpp"
#include "ajni++.hpp"
#include "variant.hpp"
#include "jre.hpp"
#include "com++.hpp"
#include <sstream>
#include <cmath>

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

extern shared_ptr<JVariant> ReadToVariant(jobject _obj);

JObject::JObject(jobject obj)
    : _obj(obj)
{
    // pass
}

JObject::~JObject()
{
    // pass
}

jobject JObject::asReturn() const
{
    return Env.NewLocalRef(_obj);
}

shared_ptr<JVariant> JObject::toVariant() const
{
    return ReadToVariant(_obj);
}

shared_ptr<JObject> JObject::make_shared(jobject obj)
{
    ::std::shared_ptr<JObject> r((JObject *)(new JObject(obj)));
    return r;
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

jstring JString::asReturn() const
{
    return Env.NewStringUTF(_str);
}

JArray::JArray(jarray arr)
{
    if (arr)
    {
        _arr = arr;
        _sz = Env.GetArrayLength(arr);
    }
}

JArray::~JArray()
{
}

string JArray::toString() const
{
    if (!_arr)
        return "";
    jbyte const *cs = Env.GetBytes((jbyteArray)_arr);
    return string((char const *)cs, _sz);
}

JValue::JValue(JValue const &r)
    : _val(r._val), _fnidx(r._fnidx)
{
    if (_fnidx)
    {
        Env.context().function_grab(_fnidx);
    }
}

JValue::JValue(JVariant const &var)
{
    JVariant::variant_type const &comvar = var;
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
    case VT::OBJECT:
    {
        _val.l = comvar.toObject();
    }
    break;
    case VT::STRING:
    {
        _val.l = Env.NewStringUTF(comvar.toString());
        _free = true;
    }
    break;
    case VT::NIL:
        break;
    case VT::FUNCTION:
    {
        auto cls = Env.context().register_class<jre::Callback>();
        JEntry<jre::Callback> cb(*cls->construct());
        // 将当前的函数保存到全局监听，执行结束后进行释放
        _fnidx = Env.context().add(var.toFunction());
        cb->id(cb, (jlong)_fnidx);
        _val.l = cb;
    }
    break;
    default:
        Logger::Error("ajnixx: 不支持类型转换 " + ::CROSS_NS::tostr((int)comvar.vt));
        break;
    }
}

JValue::~JValue()
{
    if (_free && _val.l)
    {
        Env.DeleteLocalRef(_val.l);
        _val.l = nullptr;
    }

    if (_fnidx)
    {
        Env.context().function_drop(_fnidx);
        _fnidx = 0;
    }
}

JValues::JValues(::std::initializer_list<args_type::value_type> const &vars)
{
    for (auto &e : vars)
    {
        auto t = make_shared<JValue>(*e);
        _vals.emplace_back(t);
        _jvals.emplace_back(*t);
    }
}

JValues::JValues(args_type const &vars)
{
    for (auto &e : vars)
    {
        auto t = make_shared<JValue>(*e);
        _vals.emplace_back(t);
        _jvals.emplace_back(*t);
    }
}

JVariant::JVariant()
    : vt(VT::NIL)
{
}

JVariant::JVariant(nullptr_t)
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

string const &JVariant::toString() const
{
    return _var.toString();
}

integer JVariant::toInteger() const
{
    switch (_var.vt)
    {
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
        return (integer)_var.toLonglong();
    case variant_type::VT::ULONGLONG:
        return (integer)_var.toULonglong();
    case variant_type::VT::CHAR:
        return _var.toChar();
    case variant_type::VT::UCHAR:
        return _var.toUChar();
    case variant_type::VT::BOOLEAN:
        return _var.toBool();
    case variant_type::VT::FLOAT:
        return (integer)round(_var.toFloat());
    case variant_type::VT::DOUBLE:
        return (integer)round(_var.toDouble());
    case variant_type::VT::STRING:
        return ::CROSS_NS::toint(_var.toString());
    default:
        break;
    }
    return 0;
}

number JVariant::toNumber() const
{
    switch (_var.vt)
    {
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

JTypeSignature JVariant::signature() const
{
    // 函数类型
    if (vt == VT::FUNCTION)
    {
        return jre::TypeSignature::CALLBACK;
    }

    // 普通pod数据类型
    switch (_var.vt)
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
    case variant_type::VT::NIL:
        return TypeSignature::VOID;
    default:
        break;
    }

    return TypeSignature::OBJECT;
}

AJNI_END
