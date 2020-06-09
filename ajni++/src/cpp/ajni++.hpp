#ifndef __AJNI_H_INCLUDED
#define __AJNI_H_INCLUDED

#include "core.hpp"
#include "variant.hpp"

AJNI_BEGIN

class JClass;

// 变量定义
class JField
{
public:

    JField(JClass const& clz) : _clazz(clz) {}
    virtual ~JField() {}

    // 变量名
    string name;

    // 变量类型
    JTypeSignature stype;

protected:

    JClass const& _clazz;
};

class JMemberField : public JField
{
public:

    JMemberField(JClass const& clz) : JField(clz) {}

    JVariant operator()(JObject&) const;
};

class JStaticField : public JField
{
public:

    JStaticField(JClass const& clz) : JField(clz) {}

    JVariant operator()() const;
};

// 方法定义
class JMethod
{
public:

    typedef shared_ptr<JClass> class_type;

    JMethod(JClass const& clz) : _clazz(clz) {}
    virtual ~JMethod() {}

    // 函数名
    string name;

    // 返回类型
    string sreturn;

    // 参数类型, 设置则代表不使用自动推导，手动指定入参表
    typedef ::std::vector<JTypeSignature> args_signatures_type;
    args_signatures_type sargs;

    typedef ::std::vector<JVariant> args_type;

    // 生成函数标记
    string signature(args_type const &, args_signatures_type const & = {}) const;

protected:

    JClass const& _clazz;
};

class JConstructMethod : public JMethod
{
public:

    JConstructMethod(JClass const& clz) : JMethod(clz)
    {
        sreturn = TypeSignature::VOID;
    }

    JVariant operator()() const;
    JVariant operator()(JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    virtual JVariant invoke(::std::vector<JVariant> const &) const;
};

class JMemberMethod : public JMethod
{
public:

    JMemberMethod(JClass const& clz) : JMethod(clz) {}

    JVariant operator()(JObject&) const;
    JVariant operator()(JObject&, JVariant const &) const;
    JVariant operator()(JObject&, JVariant const &, JVariant const &) const;
    JVariant operator()(JObject&, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JObject&, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JObject&, JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    virtual JVariant invoke(JObject&, ::std::vector<JVariant> const &) const;
};

class JStaticMethod : public JMethod
{
public:

    JStaticMethod(JClass const& clz) : JMethod(clz) {}

    JVariant operator()() const;
    JVariant operator()(JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    virtual JVariant invoke(::std::vector<JVariant> const &) const;

};

// 类定义
class JClass
{
public:

    JClass(JClassPath const&);
    virtual ~JClass() {}

    // 类路径
    inline JClassPath const& path() const {
        return _clazzpath;
    }

    // 类名
    JClassName name() const;

    // 是否存在
    bool exists() const;

    // 定义构造函数
    JConstructMethod construct;

    virtual jclass clazz() const {
        return (jclass)(jobject)_clazz;
    }

protected:

    JObject _clazz;
    JClassPath _clazzpath;
};

NNT_CLASS_PREPARE(JContext);

// 上下文环境
class JContext
{
    NNT_CLASS_DECL(JContext);

public:

    JContext();
    ~JContext();

    typedef shared_ptr<JClass> class_type;

    // 添加类
    bool add(class_type const&);

    // 查找类
    class_type find_class(JClassPath const&) const;

    // 注册类
    template <typename T>
    class_type register_class() {
        auto fnd = find_class(T::CLASSPATH);
        if (fnd)
            return fnd;
        auto r = ::NNT_NS::make_dynamic_shared<T, JClass>();
        return add(r) ? r : nullptr;
    }

    void clear();
};

// 实例定义
template <typename TClass>
class JEntry
{
public:

    typedef JContext::class_type class_type;

    JEntry(JObject const& obj)
    : _obj(obj)
    {
        _clazz = Env.context().register_class<TClass>();
    }

    inline TClass const* operator -> () const {
        return dynamic_cast<TClass const*>(_clazz.get());
    }

    inline operator JObject& () {
        return _obj;
    }

private:

    JContext::class_type _clazz;
    JObject _obj;
};

AJNI_END

#endif