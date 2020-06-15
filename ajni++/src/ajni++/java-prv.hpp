#ifndef __AJNI_PRIVATE_GLOBAL_H_INCLUDED
#define __AJNI_PRIVATE_GLOBAL_H_INCLUDED

AJNI_BEGIN

// 推出后自动释放的控制
class JEnvThreadAutoGuard
{
public:

    JEnvThreadAutoGuard();
    ~JEnvThreadAutoGuard();

    void free_env();

    // 推出线程时是否需要释放env
    bool detach = false;

    static JEnvThreadAutoGuard& tls();
};

namespace TypeSignature {

    enum struct TS {
        UNKNOWN,
        CLASS,
        STRING,
        OBJECT,
        BOOLEAN,
        BYTE,
        CHAR,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        VOID,
        BYTEARRAY
    };

    // 简化switch写法的工具函数
    extern TS GetTypeForSwitch(JTypeSignature const&);
}

AJNI_END

#endif
