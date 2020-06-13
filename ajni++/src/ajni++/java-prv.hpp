#ifndef __AJNI_PRIVATE_GLOBAL_H_INCLUDED
#define __AJNI_PRIVATE_GLOBAL_H_INCLUDED

AJNI_BEGIN

// 推出后自动释放的控制
class JEnvThreadAutoGuard
{
public:

    ~JEnvThreadAutoGuard();

    void free_env();

    // 推出线程时是否需要释放env
    bool detach = false;

    static JEnvThreadAutoGuard& tls();

    // 保存于线程上的注册类列表
    // typedef ::std::map<JClassPath, JContext::class_typep> classes_type;
    // classes_type classes;
};

class JGlobalObject : public JObject {
public:
    // 自动引用计数
    JGlobalObject(jobject = nullptr);

    // 复制，动引用计数
    JGlobalObject(JGlobalObject const &);

    // 释放引用计数
    virtual ~JGlobalObject();

    // 作为程序返回值输出
    jobject asReturn() const;

    shared_ptr<JVariant> toVariant() const;

    JGlobalObject &operator = (jobject);

    static shared_ptr<JObject> make_shared(jobject);
};

AJNI_END

#endif