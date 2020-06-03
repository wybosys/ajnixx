#ifndef __AJNI_H_INCLUDED
#define __AJNI_H_INCLUDED

#include "core.hpp"
#include "variant.hpp"

#include <initializer_list>

AJNI_BEGIN

class JClass;

class JField
{
    NNT_NOCOPY(JField);

public:
    JField(const JClass &cls) : _cls(cls) {}

    string name;            // 变量名
    string typ;             // 变量类型
    bool is_static = false; // 是否是静态变量

    // 获取数据
    JVariant operator()() const;
    JVariant operator()(jobject) const;

protected:
    const JClass &_cls;
};

class JMethod
{
    NNT_NOCOPY(JMethod);

public:
    JMethod(const JClass &cls) : _cls(cls) {}

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
    const JClass &_cls;
};

class JClass
{
    NNT_NOCOPY(JClass);

public:
    JClass(const JClassPath &path = "");
    virtual ~JClass();

    // jni类路径，java/lang/Class的格式
    inline const JClassPath &path() const
    {
        return _clazzpath;
    }

    // 类名，java.lang.Class的格式
    JClassName name() const;

    inline jclass clazz() const
    {
        return _clazz;
    }

    // 构造函数
    JMethod construct;

protected:
    JClassPath _clazzpath;
    jclass _clazz;
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