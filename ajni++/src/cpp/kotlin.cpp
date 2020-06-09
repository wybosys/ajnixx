#include "core.hpp"
#include "ajni++.hpp"
#include "kotlin.hpp"

AJNI_BEGIN_NS(kotlin)

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

AJNI_END_NS
