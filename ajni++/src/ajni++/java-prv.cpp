#include "core.hpp"
#include "ajni++.hpp"
#include "java-prv.hpp"
#include "jre.hpp"

#include <cross/cross.hpp>
#include <cross/str.hpp>
#include <cross/sys.hpp>

AJNI_BEGIN

JEnvThreadAutoGuard::~JEnvThreadAutoGuard()
{
    // 清理结束后才能释放env
    free_env();

    string pid = ::CROSS_NS::tostr(::CROSS_NS::get_thread_id());
    Logger::Info("线程" + pid + ": 释放线程级JNIEnv资源");
}

shared_ptr<JVariant> JObject::Extract(jobject _obj) {
    if (_obj == nullptr) {
        return ::std::make_shared<JVariant>(); // 不能返回null，客户端收到的是引用类型，通过vt判断
    }

    auto obj = make_shared<JObject>();
    obj->_reset(_obj);

    auto& ctx = Env.context();

    auto STD_NUMBER = ctx.register_class<jre::Number>();
    if (Env.IsInstanceOf(*obj, *STD_NUMBER)) {
        auto STD_DOUBLE = ctx.register_class<jre::Double>();
        if (Env.IsInstanceOf(*obj, *STD_DOUBLE)) {
            JEntry<jre::Double> ref(obj);
            return ref->doubleValue(ref);
        }

        auto STD_FLOAT = ctx.register_class<jre::Float>();
        if (Env.IsInstanceOf(*obj, *STD_FLOAT)) {
            JEntry<jre::Float> ref(obj);
            return ref->floatValue(ref);
        }

        JEntry<jre::Number> ref(obj);
        return ref->longValue(ref);
    }

    auto STD_STRING = ctx.register_class<jre::String>();
    if (Env.IsInstanceOf(*obj, *STD_STRING)) {
        JEntry<jre::String> ref(obj);
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