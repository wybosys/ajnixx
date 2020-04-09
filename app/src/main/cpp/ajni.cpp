#include "ajni.h"

AJNI_BEGIN

namespace jt {
    const string Class = "java/lang/Class";
    const string String = "java/lang/String";
}

static JNIEnv *gs_env = nullptr;

JNIEnv *Env() {
    return gs_env;
}

jobject JClassGetInstance(JClass& jc) {
    return jc._instance;
}

JVariant::JVariant(JVariant& r)
: _typ(r._typ),
_vobj(r._vobj),
_vbl(r._vbl),
_vc(r._vc),
_vb(r._vb),
_vs(r._vs),
_vi(r._vi),
_vl(r._vl),
_vf(r._vf),
_vd(r._vd),
_vss(r._vss) {
}

JVariant::JVariant(JClass& r)
: _typ(JVariant::OBJECT), _vobj(JClassGetInstance(r)) {
}

JVariant::JVariant(bool v)
: _typ(JVariant::BOOLEAN), _vbl(v ? 1 : 0) {
}

JVariant::JVariant(jchar v)
: _typ(JVariant::CHAR), _vc(v) {
}

JVariant::JVariant(jbyte v)
: _typ(JVariant::BYTE), _vb(v) {
}

JVariant::JVariant(jshort v)
: _typ(JVariant::SHORT), _vs(v) {
}

JVariant::JVariant(jint v)
: _typ(JVariant::INT), _vi(v) {
}

JVariant::JVariant(jlong v)
: _typ(JVariant::LONG), _vl(v) {
}

JVariant::JVariant(jfloat v)
: _typ(JVariant::FLOAT), _vf(v) {
}

JVariant::JVariant(jdouble v)
: _typ(JVariant::DOUBLE), _vd(v) {
}

JVariant::JVariant(const string& v)
: _typ(JVariant::STRING), _vss(v) {
}

JClass::JClass(const ajni::JClassName &name)
: _clazzname(name), _instance(nullptr) {
    _clazz = gs_env->FindClass(_clazzname.c_str());
}

JClass::~JClass() {

}

JClass::instance_type JClass::instance() throw(exception) {
    if (_instance != nullptr)
        throw "已经实例化";
    auto r = make_shared<JClass>(_clazzname);
    if (_clazz == nullptr)
        throw "没找到类型" + _clazzname;
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