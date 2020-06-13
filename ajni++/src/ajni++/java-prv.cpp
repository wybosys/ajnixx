#include "core.hpp"
#include "ajni++.hpp"
#include "java-prv.hpp"
#include "jre.hpp"

AJNI_BEGIN

JEnvThreadAutoGuard::~JEnvThreadAutoGuard()
{
    // classes.clear();

    // 清理结束后才能释放env
    free_env();
}

shared_ptr<JVariant> JObject::extract() const {
    if (_obj == nullptr)
        return ::std::make_shared<JVariant>(); // 不能返回null，客户端收到的是引用类型，通过vt判断

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

namespace TypeSignature {

    // 简化switch写法的工具函数
    TS GetTypeForSwitch(JTypeSignature const& ts)
    {
        static ::std::map<string, TS> gs_types = {
                {CLASS, TS::CLASS},
                {STRING, TS::STRING},
                {OBJECT, TS::OBJECT},
                {BOOLEAN, TS::BOOLEAN},
                {BYTE, TS::BYTE},
                {CHAR, TS::CHAR},
                {SHORT, TS::SHORT},
                {INT, TS::INT},
                {LONG, TS::LONG},
                {FLOAT, TS::FLOAT},
                {DOUBLE, TS::DOUBLE},
                {VOID, TS::VOID},
                {BYTEARRAY, TS::BYTEARRAY}
        };
        auto fnd = gs_types.find(ts);
        return fnd == gs_types.end() ? TS::UNKNOWN : fnd->second;
    }
}

AJNI_END