#include "ajni.h"
#include "jre.h"

AJNI_BEGIN_NS(jre)

Object::Object(const JClassPath &path)
: JClass(path), toString(*this) {
    toString.name = "toString";
    toString.returntyp = jt::String;
}

Throwable::Throwable(const JClassPath &path)
: Object(path) {

}

AJNI_END_NS