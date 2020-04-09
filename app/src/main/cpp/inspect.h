#ifndef _AJNI_INSPECT_H
#define _AJNI_INSPECT_H

AJNI_BEGIN

AJNI_CLASS_PREPARE(JInspect)

class JInspect
{
    AJNI_CLASS_DECL(JInspect)

public:

    JInspect(const JClass&);
    ~JInspect();

private:
    const JClass& _clz;
};

AJNI_END

#endif

