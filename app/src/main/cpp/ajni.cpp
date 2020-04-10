#include "ajni.h"

AJNI_BEGIN

namespace jt {
    const string Class = "L/java/lang/Class;";
    const string String = "L/java/lang/String;";
    const string Object = "L/java/lang/Object;";
    const string Boolean = "Z";
    const string Byte = "B";
    const string Char = "C";
    const string Short = "S";
    const string Int = "I";
    const string Long = "J";
    const string Float = "F";
    const string Double = "D";
}

static JNIEnv *gs_env = nullptr;

JNIEnv *Env() {
    return gs_env;
}

jobject JClassGetInstance(JClass& jc) {
    return jc._instance;
}

JObject::JObject(jobject obj, bool attach)
: _obj(obj) {
    if (!attach && _obj) {
        gs_env->NewLocalRef(_obj);
    }
}

JObject::JObject(ajni::JObject &r)
: _obj(r._obj)
{
    if (_obj)
        gs_env->NewLocalRef(_obj);
}

JObject::~JObject() {
    if (_obj)
        gs_env->DeleteLocalRef(_obj);
}

JString::JString(const std::string& str): _str(str) {
    _obj = gs_env->NewStringUTF(str.c_str());
}

JString::~JString() {
    gs_env->ReleaseStringChars(_obj, nullptr);
}

JVariant::JVariant(JClass& r)
: _typ(JVariant::OBJECT) {
    _v.obj = JClassGetInstance(r);
}

JVariant::JVariant(bool v)
: _typ(JVariant::BOOLEAN) {
    _v.bl = v ? 1 : 0;
}

JVariant::JVariant(jchar v)
: _typ(JVariant::CHAR) {
    _v.c = v;
}

JVariant::JVariant(jbyte v)
: _typ(JVariant::BYTE) {
    _v.b = v;
}

JVariant::JVariant(jshort v)
: _typ(JVariant::SHORT) {
    _v.s = v;
}

JVariant::JVariant(jint v)
: _typ(JVariant::INT) {
    _v.i = v;
}

JVariant::JVariant(jlong v)
: _typ(JVariant::LONG) {
    _v.l = v;
}

JVariant::JVariant(jfloat v)
: _typ(JVariant::FLOAT) {
    _v.f = v;
}

JVariant::JVariant(jdouble v)
: _typ(JVariant::DOUBLE) {
    _v.d = v;
}

JVariant::JVariant(const string& v)
: _typ(JVariant::STRING) {
    _vss = make_shared<JString>(v);
}

string JVariant::jt() const {
    switch (_typ) {
        case JVariant::OBJECT: return jt::Object;
        case JVariant::BOOLEAN: return jt::Boolean;
        case JVariant::BYTE: return jt::Byte;
        case JVariant::CHAR: return jt::Char;
        case JVariant::SHORT: return jt::Short;
        case JVariant::INT: return jt::Int;
        case JVariant::LONG: return jt::Long;
        case JVariant::FLOAT: return jt::Float;
        case JVariant::DOUBLE: return jt::Double;
        case JVariant::STRING: return jt::String;
        default: break;
    }
    return "";
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

string JMethod::signature(const vector<const JVariant*>& args) const {
    vector<string> ps;
    for (auto& e:args) {
        ps.emplace_back(e->jt());
    }
    string sig = "(" + accumulate(ps.begin(), ps.end(), string(";")) + ")" + returntyp;
    return sig;
}

JVariant JMethod::invoke(const vector<const JVariant*>& args) {
    string sig = signature(args);
    if (is_static) {
        auto mid = gs_env->GetStaticMethodID(_cls._clazz, name.c_str(), sig.c_str());
        if (mid)
            throw "没有找到函数 " + name + sig;
        //gs_env->CallStaticObjectMethod(_cls._clazz, mid, )
    } else {

    }
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