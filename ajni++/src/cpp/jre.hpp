#ifndef __AJNI_JRE_H_INCLUDED
#define __AJNI_JRE_H_INCLUDED

AJNI_BEGIN_NS(jre)

class Object : public JClass
{
public:
    static JClassPath CLASSPATH;

    Object(JClassPath const& = CLASSPATH);
    JMethod toString;
};

class Throwable : public Object
{
public:
    static JClassPath CLASSPATH;

    Throwable(JClassPath const& = CLASSPATH);
};

AJNI_END_NS

#endif