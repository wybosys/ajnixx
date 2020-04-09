#ifndef _AJNI_INSPECT_H
#define _AJNI_INSPECT_H

AJNI_BEGIN

class JInspect
{
public:

    JInspect(const JClass&);

private:
    const JClass& _clz;
};

AJNI_END

#endif

