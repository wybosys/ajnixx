#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"
#include "variant.hpp"
#include <atomic>

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

USE_CROSS

return_type JStaticField::operator()() const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetStaticFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到静态变量 " + name + stype);
        return nullptr;
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return _V(Env.GetStaticBooleanField(clz, fid));
    }
    else if (stype == TypeSignature::BYTE)
    {
        return _V(Env.GetStaticByteField(clz, fid));
    }
    else if (stype == TypeSignature::CHAR)
    {
        return _V(Env.GetStaticCharField(clz, fid));
    }
    else if (stype == TypeSignature::SHORT)
    {
        return _V(Env.GetStaticShortField(clz, fid));
    }
    else if (stype == TypeSignature::INT)
    {
        return _V(Env.GetStaticIntField(clz, fid));
    }
    else if (stype == TypeSignature::LONG)
    {
        return _V(Env.GetStaticLongField(clz, fid));
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return _V(Env.GetStaticFloatField(clz, fid));
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return _V(Env.GetStaticDoubleField(clz, fid));
    }
    else if (stype == TypeSignature::STRING)
    {
        return _V((jstring)Env.GetStaticObjectField(clz, fid));
    }
    else if (stype == TypeSignature::BYTEARRAY)
    {
        JArray arr((jarray)Env.GetStaticObjectField(clz, fid));
        return _V(arr.toString());
    }

    return _V(Env.GetStaticObjectField(clz, fid));
}

void JStaticField::operator()(JObject& obj, arg_type v)
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetStaticFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到静态变量 " + name + stype);
        return;
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        Env.SetStaticBooleanField(clz, fid, v.toBool());
    }
    else if (stype == TypeSignature::BYTE)
    {
        Env.SetStaticByteField(clz, fid, v.toInteger());
    }
    else if (stype == TypeSignature::CHAR)
    {
        Env.SetStaticCharField(clz, fid, v.toInteger());
    }
    else if (stype == TypeSignature::SHORT)
    {
        Env.SetStaticShortField(clz, fid, v.toInteger());
    }
    else if (stype == TypeSignature::INT)
    {
        Env.SetStaticIntField(clz, fid, v.toInteger());
    }
    else if (stype == TypeSignature::LONG)
    {
        Env.SetStaticLongField(clz, fid, v.toInteger());
    }
    else if (stype == TypeSignature::FLOAT)
    {
        Env.SetStaticFloatField(clz, fid, v.toNumber());
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        Env.SetStaticDoubleField(clz, fid, v.toNumber());
    }
    else if (stype == TypeSignature::STRING)
    {
        Env.SetStaticObjectField(clz, fid, JValue(v));
    }
    else {
        Env.SetStaticObjectField(clz, fid, *v.toObject());
    }
}

return_type JMemberField::operator()(JObject& obj) const
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到成员变量 " + name + stype);
        return nullptr;
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        return _V(Env.GetBooleanField(obj, fid));
    }
    else if (stype == TypeSignature::BYTE)
    {
        return _V(Env.GetByteField(obj, fid));
    }
    else if (stype == TypeSignature::CHAR)
    {
        return _V(Env.GetCharField(obj, fid));
    }
    else if (stype == TypeSignature::SHORT)
    {
        return _V(Env.GetShortField(obj, fid));
    }
    else if (stype == TypeSignature::INT)
    {
        return _V(Env.GetIntField(obj, fid));
    }
    else if (stype == TypeSignature::LONG)
    {
        return _V(Env.GetLongField(obj, fid));
    }
    else if (stype == TypeSignature::FLOAT)
    {
        return _V(Env.GetFloatField(obj, fid));
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        return _V(Env.GetDoubleField(obj, fid));
    }
    else if (stype == TypeSignature::STRING)
    {
        return _V((jstring)Env.GetObjectField(obj, fid));
    }
    else if (stype == TypeSignature::BYTEARRAY)
    {
        JArray arr((jarray)Env.GetObjectField(obj, fid));
        return _V(arr.toString());
    }

    return _V(Env.GetObjectField(obj, fid));
}

