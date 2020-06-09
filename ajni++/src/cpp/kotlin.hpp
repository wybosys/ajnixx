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

class JGlobalField
{
public:

    JGlobalField(JClassPath const&);

    // 变量名
    string name;

    // 变量类型
    JTypeSignature stype;

    // get
    JVariant operator()() const;

    // set
    void operator()(JObject&, JVariant const&);

protected:

    ::AJNI_NS::JClass _clazz;
};

class JGlobalMethod
{
public:

    // 此处的classpath其实是文件名路径
    JGlobalMethod(JClassPath const&);

    JVariant operator()() const;
    JVariant operator()(JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    virtual JVariant invoke(::std::vector<JVariant> const &) const;

    // 函数名
    string name;

    // 返回类型
    JTypeSignature sreturn;

    JMethod::args_signatures_type sargs;

protected:

    ::AJNI_NS::JClass _clazz;
};

AJNI_END_NS

#endif