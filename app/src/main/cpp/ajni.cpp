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

JVariant JMethod::operator ()() {
    return invoke(vector<const JVariant*>());
}

JVariant JMethod::operator ()(JVariant const& v) {
    return invoke(vector<const JVariant*>({
        &v
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1) {
    return invoke(vector<const JVariant*>({
        &v, &v1
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2) {
    return invoke(vector<const JVariant*>({
        &v, &v1, &v2
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3) {
    return invoke(vector<const JVariant*>({
        &v, &v1, &v2, &v3
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3, const JVariant& v4) {
    return invoke(vector<const JVariant*>({
        &v, &v1, &v2, &v3, &v4
    }));
}

JVariant JMethod::invoke(const vector<const JVariant*>& args) {
    return JVariant();
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