#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

JVariant JField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    if (is_static)
    {
        auto fid = Env.GetStaticFieldID(_clazz, name.c_str(), typ.c_str());
        if (!fid)
        {
            Logger::Error("没有找到静态变量 " + name + typ);
            return JVariant();
        }

        if (typ == TypeSignature::BOOLEAN)
        {
            return Env.GetStaticBooleanField(_clazz, fid);
        }
        else if (typ == TypeSignature::BYTE)
        {
            return Env.GetStaticByteField(_clazz, fid);
        }
        else if (typ == TypeSignature::CHAR)
        {
            return Env.GetStaticCharField(_clazz, fid);
        }
        else if (typ == TypeSignature::SHORT)
        {
            return Env.GetStaticShortField(_clazz, fid);
        }
        else if (typ == TypeSignature::INT)
        {
            return Env.GetStaticIntField(_clazz, fid);
        }
        else if (typ == TypeSignature::LONG)
        {
            return Env.GetStaticLongField(_clazz, fid);
        }
        else if (typ == TypeSignature::FLOAT)
        {
            return Env.GetStaticFloatField(_clazz, fid);
        }
        else if (typ == TypeSignature::DOUBLE)
        {
            return Env.GetStaticDoubleField(_clazz, fid);
        }
        else if (typ == TypeSignature::STRING)
        {
            return (jstring)Env.GetStaticObjectField(_clazz, fid);
        }
        else
        {
            return Env.GetStaticObjectField(_clazz, fid);
        }
    }

    return JVariant();
}

JVariant JField::operator()(jobject obj) const
{
    AJNI_CHECKEXCEPTION;

    if (!is_static)
    {
        auto fid = Env.GetFieldID(_clazz, name.c_str(), typ.c_str());
        if (!fid)
        {
            Logger::Error("没有找到静态变量 " + name + typ);
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

string JMethod::signature(args_type const &args, args_signatures_type const &predefs) const
{
    if (predefs.size())
    {
        string sig = "(" + CROSS_NS::implode(predefs, "") + ")" + sreturn;
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

JVariant JMethod::operator()(JObject& obj) const
{
    return invoke(obj, {});
}

JVariant JMethod::operator()(JObject& obj, JVariant const &v) const
{
    return invoke(obj, {v});
}

JVariant JMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1) const
{
    return invoke(obj, {v, v1});
}

JVariant JMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2) const
{
    return invoke(obj, {v, v1, v2});
}

JVariant JMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3) const
{
    return invoke(obj, {v, v1, v2, v3});
}

JVariant JMethod::operator()(JObject& obj, JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4) const
{
    return invoke(obj, {v, v1, v2, v3, v4});
}

JVariant JMethod::invoke(::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    if (is_construct)
    {
        auto mid = Env.GetMethodID(_clazz, "<init>", sig);
        if (!mid)
        {
            Logger::Error("没有找到构造函数 " + name + " " + sig);
            return JVariant();
        }

        return Env.NewObject(_clazz, mid, jvals);
    }

    if (is_static)
    {
        auto mid = Env.GetStaticMethodID(_clazz, name, sig);
        if (!mid)
        {
            Logger::Error("没有找到函数 " + name + sig);
            return JVariant();
        }

        if (sreturn == TypeSignature::BOOLEAN)
        {
            return Env.CallStaticBooleanMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::BYTE)
        {
            return Env.CallStaticByteMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::CHAR)
        {
            return Env.CallStaticCharMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::SHORT)
        {
            return Env.CallStaticShortMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::INT)
        {
            return Env.CallStaticIntMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::LONG)
        {
            return Env.CallStaticLongMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::FLOAT)
        {
            return Env.CallStaticFloatMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::DOUBLE)
        {
            return Env.CallStaticDoubleMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::STRING)
        {
            return (jstring)Env.CallStaticObjectMethod(_clazz, mid, jvals);
        }
        else if (sreturn == TypeSignature::VOID)
        {
            Env.CallStaticVoidMethod(_clazz, mid, jvals);
        }
        else
        {
            return Env.CallStaticObjectMethod(_clazz, mid, jvals);
        }
    }
    return JVariant();
}

JVariant JMethod::invoke(JObject& obj, ::std::vector<JVariant> const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    if (!is_static)
    {
        auto mid = Env.GetMethodID(_clazz, name, sig);
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
    }
    return JVariant();
}

JClass::JClass(JClassPath const&path)
    : _clazzpath(path), construct(*this)
{
    if (!path.empty()) {
        _clazz = Env.FindClass(path);
    }

    construct.is_construct = true;
    construct.sreturn = TypeSignature::VOID;
}

JClassName JClass::name() const
{
    return CROSS_NS::replace(_clazzpath, "/", ".");
}

bool JClass::exists() const {
    return _clazz != nullptr;
}

NNT_SINGLETON_IMPL(JContext);

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

JContext::class_type JContext::find_class(JClassPath const& ph) const {
    auto fnd = d_ptr->classes.find(ph);
    return fnd == d_ptr->classes.end() ? nullptr : fnd->second;
}

AJNI_END
