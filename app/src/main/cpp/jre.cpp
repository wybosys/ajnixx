#include "ajni.h"
#include "jre.h"

AJNI_BEGIN_NS(jre)

Object::Object(const JClassName &name)
: JClass(name), toString(*this) {
    toString.name = "toString";
    toString.returntyp = jt::String;
}

Throwable::Throwable(const JClassName &name)
: Object(name) {

}

AJNI_END_NS