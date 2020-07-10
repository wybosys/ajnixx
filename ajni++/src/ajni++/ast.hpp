#ifndef __AJNI_AST_H_INCLUDED
#define __AJNI_AST_H_INCLUDED

#include "jnienv.hpp"
#include "variant.hpp"
#include <set>

AJNI_BEGIN

class JClass;

// 变量定义
class JField
{
public:

    JField(JClass& clz);

    virtual ~JField()
    {
    }

    // 变量名
    string name;

    // 变量类型
    JTypeSignature stype;

protected:

    JClass& _clazz;
};

typedef shared_ptr<JVariant> return_type;

typedef JVariant arg_type;

class JMemberField : public JField
{
public:

    JMemberField(JClass& clz)
        : JField(clz)
    {
    }

    // get
    return_type operator()(JObject&) const;

    // set
    void operator()(JObject&, arg_type const&);
};

class JStaticField : public JField
{
public:

    JStaticField(JClass& clz)
        : JField(clz)
    {
    }

    // get
    return_type operator()() const;

    // set
    void operator()(JObject&, arg_type const&);
};

// 方法定义
class JMethod
{
public:

    JMethod(JClass& clz);

    virtual ~JMethod()
    {
    }

    // 函数名
    string name;

    // 返回类型
    JTypeSignature sreturn = TypeSignature::VOID;

    // 如果返回的是object，框架会根据该开关判断是否遇到了Java层业务异常
    // 返回的不是object则该开关不起作用
    bool nullable = false;

    // 参数类型, 设置则代表不使用自动推导，手动指定入参表
    typedef ::std::vector<JTypeSignature> args_signatures_type;
    typedef shared_ptr<args_signatures_type> args_signatures_typep;
    args_signatures_typep sargs;

    // 生成函数标记
    string signature(args_type const&, args_signatures_typep const& = {}) const;

    static string
    Signature(args_type const&, JTypeSignature const& sreturn, args_signatures_typep const& = {});

protected:

    JClass& _clazz;
};

class JConstructMethod : public JMethod
{
public:

    JConstructMethod(JClass& clz)
        : JMethod(clz)
    {
        sreturn = TypeSignature::VOID;
    }

    return_type operator()() const;

    return_type operator()(arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&) const;

    return_type
    operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    virtual return_type invoke(args_type const&) const;

};

class JMemberMethod : public JMethod
{
public:

    JMemberMethod(JClass& clz)
        : JMethod(clz)
    {
    }

    return_type operator()(JObject&) const;

    return_type operator()(JObject&, arg_type const&) const;

    return_type operator()(JObject&, arg_type const&, arg_type const&) const;

    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&) const;

    return_type operator()(JObject&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type
    operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type
    operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&) const;

    return_type
    operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&) const;

    return_type
    operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;

    return_type
    operator()(JObject&, arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    virtual return_type invoke(JObject&, args_type const&) const;

};

class JStaticMethod : public JMethod
{
public:

    JStaticMethod(JClass& clz)
        : JMethod(clz)
    {
    }

    return_type operator()() const;

    return_type operator()(arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&) const;

    return_type
    operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    return_type operator()(arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&, arg_type const&, arg_type const&, arg_type const&,
        arg_type const&) const;

    virtual return_type invoke(args_type const&) const;

};

// 类定义
class JClass
{
public:

    JClass(JClassPath const& = "");

    virtual ~JClass()
    {
    }

    // 类名
    JClassName name() const;

    // 类路径
    JClassPath const& path() const;

    // 是否存在
    bool exists() const;

    // 定义构造函数
    JConstructMethod construct;

protected:

    JObject _clazz;
    JClassPath _clazzpath;

    // 提权到global
    virtual void _asglobal();

    friend class JEnv;

    friend class JEnvPrivate;

    friend class JContext;

    friend class kotlin::JClass;
};

NNT_CLASS_PREPARE(JContext);

// 上下文环境
class JContext
{
NNT_CLASS_DECL(JContext);

public:

    JContext();

    ~JContext();

    typedef shared_ptr<JClass> class_typep;

    // 添加类对象，避免重复构造，参照 register_class
    bool add(class_typep const&);

    // 根据Java的类路径查找类对象
    class_typep find_class(JClassPath const&) const;

