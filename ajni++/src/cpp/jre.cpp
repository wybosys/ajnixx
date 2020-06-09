#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

AJNI_BEGIN_NS(jre)

JClassPath Object::CLASSPATH = "java/lang/Object";

Object::Object(JClassPath const& cp)
    : JClass(cp), toString(*this)
{
    toString.name = "toString";
    toString.returntype = TypeSignature::STRING;
}

JClassPath Throwable::CLASSPATH = "java/lang/Throwable";

Throwable::Throwable(JClassPath const& cp)
    : Object(cp)
{
}

AJNI_END_NS