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

typedef shared_ptr<JVariant> return_type;
typedef JVariant arg_type;

class JMemberField : public JField
{
public:

    JMemberField(JClass const& clz) : JField(clz) {}

    // get
    return_type operator()(JObject&) const;

    // set
    void operator()(JObject&, arg_type const&);
};

class JStaticField : public JField
{
public:

    JStaticField(JClass const& clz) : JField(clz) {}

    // get
    return_type operator()() const;

    // set
    void operator()(JObject&, arg_type const&);
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
    JTypeSignature sreturn;

    // 参数类型, 设置则代表不使用自动推导，手动指定入参表
    typedef ::std::vector<JTypeSignature> args_signatures_type;
    args_signatures_type sargs;

    // 生成函数标记
    string signature(args_type const &, args_signatures_type const & = {}) const;

    static string Signature(args_type const &, JTypeSignature const& sreturn, args_signatures_type const & = {});

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
};

class JMemberMethod : public JMethod
{
public:

    JMemberMethod(JClass const& clz) : JMethod(clz) {}

    return_type operator()(JObject&) const;
    return_type operator()(JObject&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;
    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;

    virtual return_type invoke(JObject&, args_type const &) const;
};

class JStaticMethod : public JMethod
{
public:

    JStaticMethod(JClass const& clz) : JMethod(clz) {}

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
    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;


    virtual return_type invoke(args_type const &) const;

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

    inline jclass clazz() const {
        return (jclass)(jobject)_clazz;
    }

    inline operator jclass () const {
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

    typedef typename JVariant::function_type function_type;
    typedef size_t function_index_type;

    // 保存函数返回索引
    function_index_type add(shared_ptr<function_type> const&);

    // 增加函数计数
    void function_grab(function_index_type);

    // 减少函数技术，释放返回true，否则返回false
    bool function_drop(function_index_type);

    // 获得函数
    shared_ptr<function_type> find_function(function_index_type) const;

    // 清空
    void clear();
};

// 实例定义
template <typename TClass>
class JEntry
{
public:

    typedef JContext::class_type class_type;

    JEntry(shared_ptr<JObject> const& obj)
    : _obj(obj)
    {
        _clazz = Env.context().register_class<TClass>();
    }

    inline TClass* operator -> () {
        return dynamic_cast<TClass*>(_clazz.get());
    }

    inline TClass const* operator -> () const {
        return dynamic_cast<TClass const*>(_clazz.get());
    }

    inline operator JObject& () {
        return *_obj;
    }

    inline jobject asReturn() const {
        return _obj->asReturn();
    }

private:

    JContext::class_type _clazz;
    shared_ptr<JObject> _obj;
};

AJNI_END

#endif