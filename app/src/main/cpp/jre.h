#ifndef _AJNI_JRE_H
#define _AJNI_JRE_H

AJNI_BEGIN_NS(jre)

class Object: public JClass {
public:
    Object(const JClassName& name="java/lang/Object");
    JMethod toString;
};

class Throwable: public Object {
public:
    Throwable(const JClassName& name="java/lang/Throwable");
};

AJNI_END_NS

#endif