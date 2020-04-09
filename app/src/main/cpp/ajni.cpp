#include "ajni.h"

AJNI_BEGIN

static JNIEnv *gs_env = nullptr;

JNIEnv *Env() {
    return gs_env;
}

JClass::JClass(const ajni::JClassName &name)
: _clazzname(name), _clazz(nullptr), _instance(nullptr) {

}

JClass::~JClass() {

}

JClass::instance_type JClass::instance() throw(exception) {
    if (_instance != nullptr)
        throw "已经实例化";
    auto r = make_shared<JClass>(_clazzname);
    if (_clazz == nullptr) {
        // 查找类
        _clazz = gs_env->FindClass(_clazzname.c_str());
        if (_clazz == nullptr)
            throw "没找到类型" + _clazzname;
    }
    r->_clazz = _clazz;
    return r;
}

AJNI_END

void AJNI_FUNC(AJni_Init)(JNIEnv *env, jobject self) {
    ajni::gs_env = env;
}

void AJNI_FUNC(AJni_Main)(JNIEnv *env, jobject self) {
    // pass
}