#include "core.hpp"
#include "ajni++.hpp"
#include "kotlin.hpp"

AJNI_BEGIN_NS(kotlin)

JClass::JClass(JClassPath const& cp)
: ::AJNI_NS::JClass(cp)
{
    {
        AJNI_CHECKEXCEPTION(false);
        _clazz$ = Env.FindClass(cp + "$Companion");
    }
}

jclass JClass::static_clazz() const
{
    return _clazz$ ? (jclass)(jobject)_clazz$ : (jclass)(jobject)_clazz;
}

JVariant JStaticMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto clz = _clazz.static_clazz();

    auto mid = Env.GetMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return JVariant();
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return Env.CallBooleanMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return Env.CallByteMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return Env.CallCharMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return Env.CallShortMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::INT)
    {
        return Env.CallIntMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return Env.CallLongMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return Env.CallFloatMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return Env.CallDoubleMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return (jstring)Env.CallObjectMethod(clz, mid, jvals);
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallVoidMethod(clz, mid, jvals);
    }
    else
    {
        return Env.CallObjectMethod(clz, mid, jvals);
    }

    return JVariant();
}

AJNI_END_NS
