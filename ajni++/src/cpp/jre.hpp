#ifndef __AJNI_JRE_H_INCLUDED
#define __AJNI_JRE_H_INCLUDED

AJNI_BEGIN_NS(jre)

class Object : public JClass
{
public:
    static JClassPath const CLASSPATH;

    Object(JClassPath const& = CLASSPATH);
    JMemberMethod toString;
};

class Throwable : public Object
{
public:
    static JClassPath const CLASSPATH;

    Throwable(JClassPath const& = CLASSPATH);
};

namespace TypeSignature
{
    extern const JClassPath CALLBACK;
}

class Callback : public Object
{
public:
    static JClassPath const CLASSPATH;

    Callback(JClassPath const& = CLASSPATH);

    JMemberField id;
};

AJNI_END_NS

#endif