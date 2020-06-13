#include "core.hpp"
#include "ajni++.hpp"
#include "java-prv.hpp"
#include "jre.hpp"

AJNI_BEGIN

JEnvThreadAutoGuard::~JEnvThreadAutoGuard()
{
    classes.clear();

    // 清理结束后才能释放env
    free_env();
}

extern shared_ptr<JVariant> ReadToVariant(jobject _obj);

JGlobalObject::JGlobalObject(jobject obj)
        : JObject(obj)
{
    if (_obj)
        _obj = Env.NewGlobalRef(_obj);
}

JGlobalObject::JGlobalObject(JGlobalObject const &r)
        : JObject(r._obj)
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

JGlobalObject &JGlobalObject::operator = (jobject r) {
    if (_obj == r)
        return *this;

    if (_obj)
        Env.DeleteGlobalRef(_obj);

    _obj = r;
    if (_obj) {
        _obj = Env.NewGlobalRef(_obj);
    }

    return *this;
}

jobject JGlobalObject::asReturn() const
{
    return Env.NewGlobalRef(_obj);
}

shared_ptr<JVariant> JGlobalObject::toVariant() const
{
    return ReadToVariant(_obj);
}

shared_ptr<JObject> JGlobalObject::make_shared(jobject obj)
{
    ::std::shared_ptr<JObject> r((JObject*)(new JGlobalObject(obj)));
    return r;
}

shared_ptr<JVariant> ReadToVariant(jobject _obj)
{
    if (_obj == nullptr)
        return make_shared<JVariant>(); // 不能返回null，客户端收到的是引用类型，通过vt判断

    auto& ctx = Env.context();

    auto STD_NUMBER = ctx.register_class<jre::Number>();
    if (Env.IsInstanceOf(_obj, *STD_NUMBER)) {
        auto STD_DOUBLE = ctx.register_class<jre::Double>();
        if (Env.IsInstanceOf(_obj, *STD_DOUBLE)) {
            JEntry<jre::Double> ref(_obj);
            return ref->doubleValue(ref);
        }

        auto STD_FLOAT = ctx.register_class<jre::Float>();
        if (Env.IsInstanceOf(_obj, *STD_FLOAT)) {
            JEntry<jre::Float> ref(_obj);
            return ref->floatValue(ref);
        }

        JEntry<jre::Number> ref(_obj);
        return ref->longValue(ref);
    }

    auto STD_STRING = ctx.register_class<jre::String>();
    if (Env.IsInstanceOf(_obj, *STD_STRING)) {
        JEntry<jre::String> ref(_obj);
        return ref->getBytes(ref);
    }

    return _V(_obj);
}

AJNI_END