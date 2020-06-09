#ifndef __AJNIXX_KOTLIN_H_INCLUDED
#define __AJNIXX_KOTLIN_H_INCLUDED

#include "ajni++.hpp"

AJNI_BEGIN_NS(kotlin)

class JClass : public ::AJNI_NS::JClass
{
public:

    JClass(JClassPath const& cp);

    virtual jclass clazz$() const;

    inline JObject const& object$() const {
        return _object$;
    }

protected:

    JObject _object$;
    JObject _clazz$;
    JClassPath _classpath$;
};

class JStaticMethod : public ::AJNI_NS::JStaticMethod
{
public:

    JStaticMethod(JClass const& clz)
    : ::AJNI_NS::JStaticMethod(clz)
    {}

    virtual JVariant invoke(::std::vector<JVariant> const &) const;
};

AJNI_END_NS

#endif