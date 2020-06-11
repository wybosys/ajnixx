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

class Number : public Object
{
public:
    static JClassPath const CLASSPATH;

    Number(JClassPath const& = CLASSPATH);
    JMemberField longValue;
};

class Float : public Number
{
public:
    static JClassPath const CLASSPATH;

    Float(JClassPath const& = CLASSPATH);
    JMemberField floatValue;
};

class Double : public Number
{
public:
    static JClassPath const CLASSPATH;

    Double(JClassPath const& = CLASSPATH);
    JMemberField doubleValue;
};

class String : public Object
{
public:
    static JClassPath const CLASSPATH;

    String(JClassPath const& = CLASSPATH);
    JMemberField getBytes;
};

namespace TypeSignature
{
    extern const JTypeSignature CALLBACK;
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