#include "ajni.h"
#include "inspect.h"

AJNI_BEGIN

class JReflectClass: public JClass
{
public:

    JReflectClass()
    : JClass("java/lang/Class"),
    ForName(*this)
    {
        ForName.name = "forName";
        ForName.is_static = true;
        ForName.returntyp = jt::Class;
        ForName.argtyps.emplace_back(jt::String);
    }

    JMethod ForName;
};

struct JInspectPrivate {
    shared_ptr<JReflectClass> rclass;
};

JInspect::JInspect(const ajni::JClass &clz)
:_clz(clz) {
    AJNI_CLASS_CONSTRUCT()

    JReflectClass Class;
    JVariant t = Class.ForName(clz._clazzname);
}

JInspect::~JInspect() {
    AJNI_CLASS_DESTORY()
}

AJNI_END
