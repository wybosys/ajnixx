#ifndef __AJNIXX_KOTLIN_H_INCLUDED
#define __AJNIXX_KOTLIN_H_INCLUDED

#include "ajni++.hpp"

AJNI_BEGIN_NS(kotlin)

class JClass : public ::AJNI_NS::JClass
{
public:

    JClass(JClassPath const& cp);

    virtual jclass static_clazz() const;

protected:

    JObject _clazz$;
};

class JStaticMethod : public ::AJNI_NS::JStaticMethod
{
public:

    JStaticMethod(JClass const& clz)
    : ::AJNI_NS::JStaticMethod(clz)
    {}

    JVariant invoke(::std::vector<JVariant> const &) const;
};

AJNI_END_NS

#endif