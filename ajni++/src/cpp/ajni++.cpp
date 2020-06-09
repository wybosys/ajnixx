#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

USE_CROSS;

JVariant JField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    if (is_static)
    {
        auto fid = Env.GetStaticFieldID(_cls, name.c_str(), typ.c_str());
        if (!fid)
        {
            cerr << "没有找到静态变量 " + name + typ << endl;
            return JVariant();
        }

        if (typ == TypeSignature::BOOLEAN)
        {
            return Env.GetStaticBooleanField(_cls, fid);
        }
        else if (typ == TypeSignature::BYTE)
        {
            return Env.GetStaticByteField(_cls, fid);
        }
        else if (typ == TypeSignature::CHAR)
        {
            return Env.GetStaticCharField(_cls, fid);
        }
        else if (typ == TypeSignature::SHORT)
        {
            return Env.GetStaticShortField(_cls, fid);
        }
        else if (typ == TypeSignature::INT)
        {
            return Env.GetStaticIntField(_cls, fid);
        }
        else if (typ == TypeSignature::LONG)
        {
            return Env.GetStaticLongField(_cls, fid);
        }
        else if (typ == TypeSignature::FLOAT)
        {
            return Env.GetStaticFloatField(_cls, fid);
        }
        else if (typ == TypeSignature::DOUBLE)
        {
            return Env.GetStaticDoubleField(_cls, fid);
        }
        else if (typ == TypeSignature::STRING)
        {
            return (jstring)Env.GetStaticObjectField(_cls, fid);
        }
        else
        {
            return Env.GetStaticObjectField(_cls, fid);
        }
    }

    return JVariant();
}

JVariant JField::operator()(jobject obj) const
{
    AJNI_CHECKEXCEPTION;

    if (!is_static)
    {
        auto fid = Env.GetFieldID(_cls, name.c_str(), typ.c_str());
        if (!fid)
        {
            cerr << "没有找到静态变量 " + name + typ << endl;
            return JVariant();
        }

        if (typ == TypeSignature::BOOLEAN)
        {
            return Env.GetBooleanField(obj, fid);
        }
        else if (typ == TypeSignature::BYTE)
        {
            return Env.GetByteField(obj, fid);
        }
        else if (typ == TypeSignature::CHAR)
        {
            return Env.GetCharField(obj, fid);
        }
        else if (typ == TypeSignature::SHORT)
        {
            return Env.GetShortField(obj, fid);
        }
        else if (typ == TypeSignature::INT)
        {
            return Env.GetIntField(obj, fid);
        }
        else if (typ == TypeSignature::LONG)
        {
            return Env.GetLongField(obj, fid);
        }
        else if (typ == TypeSignature::FLOAT)
        {
            return Env.GetFloatField(obj, fid);
        }
        else if (typ == TypeSignature::DOUBLE)
        {
            return Env.GetDoubleField(obj, fid);
        }
        else if (typ == TypeSignature::STRING)
        {
            return (jstring)Env.GetObjectField(obj, fid);
        }
        else
        {
            return Env.GetObjectField(obj, fid);
        }
    }

    return JVariant();
}

JVariant JMethod::operator()() const
{
    return invoke({});
}

JVariant JMethod::operator()(JVariant const &v) const
{
    return invoke({v});
}

JVariant JMethod::operator()(JVariant const &v, JVariant const &v1) const
{
    return invoke({v, v1});
}

JVariant JMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke({v, v1, v2});
}

JVariant JMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke({v, v1, v2, v3});
}

JVariant JMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke({v, v1, v2, v3, v4});
}

string JMethod::signature(::std::vector<JVariant> const &args, ::std::vector<JTypeSignature> const &predefs) const
{
    if (predefs.size())
    {
        string sig = "(" + implode(predefs, "") + ")" + returntype;
        return sig;
    }

    ::std::vector<string> ps;
    for (auto &e : args)
    {
        ps.emplace_back(e.signature());
    }

    string sig = "(" + implode(ps, "") + ")" + returntype;
    return sig;
}

