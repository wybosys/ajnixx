#ifndef __AJNI_PRIVATE_GLOBAL_H_INCLUDED
#define __AJNI_PRIVATE_GLOBAL_H_INCLUDED

AJNI_BEGIN

class JGlobalObject : public JWeakObject {
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

    static shared_ptr<JWeakObject> make_shared(jobject);
};

AJNI_END

#endif