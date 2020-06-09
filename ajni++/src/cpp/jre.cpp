#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"

AJNI_BEGIN_NS(jre)

Object::Object(const JClassPath &path)
    : JClass(path), toString(*this)
{
    toString.name = "toString";
    toString.returntype = TypeSignature::STRING;
}

Throwable::Throwable(const JClassPath &path)
    : Object(path)
{
}

AJNI_END_NS