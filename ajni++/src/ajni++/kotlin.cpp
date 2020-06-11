#include "core.hpp"
#include "ajni++.hpp"
#include "kotlin.hpp"

AJNI_BEGIN_NS(kotlin)

string capitalize(string const& str)
{
    return (char)toupper(str[0]) + str.substr(1);
}

JClass::JClass(JClassPath const& cp)
: ::AJNI_NS::JClass(cp)
{
    {
        AJNI_CHECKEXCEPTION(false);
        _classpath$ = cp + "$Companion";

        jclass jclz = Env.FindClass(_classpath$);
        if (jclz) {
            _clazz$ = make_shared<JObject>(jclz);

            // 获取静态对象地址
            ::AJNI_NS::JStaticField sf(*this);
            sf.name = "Companion";
            sf.stype = "L" + _classpath$ + ";";
            _object$ = *sf();
        }
    }
}

jclass JClass::clazz$() const
{
    return _clazz$ ? (jclass)(jobject)*_clazz$ : (jclass)(jobject)_clazz;
}

return_type JStaticMethod::invoke(args_type const &args) const
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
        return nullptr;
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return _V(Env.CallBooleanMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return _V(Env.CallByteMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return _V(Env.CallCharMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return _V(Env.CallShortMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::INT)
    {
        return _V(Env.CallIntMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return _V(Env.CallLongMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return _V(Env.CallFloatMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return _V(Env.CallDoubleMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return _V((jstring)Env.CallObjectMethod(obj$, mid, jvals));
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallVoidMethod(obj$, mid, jvals);
        return nullptr;
    }

    return _V(Env.CallObjectMethod(obj$, mid, jvals));
}

JGlobalField::JGlobalField(JClassPath const& cp)
: _clazz(cp + "Kt")
{
}

return_type JGlobalField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();
    auto getname = "get" + capitalize(name);
    string sig = JMethod::Signature({}, stype, {});
    JValues jvals;

    auto mid = Env.GetStaticMethodID(clz, getname.c_str(), sig.c_str());
    if (!mid)
    {
        Logger::Error("没有找到全局变量 " + name + stype);
        return nullptr;
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return _V(Env.CallStaticBooleanMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::BYTE)
    {
        return _V(Env.CallStaticByteMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::CHAR)
    {
        return _V(Env.CallStaticCharMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::SHORT)
    {
        return _V(Env.CallStaticShortMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::INT)
    {
        return _V(Env.CallStaticIntMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::LONG)
    {
        return _V(Env.CallStaticLongMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return _V(Env.CallStaticFloatMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return _V(Env.CallStaticDoubleMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::STRING)
    {
        return _V((jstring)Env.CallStaticObjectMethod(clz, mid, jvals));
    }
    else if (stype == TypeSignature::VOID)
    {
        Env.CallStaticVoidMethod(clz, mid, jvals);
        return nullptr;
    }

    return _V(Env.CallStaticObjectMethod(clz, mid, jvals));
}

void JGlobalField::operator()(JVariant const& v)
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();
    auto setname = "set" + capitalize(name);
    string sig = JMethod::Signature({&v}, TypeSignature::VOID, {});
    JValues jvals({&v});

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

return_type JGlobalMethod::operator()() const
{
    return invoke({});
}

return_type JGlobalMethod::operator()(arg_type v) const
{
    return invoke({&v});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1) const
{
    return invoke({&v, &v1});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2) const
{
    return invoke({&v, &v1, &v2});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3) const
{
    return invoke({&v, &v1, &v2, &v3});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4) const
{
    return invoke({&v, &v1, &v2, &v3, &v4});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7});
}

return_type JGlobalMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7, arg_type v8) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8});
}

return_type JGlobalMethod::invoke(args_type const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = JMethod::Signature(args, sreturn, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();

    auto mid = Env.GetStaticMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return nullptr;
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return _V(Env.CallStaticBooleanMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return _V(Env.CallStaticByteMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return _V(Env.CallStaticCharMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return _V(Env.CallStaticShortMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::INT)
    {
        return _V(Env.CallStaticIntMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return _V(Env.CallStaticLongMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return _V(Env.CallStaticFloatMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return _V(Env.CallStaticDoubleMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return _V((jstring)Env.CallStaticObjectMethod(clz, mid, jvals));
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallStaticVoidMethod(clz, mid, jvals);
        return nullptr;
    }

    return _V(Env.CallStaticObjectMethod(clz, mid, jvals));
}

AJNI_END_NS
