#include "core.hpp"
#include "ajni++.hpp"
#include "inspect.hpp"

AJNI_BEGIN

class JReflectClass : public JClass
{
public:
    JReflectClass()
        : JClass("java/lang/Class"),
          ForName(*this)
    {
        ForName.name = "forName";
        ForName.is_static = true;
        ForName.returntype = TypeSignature::CLASS;
        ForName.argtypes.emplace_back(TypeSignature::STRING);
    }

    JMethod ForName;
};

struct JInspectPrivate
{
    shared_ptr<JReflectClass> rclass;
};

JInspect::JInspect(const ajni::JClass &clz)
    : _clz(clz)
{
    NNT_CLASS_CONSTRUCT();

    JReflectClass Class;
    JVariant t = Class.ForName(clz.name());
}

JInspect::~JInspect(){
    NNT_CLASS_DESTORY();
}

AJNI_END
