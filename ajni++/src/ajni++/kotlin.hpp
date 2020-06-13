#ifndef __AJNIXX_KOTLIN_H_INCLUDED
#define __AJNIXX_KOTLIN_H_INCLUDED

#include "ajni++.hpp"

AJNI_BEGIN_NS(kotlin)

using ::AJNI_NS::JMemberField;
using ::AJNI_NS::JMemberMethod;
using ::AJNI_NS::JStaticField;

class JClass : public ::AJNI_NS::JClass
{
public:

    JClass(JClassPath const& cp);

    jclass clazz$() const;

    JObject const& object$() const;

protected:

    JObject _object$;
    JObject _clazz$;
    JClassPath _classpath$;
};

class JStaticMethod : public ::AJNI_NS::JStaticMethod
{
public:

    JStaticMethod(JClass& clz)
    : ::AJNI_NS::JStaticMethod(clz)
    {}

    virtual return_type invoke(args_type const &) const;
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
    return_type operator()() const;

    // set
    void operator()(JVariant const&);

protected:

    ::AJNI_NS::JClass _clazz;
};

class JGlobalMethod
{
public:

    // 此处的classpath其实是文件名路径
    JGlobalMethod(JClassPath const&);

    return_type operator()() const;
    return_type operator()(arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;

    virtual return_type invoke(args_type const&) const;

    // 函数名
    string name;

    // 返回类型
    JTypeSignature sreturn;

    JMethod::args_signatures_typep sargs;

protected:

    ::AJNI_NS::JClass _clazz;
};

AJNI_END_NS

#endif