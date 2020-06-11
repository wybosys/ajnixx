#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

AJNI_BEGIN_NS(jre)

JClassPath const Object::CLASSPATH = "java/lang/Object";

Object::Object(JClassPath const& cp)
    : JClass(cp), toString(*this)
{
    toString.name = "toString";
    toString.sreturn = ::AJNI_NS::TypeSignature::STRING;
}

JClassPath const Throwable::CLASSPATH = "java/lang/Throwable";

Throwable::Throwable(JClassPath const& cp)
    : Object(cp)
{
}

namespace TypeSignature
{
    const JClassPath CALLBACK = "com/nnt/ajnixx/Callback";
}

JClassPath const Callback::CLASSPATH = TypeSignature::CALLBACK;

Callback::Callback(JClassPath const& cp)
: Object(cp),
id(*this)
{
    id.name = "id";
    id.stype = ::AJNI_NS::TypeSignature::LONG;
}

AJNI_END_NS