    // 注册类，避免每次使用C++映射的Java类都重新构造
    template<typename T>
    class_typep register_class()
    {
        auto fnd = find_class(T::CLASSPATH);
        if (fnd)
            return fnd;
        auto r = ::NNT_NS::make_dynamic_shared<T, JClass>();
        return add(r) ? r : nullptr;
    }

    // 保存回调函数，返回索引，使得可以穿透到Java层
    size_t add_callback(shared_ptr<JCallback> const&);

    // 增加回调计数，如果位于Java异步调用中，则避免同步调用完成后C++层回调自动被释放，从而报找不到回调的错误
    void callback_grab(size_t);

    // 减少回调计数，释放返回true，否则返回false
    bool callback_drop(size_t);

    // 根据回调索引获得回调数据
    shared_ptr<JCallback> find_callback(size_t) const;

    // 清空
    void clear();
};

// 实例定义
template<typename TClass>
class JEntry
{
public:

    typedef TClass class_type;
    typedef shared_ptr<JClass> class_typep;
    typedef JEntry<TClass> self_type;
    typedef shared_ptr<self_type> return_type;

    // 实例化一个实体
    template<typename ...TArgs>
    static return_type Instance(TArgs&& ...args)
    {
        auto cls = Env.context().register_class<class_type>();
        auto obj = cls->construct(::std::forward<TArgs>(args)...);
        return make_shared<self_type>(obj, cls);
    }

    // 传入其他地方已经实例化好了的
    explicit JEntry(JVariant const& var, class_typep const& clz = nullptr)
        : _clazz(clz)
    {
        assert((void*)&var != nullptr);
        assert(var.vt == JVariant::VT::OBJECT); // 只有object对象才可以转换为Entry对象

        _obj = var.toObject();

        if (!_clazz)
        {
            _clazz = Env.context().register_class<class_type>();
        }
    }

    // 传入其他地方已经实例化好了的
    explicit JEntry(shared_ptr<JVariant> const& pvar, class_typep const& clz = nullptr)
        : _clazz(clz)
    {
        assert(pvar);
        assert(pvar->vt == JVariant::VT::OBJECT);

        _obj = pvar->toObject();

        if (!_clazz)
        {
            _clazz = Env.context().register_class<class_type>();
        }
    }

    explicit JEntry(shared_ptr<JObject> const& var, class_typep const& clz = nullptr)
        : _clazz(clz)
    {
        assert(var);
        _obj = var;

        if (!_clazz)
        {
            _clazz = Env.context().register_class<class_type>();
        }
    }

    inline class_type* operator->()
    {
        return dynamic_cast<class_type*>(_clazz.get());
    }

    inline class_type const* operator->() const
    {
        return dynamic_cast<class_type*>(_clazz.get());
    }

    inline operator JObject&() const
    {
        return _gobj ? *_gobj->gobj : *_obj;
    }

    inline jobject asReturn() const
    {
        return _obj->asReturn();
    }

    // 增加计数
    void grab() const;

    // 减少计数
    void drop() const;

private:

    class_typep _clazz;
    mutable shared_ptr<JObject> _obj;
    mutable shared_ptr<JObject::_JGlobalObject> _gobj;
};

// 类静态实例
template<typename TClass>
class JClassEntry
{
public:

    typedef TClass class_type;
    typedef shared_ptr<JClass> class_typep;

    explicit JClassEntry(class_typep const& clz = nullptr)
        : _clazz(clz)
    {
        if (!_clazz)
        {
            _clazz = Env.context().register_class<class_type>();
        }
    }

    inline class_type* operator->()
    {
        return dynamic_cast<class_type*>(_clazz.get());
    }

    inline class_type const* operator->() const
    {
        return dynamic_cast<class_type*>(_clazz.get());
    }

private:

    class_typep _clazz;
};

// jstring转换为cstring
extern string tostr(jstring);

template<typename TClass>
inline void JEntry<TClass>::grab() const
{
    if (_gobj)
    {
        _gobj->grab();
    }
    else
    {
        _gobj = make_shared<JObject::_JGlobalObject>(*_obj);
        _obj = nullptr; // 通常obj会跨线程使用，所以当grab后，必须释放临时对象，避免drop时挂掉
    }
}

template<typename TClass>
inline void JEntry<TClass>::drop() const
{
    if (_gobj->drop())
    {
        _gobj = nullptr;
    }
}

AJNI_END

#endif