void JMemberField::operator()(JObject& obj, arg_type v)
{
    AJNI_CHECKEXCEPTION;

    auto clz = _clazz.clazz();

    auto fid = Env.GetFieldID(clz, name.c_str(), stype.c_str());
    if (!fid)
    {
        Logger::Error("没有找到成员变量 " + name + stype);
        return;
    }

    if (stype == TypeSignature::BOOLEAN)
    {
        Env.SetBooleanField(obj, fid, v.toBool());
    }
    else if (stype == TypeSignature::BYTE)
    {
        Env.SetByteField(obj, fid, v.toInteger());
    }
    else if (stype == TypeSignature::CHAR)
    {
        Env.SetCharField(obj, fid, v.toInteger());
    }
    else if (stype == TypeSignature::SHORT)
    {
        Env.SetShortField(obj, fid, v.toInteger());
    }
    else if (stype == TypeSignature::INT)
    {
        Env.SetIntField(obj, fid, v.toInteger());
    }
    else if (stype == TypeSignature::LONG)
    {
        Env.SetLongField(obj, fid, v.toInteger());
    }
    else if (stype == TypeSignature::FLOAT)
    {
        Env.SetFloatField(obj, fid, v.toNumber());
    }
    else if (stype == TypeSignature::DOUBLE)
    {
        Env.SetDoubleField(obj, fid, v.toNumber());
    }
    else if (stype == TypeSignature::STRING)
    {
        JValue jv(v);
        Env.SetObjectField(obj, fid, jv);
    }
    else
    {
        Env.SetObjectField(obj, fid, *v.toObject());
    }
}

return_type JConstructMethod::operator()() const
{
    return invoke({});
}

return_type JConstructMethod::operator()(arg_type v) const
{
    return invoke({&v});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1) const
{
    return invoke({&v, &v1});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2) const
{
    return invoke({&v, &v1, &v2});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3) const
{
    return invoke({&v, &v1, &v2, &v3});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4) const
{
    return invoke({&v, &v1, &v2, &v3, &v4});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7});
}

return_type JConstructMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7, arg_type v8) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8});
}

return_type JStaticMethod::operator()() const
{
    return invoke({});
}

return_type JStaticMethod::operator()(arg_type v) const
{
    return invoke({&v});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1) const
{
    return invoke({&v, &v1});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2) const
{
    return invoke({&v, &v1, &v2});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3) const
{
    return invoke({&v, &v1, &v2, &v3});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4) const
{
    return invoke({&v, &v1, &v2, &v3, &v4});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7});
}

return_type JStaticMethod::operator()(arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7, arg_type v8) const
{
    return invoke({&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8});
}

string JMethod::signature(args_type const &args, args_signatures_type const &predefs) const
{
    return Signature(args, sreturn, predefs);
}

string JMethod::Signature(args_type const &args, JTypeSignature const& sreturn, args_signatures_type const &predefs)
{
    if (predefs.size())
    {
        ::std::vector<string> tss(predefs.begin(), predefs.end());
        string sig = "(" + implode(tss, "") + ")" + sreturn;
        return sig;
    }

    ::std::vector<string> ps;
    for (auto &e : args)
    {
        ps.emplace_back(e->signature());
    }

    string sig = "(" + implode(ps, "") + ")" + sreturn;
    return sig;
}

return_type JMemberMethod::operator()(JObject& obj) const
{
    return invoke(obj, {});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v) const
{
    return invoke(obj, {&v});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1) const
{
    return invoke(obj, {&v, &v1});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2) const
{
    return invoke(obj, {&v, &v1, &v2});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3) const
{
    return invoke(obj, {&v, &v1, &v2, &v3});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4) const
{
    return invoke(obj, {&v, &v1, &v2, &v3, &v4});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5) const
{
    return invoke(obj, {&v, &v1, &v2, &v3, &v4, &v5});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6) const
{
    return invoke(obj, {&v, &v1, &v2, &v3, &v4, &v5, &v6});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7) const
{
    return invoke(obj, {&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7});
}

return_type JMemberMethod::operator()(JObject& obj, arg_type v, arg_type v1, arg_type v2, arg_type v3, arg_type v4, arg_type v5, arg_type v6, arg_type v7, arg_type v8) const
{
    return invoke(obj, {&v, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8});
}

return_type JConstructMethod::invoke(args_type const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();
    auto mid = Env.GetMethodID(clz, "<init>", sig);
    if (!mid)
    {
        Logger::Error("没有找到构造函数 " + name + " " + sig);
        return nullptr;
    }

    return _V(Env.NewObject(clz, mid, jvals));
}

