#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

JVariant JStaticField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetStaticFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到静态变量 " + name + stype);
        return JVariant();
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return Env.GetStaticBooleanField(clz, fid);
    }
    else if (stype == TypeSignature::BYTE)
    {
        return Env.GetStaticByteField(clz, fid);
    }
    else if (stype == TypeSignature::CHAR)
    {
        return Env.GetStaticCharField(clz, fid);
    }
    else if (stype == TypeSignature::SHORT)
    {
        return Env.GetStaticShortField(clz, fid);
    }
    else if (stype == TypeSignature::INT)
    {
        return Env.GetStaticIntField(clz, fid);
    }
    else if (stype == TypeSignature::LONG)
    {
        return Env.GetStaticLongField(clz, fid);
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return Env.GetStaticFloatField(clz, fid);
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return Env.GetStaticDoubleField(clz, fid);
    }
    else if (stype == TypeSignature::STRING)
    {
        return (jstring)Env.GetStaticObjectField(clz, fid);
    }
    else
    {
        return Env.GetStaticObjectField(clz, fid);
    }

    return JVariant();
}

JVariant JMemberField::operator()(JObject& obj) const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到静态变量 " + name + stype);
        return JVariant();
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return Env.GetBooleanField(obj, fid);
    }
    else if (stype == TypeSignature::BYTE)
    {
        return Env.GetByteField(obj, fid);
    }
    else if (stype == TypeSignature::CHAR)
    {
        return Env.GetCharField(obj, fid);
    }
    else if (stype == TypeSignature::SHORT)
    {
        return Env.GetShortField(obj, fid);
    }
    else if (stype == TypeSignature::INT)
    {
        return Env.GetIntField(obj, fid);
    }
    else if (stype == TypeSignature::LONG)
    {
        return Env.GetLongField(obj, fid);
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return Env.GetFloatField(obj, fid);
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return Env.GetDoubleField(obj, fid);
    }
    else if (stype == TypeSignature::STRING)
    {
        return (jstring)Env.GetObjectField(obj, fid);
    }
    else
    {
        return Env.GetObjectField(obj, fid);
    }

    return JVariant();
}

JVariant JConstructMethod::operator()() const
{
    return invoke({});
}

JVariant JConstructMethod::operator()(JVariant const &v) const
{
    return invoke({v});
}

JVariant JConstructMethod::operator()(JVariant const &v, JVariant const &v1) const
{
    return invoke({v, v1});
}

JVariant JConstructMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke({v, v1, v2});
}

JVariant JConstructMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke({v, v1, v2, v3});
}

JVariant JConstructMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke({v, v1, v2, v3, v4});
}

JVariant JStaticMethod::operator()() const
{
    return invoke({});
}

JVariant JStaticMethod::operator()(JVariant const &v) const
{
    return invoke({v});
}

JVariant JStaticMethod::operator()(JVariant const &v, JVariant const &v1) const
{
    return invoke({v, v1});
}

JVariant JStaticMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke({v, v1, v2});
}

JVariant JStaticMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke({v, v1, v2, v3});
}

JVariant JStaticMethod::operator()(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke({v, v1, v2, v3, v4});
}

string JMethod::signature(args_type const &args, args_signatures_type const &predefs) const
{
    if (predefs.size())
    {
        ::std::vector<string> tss(predefs.begin(), predefs.end());
        string sig = "(" + CROSS_NS::implode(tss, "") + ")" + sreturn;
        return sig;
    }

    ::std::vector<string> ps;
    for (auto &e : args)
    {
        ps.emplace_back(e.signature());
    }

    string sig = "(" + CROSS_NS::implode(ps, "") + ")" + sreturn;
    return sig;
}

JVariant JMemberMethod::operator()(JObject& obj) const
{
    return invoke(obj, {});
}

JVariant JMemberMethod::operator()(JObject& obj, JVariant const &v) const
{
    return invoke(obj, {v});
}

JVariant JMemberMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1) const
{
    return invoke(obj, {v, v1});
}

JVariant JMemberMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke(obj, {v, v1, v2});
}

JVariant JMemberMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke(obj, {v, v1, v2, v3});
}

JVariant JMemberMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke(obj, {v, v1, v2, v3, v4});
}

JVariant JConstructMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();
    auto mid = Env.GetMethodID(clz, "<init>", sig);
    if (!mid)
    {
        Logger::Error("没有找到构造函数 " + name + " " + sig);
        return JVariant();
    }

    return Env.NewObject(clz, mid, jvals);
}

JVariant JStaticMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
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

JVariant JMemberMethod::invoke(JObject& obj, ::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();

    auto mid = Env.GetMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return JVariant();
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return Env.CallBooleanMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return Env.CallByteMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return Env.CallCharMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return Env.CallShortMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::INT)
    {
        return Env.CallIntMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return Env.CallLongMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return Env.CallFloatMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return Env.CallDoubleMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return (jstring)Env.CallObjectMethod(obj, mid, jvals);
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallVoidMethod(obj, mid, jvals);
    }
    else
    {
        return Env.CallObjectMethod(obj, mid, jvals);
    }

    return JVariant();
}

JClass::JClass(JClassPath const&path)
    : _clazzpath(path), construct(*this)
{
    if (!path.empty()) {
        AJNI_CHECKEXCEPTION;
        _clazz = Env.FindClass(path);
    }
}

JClassName JClass::name() const
{
    return _clazzpath;
}

bool JClass::exists() const {
    return _clazz != nullptr;
}

class JContextPrivate
{
public:

    typedef ::std::map<JClassPath, JContext::class_type> classes_type;
    classes_type classes;
};

JContext::JContext()
{
    NNT_CLASS_CONSTRUCT();
}

JContext::~JContext()
{
    NNT_CLASS_DESTORY();
}

bool JContext::add(class_type const& cls)
{
    if (!cls->exists())
        return false;
    d_ptr->classes[cls->path()] = cls;
    return true;
}

JContext::class_type JContext::find_class(JClassPath const& ph) const
{
    auto fnd = d_ptr->classes.find(ph);
    return fnd == d_ptr->classes.end() ? nullptr : fnd->second;
}

void JContext::clear()
{
    d_ptr->classes.clear();
}

AJNI_END
