#ifndef __AJNI_H_INCLUDED
#define __AJNI_H_INCLUDED

#include "core.hpp"
#include "variant.hpp"

AJNI_BEGIN

class JClass;

// 变量定义
class JField
{
    NNT_NOCOPY(JField);

public:

    JField(JClass const& cls) : _cls(cls) {}

    string name;            // 变量名
    string typ;             // 变量类型
    bool is_static = false; // 是否是静态变量

    // 获取数据
    JVariant operator()() const;
    JVariant operator()(jobject) const;

protected:

    JClass const&_cls;
};

// 方法定义
class JMethod
{
    NNT_NOCOPY(JMethod);

public:

    JMethod(JClass const& cls) : _cls(cls) {}

    string name;       // 函数名
    string returntype; // 返回类型

    bool is_static = false;    // 是否是静态函数
    bool is_construct = false; // 是否是构造函数

    // 参数类型, 设置则代表不使用自动推导，手动指定入参表
    ::std::vector<JTypeSignature> argtypes;

    // 执行java函数
    JVariant operator()() const;
    JVariant operator()(JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant invoke(::std::vector<JVariant> const &) const;

    // 成员函数
    JVariant operator()(jobject) const;
    JVariant operator()(jobject, JVariant const &) const;
    JVariant operator()(jobject, JVariant const &, JVariant const &) const;
    JVariant operator()(jobject, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(jobject, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant operator()(jobject, JVariant const &, JVariant const &, JVariant const &, JVariant const &, JVariant const &) const;
    JVariant invoke(jobject, ::std::vector<JVariant> const &) const;

    // 生成函数标记
    string signature(::std::vector<JVariant> const &, ::std::vector<JTypeSignature> const & = {}) const;

protected:

    JClass const& _cls;
};

// 类定义
class JClass
{
    NNT_NOCOPY(JClass);

public:

    JClass(JClassPath const& path = "");
    virtual ~JClass();

    // jni类路径，java/lang/Class的格式
    inline JClassPath const& path() const {
        return _clazzpath;
    }

    // 类名
    JClassName name() const;

    // 是否存在
    bool exists() const;

    // 构造函数
    JMethod construct;

    inline operator jclass () const {
        return _clazz;
    }

protected:

    JClassPath _clazzpath;
    jclass _clazz = nullptr;
};

// 实例对象
template <typename Clazz = JClass>
class JEntry : public JObject
{
    NNT_NOCOPY(JEntry);

public:
    typedef shared_ptr<Clazz> clazz_type;
    typedef JEntry<Clazz> self_type;

    JEntry(jobject obj, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = obj;
    }

    JEntry(clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct();
    }

    JEntry(JVariant const &v, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct(v);
    }

    JEntry(JVariant const &v, JVariant const &v1, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct(v, v1);
    }

    JEntry(JVariant const &v, JVariant const &v1, JVariant const &v2, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct(v, v1, v2);
    }

    JEntry(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct(v, v1, v2, v3);
    }

    JEntry(JVariant const &v, JVariant const &v1, JVariant const &v2, JVariant const &v3, JVariant const &v4, clazz_type clz = make_shared<Clazz>())
        : _clazz(clz)
    {
        *this = clz->construct(v, v1, v2, v3, v4);
    }

    inline const Clazz *operator->() const
    {
        return _clazz.get();
    }

private:
    clazz_type _clazz;
};

AJNI_END

#endif