#include "core.hpp"
#include "ajni++.hpp"
#include "kotlin.hpp"

AJNI_BEGIN_NS(kotlin)

string FirstUppcase(string const& str)
{
    return (char)toupper(str[0]) + str.substr(1);
}

JClass::JClass(JClassPath const& cp)
: ::AJNI_NS::JClass(cp)
{
    {
        AJNI_CHECKEXCEPTION(false);
        _classpath$ = cp + "$Companion";
        _clazz$ = Env.FindClass(_classpath$);
        if (_clazz$) {
            // 获取静态对象地址
            ::AJNI_NS::JStaticField sf(*this);
            sf.name = "Companion";
            sf.stype = "L" + _classpath$ + ";";
            _object$ = sf();
        }
    }
}

jclass JClass::clazz$() const
{
    return _clazz$ ? (jclass)(jobject)_clazz$ : (jclass)(jobject)_clazz;
}

JVariant JStaticMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto const& clazz = dynamic_cast<JClass const&>(_clazz);
    auto clz = clazz.clazz$();
    auto const& obj$ = clazz.object$();

    auto mid = Env.GetMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return JVariant();
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return Env.CallBooleanMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return Env.CallByteMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return Env.CallCharMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return Env.CallShortMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::INT)
    {
        return Env.CallIntMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return Env.CallLongMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return Env.CallFloatMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return Env.CallDoubleMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return (jstring)Env.CallObjectMethod(obj$, mid, jvals);
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallVoidMethod(obj$, mid, jvals);
    }
    else
    {
        return Env.CallObjectMethod(obj$, mid, jvals);
    }

    return JVariant();
}

JGlobalField::JGlobalField(JClassPath const& cp)
: _clazz(cp + "Kt")
{
}

JVariant JGlobalField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();
    auto getname = "get" + FirstUppcase(name);
    string sig = JMethod::Signature({}, stype, {});
    JValues jvals;

    auto mid = Env.GetStaticMethodID(clz, getname.c_str(), sig.c_str());
    if (!mid)
    {
        Logger::Error("没有找到全局变量 " + name + stype);
        return JVariant();
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return Env.CallStaticBooleanMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::BYTE)
    {
        return Env.CallStaticByteMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::CHAR)
    {
        return Env.CallStaticCharMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::SHORT)
    {
        return Env.CallStaticShortMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::INT)
    {
        return Env.CallStaticIntMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::LONG)
    {
        return Env.CallStaticLongMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return Env.CallStaticFloatMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return Env.CallStaticDoubleMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::STRING)
    {
        return (jstring)Env.CallStaticObjectMethod(clz, mid, jvals);
    }
    else if (stype == TypeSignature::VOID)
    {
        Env.CallStaticVoidMethod(clz, mid, jvals);
    }
    else
    {
        return Env.CallStaticObjectMethod(clz, mid, jvals);
    }

    return JVariant();
}

void JGlobalField::operator()(JVariant const& v)
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();
    auto setname = "set" + FirstUppcase(name);
    string sig = JMethod::Signature({v}, TypeSignature::VOID, {});
    JValues jvals({v});

    auto mid = Env.GetStaticMethodID(clz, setname.c_str(), sig.c_str());
    if (!mid)
    {
        Logger::Error("没有找到全局变量 " + name + stype);
        return;
    }

    Env.CallStaticVoidMethod(clz, mid, jvals);
}

JGlobalMethod::JGlobalMethod(JClassPath const& cp)
: _clazz(cp + "Kt")
{
}

JVariant JGlobalMethod::operator()() const
{
    return invoke({});
}

JVariant JGlobalMethod::operator()(JVariant const &v) const
{
    return invoke({v});
}

JVariant JGlobalMethod::operator()(JVariant const &v, JVariant const &v1) const
{
    return invoke({v, v1});
}

JVariant JGlobalMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke({v, v1, v2});
}

JVariant JGlobalMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke({v, v1, v2, v3});
}

JVariant JGlobalMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke({v, v1, v2, v3, v4});
}

JVariant JGlobalMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = JMethod::Signature(args, sreturn, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();

    auto mid = Env.GetStaticMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return JVariant();
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return Env.CallStaticBooleanMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return Env.CallStaticByteMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return Env.CallStaticCharMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return Env.CallStaticShortMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::INT)
    {
        return Env.CallStaticIntMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return Env.CallStaticLongMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return Env.CallStaticFloatMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return Env.CallStaticDoubleMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return (jstring)Env.CallStaticObjectMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallStaticVoidMethod(clz, mid, jvals);
    }
    else
    {
        return Env.CallStaticObjectMethod(clz, mid, jvals);
    }

    return JVariant();
}

AJNI_END_NS
