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

JClassPath const Number::CLASSPATH = "java/lang/Number";

Number::Number(JClassPath const& cp)
: Object(cp),
longValue(*this)
{
    longValue.name = "longValue";
    longValue.sreturn = ::AJNI_NS::TypeSignature::LONG;
}

JClassPath const Float::CLASSPATH = "java/lang/Float";

Float::Float(JClassPath const& cp)
: Number(cp),
floatValue(*this)
{
    floatValue.name = "floatValue";
    floatValue.sreturn = ::AJNI_NS::TypeSignature::FLOAT;
}

JClassPath const Double::CLASSPATH = "java/lang/Double";

Double::Double(JClassPath const& cp)
: Number(cp),
doubleValue(*this)
{
    doubleValue.name = "doubleValue";
    doubleValue.sreturn = ::AJNI_NS::TypeSignature::DOUBLE;
}

JClassPath const String::CLASSPATH = "java/lang/String";

String::String(JClassPath const& cp)
: Object(cp),
getBytes(*this)
{
    getBytes.name = "getBytes";
    getBytes.sreturn = ::AJNI_NS::TypeSignature::BYTEARRAY;
}

namespace TypeSignature
{
    const JTypeSignature CALLBACK = "Lcom/nnt/ajnixx/Callback;";
}

JClassPath const Callback::CLASSPATH = "com/nnt/ajnixx/Callback";

Callback::Callback(JClassPath const& cp)
: Object(cp),
id(*this)
{
    id.name = "id";
    id.stype = ::AJNI_NS::TypeSignature::LONG;
}

AJNI_END_NS