return_type JStaticMethod::invoke(args_type const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
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
    else if (sreturn == TypeSignature::BYTEARRAY)
    {
        JArray arr((jarray)Env.CallStaticObjectMethod(clz, mid, jvals));
        return _V(arr.toString());
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallStaticVoidMethod(clz, mid, jvals);
        return nullptr;
    }

    return _V(Env.CallStaticObjectMethod(clz, mid, jvals));
}

return_type JMemberMethod::invoke(JObject& obj, args_type const &args) const
{
    AJNI_CHECKEXCEPTION;

    string sig = signature(args, sargs);
    JValues jvals(args);

    auto clz = _clazz.clazz();

    auto mid = Env.GetMethodID(clz, name, sig);
    if (!mid)
    {
        Logger::Error("没有找到函数 " + name + sig);
        return nullptr;
    }

    if (sreturn == TypeSignature::BOOLEAN)
    {
        return _V(Env.CallBooleanMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::BYTE)
    {
        return _V(Env.CallByteMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::CHAR)
    {
        return _V(Env.CallCharMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::SHORT)
    {
        return _V(Env.CallShortMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::INT)
    {
        return _V(Env.CallIntMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::LONG)
    {
        return _V(Env.CallLongMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::FLOAT)
    {
        return _V(Env.CallFloatMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::DOUBLE)
    {
        return _V(Env.CallDoubleMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::STRING)
    {
        return _V((jstring)Env.CallObjectMethod(obj, mid, jvals));
    }
    else if (sreturn == TypeSignature::BYTEARRAY)
    {
        JArray arr((jarray)Env.CallObjectMethod(obj, mid, jvals));
        return _V(arr.toString());
    }
    else if (sreturn == TypeSignature::VOID)
    {
        Env.CallVoidMethod(obj, mid, jvals);
        return nullptr;
    }

    return _V(Env.CallObjectMethod(obj, mid, jvals));
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

    JContextPrivate()
    : index_functions(0)
    {}

    typedef ::std::map<JClassPath, JContext::class_type> classes_type;
    classes_type classes;

    class FunctionType {
    public:

        typedef shared_ptr<JVariant::function_type> function_type;

        FunctionType(function_type _fn)
        : fn(_fn), referencedCount(1)
        {}

        function_type fn;
        ::std::atomic<size_t> referencedCount;
    };

    typedef shared_ptr<FunctionType> function_type;
    typedef ::std::map<size_t, function_type> functions_type;
    functions_type functions;

    ::std::atomic<size_t> index_functions;
    ::std::mutex mtx_functions;
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
    d_ptr->functions.clear();
    d_ptr->index_functions = 0;
}

size_t JContext::add(shared_ptr<function_type> const& fn)
{
    size_t idx = ++d_ptr->index_functions;
    NNT_AUTOGUARD(d_ptr->mtx_functions);
    d_ptr->functions[idx] = make_shared<private_class_type::FunctionType>(fn);
    return idx;
}

void JContext::function_grab(function_index_type fnid)
{
    NNT_AUTOGUARD(d_ptr->mtx_functions);
    auto fnd = d_ptr->functions.find(fnid);
    if (fnd != d_ptr->functions.end()) {
        ++fnd->second->referencedCount;
    } else {
        Logger::Error("没有找到函数索引 " + tostr((int)fnid));
    }
}

bool JContext::function_drop(function_index_type fnid)
{
    NNT_AUTOGUARD(d_ptr->mtx_functions);
    auto fnd = d_ptr->functions.find(fnid);
    if (fnd != d_ptr->functions.end()) {
        if (--fnd->second->referencedCount == 0) {
            d_ptr->functions.erase(fnd);
            return true;
        }
    } else {
        Logger::Error("没有找到函数索引 " + tostr((int)fnid));
    }
    return false;
}

shared_ptr<JContext::function_type> JContext::find_function(function_index_type fnid) const
{
    NNT_AUTOGUARD(d_ptr->mtx_functions);
    auto fnd = d_ptr->functions.find(fnid);
    return fnd == d_ptr->functions.end() ? nullptr : fnd->second->fn;
}

AJNI_END
