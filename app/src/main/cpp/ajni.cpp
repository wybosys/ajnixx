#include "ajni.h"
#include "stlext.h"
#include "jre.h"

AJNI_BEGIN

namespace jt {
    const string Class = "Ljava/lang/Class;";
    const string String = "Ljava/lang/String;";
    const string Object = "Ljava/lang/Object;";
    const string Boolean = "Z";
    const string Byte = "B";
    const string Char = "C";
    const string Short = "S";
    const string Int = "I";
    const string Long = "J";
    const string Float = "F";
    const string Double = "D";
}

static JavaVM *gs_vm = nullptr;
static JNIEnv *gs_env = nullptr;

JNIEnv *Env() {
    return gs_env;
}

JObject::JObject(jobject obj, bool attach)
: _obj(obj) {
    if (!attach && _obj) {
        _obj = gs_env->NewLocalRef(_obj);
    }
}

JObject::JObject(ajni::JObject &r)
: _obj(r._obj)
{
    if (_obj) {
        _obj = gs_env->NewLocalRef(_obj);
    }
}

JObject::~JObject() {
    if (_obj) {
        gs_env->DeleteLocalRef(_obj);
        _obj = nullptr;
    }
}

JString::JString(jstring v, bool attach) {
    jboolean cp = false;
    size_t sl = gs_env->GetStringUTFLength(v);
    const char* cs = gs_env->GetStringUTFChars(v, &cp);
    _str = string(cs, sl);
    gs_env->ReleaseStringUTFChars(v, cs);

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
    if (_obj) {
        gs_env->DeleteLocalRef(_obj);
        _obj = nullptr;
    }
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

JVariant JMethod::operator ()() const {
    return invoke(vector<const JVariant*>());
}

JVariant JMethod::operator ()(JVariant const& v) const {
    return invoke(vector<const JVariant*>({
        &v
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1) const {
    return invoke(vector<const JVariant*>({
        &v, &v1
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2) const {
    return invoke(vector<const JVariant*>({
        &v, &v1, &v2
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3) const {
    return invoke(vector<const JVariant*>({
        &v, &v1, &v2, &v3
    }));
}

JVariant JMethod::operator ()(const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3, const JVariant& v4) const {
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

JVariant JMethod::operator ()(jobject obj) const {
    return invoke(obj, vector<const JVariant*>());
}

JVariant JMethod::operator ()(jobject obj, JVariant const& v) const {
    return invoke(obj, vector<const JVariant*>({
        &v
    }));
}

JVariant JMethod::operator ()(jobject obj, const JVariant& v, const JVariant& v1) const {
    return invoke(obj, vector<const JVariant*>({
        &v, &v1
    }));
}

JVariant JMethod::operator ()(jobject obj, const JVariant& v, const JVariant& v1, const JVariant& v2) const {
    return invoke(obj, vector<const JVariant*>({
        &v, &v1, &v2
    }));
}

JVariant JMethod::operator ()(jobject obj, const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3) const {
    return invoke(obj, vector<const JVariant*>({
        &v, &v1, &v2, &v3
    }));
}

JVariant JMethod::operator ()(jobject obj, const JVariant& v, const JVariant& v1, const JVariant& v2, const JVariant& v3, const JVariant& v4) const {
    return invoke(obj, vector<const JVariant*>({
        &v, &v1, &v2, &v3, &v4
    }));
}

JVariant JMethod::invoke(const vector<const JVariant*>& args) const {
    AJNI_CHECKEXCEPTION

    string sig = signature(args);
    vector<jvalue> jargs;
    for (auto& e: args) {
        jargs.emplace_back(e->jv());
    }
    const jvalue* jpargs = jargs.size() ? (const jvalue*)&jargs[0] : nullptr;

    if (is_static) {
        auto mid = gs_env->GetStaticMethodID(_cls.clazz(), name.c_str(), sig.c_str());
        if (!mid)
            throw "没有找到函数 " + name + sig;
        if (returntyp == jt::Boolean) {
            return gs_env->CallStaticBooleanMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Byte) {
            return gs_env->CallStaticByteMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Char) {
            return gs_env->CallStaticCharMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Short) {
            return gs_env->CallStaticShortMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Int) {
            return gs_env->CallStaticIntMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Long) {
            return gs_env->CallStaticLongMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Float) {
            return gs_env->CallStaticFloatMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::Double) {
            return gs_env->CallStaticDoubleMethodA(_cls.clazz(), mid, jpargs);
        } else if (returntyp == jt::String) {
            return (jstring)gs_env->CallStaticObjectMethodA(_cls.clazz(), mid, jpargs);
        } else {
            return gs_env->CallStaticObjectMethodA(_cls.clazz(), mid, jpargs);
        }
    }
    return JVariant();
}

JVariant JMethod::invoke(jobject obj, const vector<const JVariant*>& args) const {
    AJNI_CHECKEXCEPTION

    string sig = signature(args);
    vector<jvalue> jargs;
    for (auto& e: args) {
        jargs.emplace_back(e->jv());
    }
    const jvalue* jpargs = jargs.size() ? (const jvalue*)&jargs[0] : nullptr;

    if (!is_static) {
        auto mid = gs_env->GetMethodID(_cls.clazz(), name.c_str(), sig.c_str());
        if (!mid)
            throw "没有找到函数 " + name + sig;
        if (returntyp == jt::Boolean) {
            return gs_env->CallBooleanMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Byte) {
            return gs_env->CallByteMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Char) {
            return gs_env->CallCharMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Short) {
            return gs_env->CallShortMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Int) {
            return gs_env->CallIntMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Long) {
            return gs_env->CallLongMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Float) {
            return gs_env->CallFloatMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::Double) {
            return gs_env->CallDoubleMethodA(obj, mid, jpargs);
        } else if (returntyp == jt::String) {
            return (jstring) gs_env->CallObjectMethodA(obj, mid, jpargs);
        } else {
            return gs_env->CallObjectMethodA(obj, mid, jpargs);
        }
    }
    return JVariant();
}

JClass::JClass(const JClassPath &path)
: _clazzpath(path), _clazz(nullptr) {
    if (!path.empty()) {
        _clazz = gs_env->FindClass(path.c_str());
    }
}

JClass::~JClass() {
    if (_clazz) {
        gs_env->DeleteLocalRef(_clazz);
        _clazz = nullptr;
    }
}

ExceptionGuard::~ExceptionGuard() {
    if (!gs_env->ExceptionCheck())
        return;
    jthrowable exp = gs_env->ExceptionOccurred();
    gs_env->ExceptionClear();

    JEntry<jre::Throwable> obj(exp);
    string msg = obj->toString(obj);
    AJNI_LOGE("捕获JNI异常 %s", msg.c_str());
}

AJNI_END

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    ajni::gs_vm = vm;
    vm->GetEnv((void**)&ajni::gs_env, JNI_VERSION_1_4);
    vm->AttachCurrentThread(&ajni::gs_env, nullptr);
    return JNI_VERSION_1_4;
}

void AJNI_FUNC(AJni_Init)(JNIEnv *env, jobject self) {
    // pass
}

void AJNI_FUNC(AJni_Main)(JNIEnv *env, jobject self) {
    // pass
}