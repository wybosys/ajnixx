#include "ajni.h"
#include "stlext.h"

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

JString::JString(jstring v, bool attach) {
    jboolean cp = false;
    size_t sl = gs_env->GetStringUTFLength(v);
    _str = string(gs_env->GetStringUTFChars(v, &cp), sl);
    if (!attach) {
        _obj = gs_env->NewStringUTF(_str.c_str());
    } else {
        _obj = v;
    }
}

JString::JString(const std::string& str): _str(str) {
    _obj = gs_env->NewStringUTF(str.c_str());
}

JString::~JString() {
    gs_env->ReleaseStringChars(_obj, nullptr);
}

JVariant::JVariant(bool v)
: _typ(JVariant::BOOLEAN) {
    _v.z = v ? 1 : 0;
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
    _v.j = v;
}

JVariant::JVariant(jfloat v)
: _typ(JVariant::FLOAT) {
    _v.f = v;
}

JVariant::JVariant(jdouble v)
: _typ(JVariant::DOUBLE) {
    _v.d = v;
}

JVariant::JVariant(jobject v)
: _typ(JVariant::OBJECT) {
    _vo = make_shared<JObject>(v, true);
    _v.l = *_vo;
}

JVariant::JVariant(jstring v)
: _typ(JVariant::STRING) {
    _vs = make_shared<JString>(v);
    _v.l = v;
}

JVariant::JVariant(const string& v)
: _typ(JVariant::STRING) {
    _vs = make_shared<JString>(v);
    _v.l = (jstring)*_vs;
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
    if (!name.empty()) {
        _clazz = gs_env->FindClass(_clazzname.c_str());
    }
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
    string sig = "(" + join(ps.begin(), ps.end(), ";") + ")" + returntyp;
    return sig;
}

JVariant JMethod::invoke(const vector<const JVariant*>& args) {
    string sig = signature(args);
    vector<jvalue> jargs;
    for (auto& e: args) {
        jargs.emplace_back(e->jv());
    }
    if (is_static) {
        auto mid = gs_env->GetStaticMethodID(_cls._clazz, name.c_str(), sig.c_str());
        if (!mid)
            throw "没有找到函数 " + name + sig;
        if (returntyp == jt::Boolean) {
            return gs_env->CallStaticBooleanMethodA(_cls._clazz, mid, (jvalue*)&jargs);
        } else if (returntyp == jt::Byte) {
            return gs_env->CallStaticByteMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Char) {
            return gs_env->CallStaticCharMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Short) {
            return gs_env->CallStaticShortMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Int) {
            return gs_env->CallStaticIntMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Long) {
            return gs_env->CallStaticLongMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Float) {
            return gs_env->CallStaticFloatMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::Double) {
            return gs_env->CallStaticDoubleMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else if (returntyp == jt::String) {
            return (jstring)gs_env->CallStaticObjectMethodA(_cls._clazz, mid, (jvalue*)&args);
        } else {
            return gs_env->CallStaticObjectMethodA(_cls._clazz, mid, (jvalue*)&args);
        }
    } else {
        auto mid = gs_env->GetMethodID(_cls._clazz, name.c_str(), sig.c_str());
        if (!mid)
            throw "没有找到函数 " + name + sig;
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