JVariant JMethod::operator()(jobject obj) const
{
    return invoke(obj, {});
}

JVariant JMethod::operator()(jobject obj, JVariant const &v) const
{
    return invoke(obj, {v});
}

JVariant JMethod::operator()(jobject obj, JVariant const &v, JVariant const &v1) const
{
    return invoke(obj, {v, v1});
}

JVariant JMethod::operator()(jobject obj, JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke(obj, {v, v1, v2});
}

JVariant JMethod::operator()(jobject obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke(obj, {v, v1, v2, v3});
}

JVariant JMethod::operator()(jobject obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke(obj, {v, v1, v2, v3, v4});
}

JVariant JMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, argtypes);
    JValues jvals(args);

    if (is_construct)
    {
        auto mid = Env.GetMethodID(_cls, "<init>", sig);
        if (!mid)
        {
            cerr << "没有找到构造函数 " + name + sig << endl;
            return JVariant();
        }

        return Env.NewObject(_cls, mid, jvals);
    }

    if (is_static)
    {
        auto mid = Env.GetStaticMethodID(_cls, name, sig);
        if (!mid)
        {
            cerr << "没有找到函数 " + name + sig << endl;
            return JVariant();
        }

        if (returntype == TypeSignature::BOOLEAN)
        {
            return Env.CallStaticBooleanMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::BYTE)
        {
            return Env.CallStaticByteMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::CHAR)
        {
            return Env.CallStaticCharMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::SHORT)
        {
            return Env.CallStaticShortMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::INT)
        {
            return Env.CallStaticIntMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::LONG)
        {
            return Env.CallStaticLongMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::FLOAT)
        {
            return Env.CallStaticFloatMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::DOUBLE)
        {
            return Env.CallStaticDoubleMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::STRING)
        {
            return (jstring)Env.CallStaticObjectMethod(_cls, mid, jvals);
        }
        else if (returntype == TypeSignature::VOID)
        {
            Env.CallStaticVoidMethod(_cls, mid, jvals);
        }
        else
        {
            return Env.CallStaticObjectMethod(_cls, mid, jvals);
        }
    }
    return JVariant();
}

JVariant JMethod::invoke(jobject obj, ::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, argtypes);
    JValues jvals(args);

    if (!is_static)
    {
        auto mid = Env.GetMethodID(_cls, name, sig);
        if (!mid)
        {
            cerr << "没有找到函数 " + name + sig << endl;
            return JVariant();
        }

        if (returntype == TypeSignature::BOOLEAN)
        {
            return Env.CallBooleanMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::BYTE)
        {
            return Env.CallByteMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::CHAR)
        {
            return Env.CallCharMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::SHORT)
        {
            return Env.CallShortMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::INT)
        {
            return Env.CallIntMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::LONG)
        {
            return Env.CallLongMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::FLOAT)
        {
            return Env.CallFloatMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::DOUBLE)
        {
            return Env.CallDoubleMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::STRING)
        {
            return (jstring)Env.CallObjectMethod(obj, mid, jvals);
        }
        else if (returntype == TypeSignature::VOID)
        {
            Env.CallVoidMethod(obj, mid, jvals);
        }
        else
        {
            return Env.CallObjectMethod(obj, mid, jvals);
        }
    }
    return JVariant();
}

JClass::JClass(JClassPath const&path)
    : _clazzpath(path), construct(*this)
{
    if (!path.empty())
    {
        _clazz = Env.FindClass(path);
        if (_clazz)
        {
            _clazz = Env.NewGlobalRef(_clazz);
        }
    }

    construct.is_construct = true;
    construct.returntype = TypeSignature::VOID;
}

JClass::~JClass()
{
    if (_clazz)
    {
        Env.DeleteGlobalRef(_clazz);
        _clazz = nullptr;
    }
}

JClassName JClass::name() const
{
    return replace(_clazzpath, "/", ".");
}

bool JClass::exists() const {
    return _clazz != nullptr;
}

AJNI_END
