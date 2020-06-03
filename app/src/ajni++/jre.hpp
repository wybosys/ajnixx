#ifndef __AJNI_JRE_H_INCLUDED
#define __AJNI_JRE_H_INCLUDED

AJNI_BEGIN_NS(jre)

class Object : public JClass
{
public:
    Object(const JClassPath &path = "java/lang/Object");
    JMethod toString;
};

class Throwable : public Object
{
public:
    Throwable(const JClassPath &path = "java/lang/Throwable");
};

AJNI_END_NS

#endif