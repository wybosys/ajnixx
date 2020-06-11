#include "core.hpp"
#include "ajni++.hpp"
#include "jre.hpp"
#include "variant.hpp"

#include <cross/cross.hpp>
#include <cross/str.hpp>

AJNI_BEGIN

const jobject jnull = nullptr;

static JavaVM *gs_vm = nullptr;
static JNIEnv *gs_env = nullptr;
static thread_local bool tls_ismain = false;
static thread_local JNIEnv *tls_env = nullptr;

JEnv Env;

class JEnvPrivate
{
public:

    void clear()
    {
        ctx.clear();
    }

    JContext ctx;
};

JEnv::JEnv()
{
    NNT_CLASS_CONSTRUCT();
}

JEnv::~JEnv()
{
    NNT_CLASS_DESTORY();
}

JContext& JEnv::context()
{
    return d_ptr->ctx;
}

void JEnv::BindVM(JavaVM *vm, JNIEnv *env)
{
    Logger::Info("启动AJNI++环境");

    ajni::gs_vm = vm;

    if (!env) {
        vm->GetEnv((void **) &env, JNI_VERSION_1_4);
        vm->AttachCurrentThread(&env, nullptr);
    }

    ajni::gs_env = ajni::tls_env = env;
    ajni::tls_ismain = true;
}

void JEnv::UnbindVM()
{
    ajni::gs_vm = nullptr;
    ajni::tls_env = nullptr;
    d_ptr->clear();

    Logger::Info("释放AJNI++环境");
}

jclass JEnv::FindClass(string const& str)
{
    return tls_env->FindClass(str.c_str());
}

bool JEnv::IsAssignableFrom(jclass l, jclass r)
{
    return tls_env->IsAssignableFrom(l, r);
}

bool JEnv::IsInstanceOf(jobject obj, jclass clz)
{
    return tls_env->IsInstanceOf(obj, clz);
}

jfieldID JEnv::GetStaticFieldID(jclass cls, const string &name, const string &typ)
{
    return tls_env->GetStaticFieldID(cls, name.c_str(), typ.c_str());
}

jboolean JEnv::GetStaticBooleanField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticBooleanField(cls, id);
}

jbyte JEnv::GetStaticByteField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticByteField(cls, id);
}

jobject JEnv::GetStaticObjectField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticObjectField(cls, id);
}

jchar JEnv::GetStaticCharField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticCharField(cls, id);
}

jshort JEnv::GetStaticShortField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticShortField(cls, id);
}

jint JEnv::GetStaticIntField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticIntField(cls, id);
}

jlong JEnv::GetStaticLongField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticLongField(cls, id);
}

jfloat JEnv::GetStaticFloatField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticFloatField(cls, id);
}

jdouble JEnv::GetStaticDoubleField(jclass cls, jfieldID id)
{
    return tls_env->GetStaticDoubleField(cls, id);
}

void JEnv::SetStaticObjectField(jclass cls, jfieldID id, jobject v)
{
    tls_env->SetStaticObjectField(cls, id, v);
}

void JEnv::SetStaticBooleanField(jclass cls, jfieldID id, jboolean v)
{
    tls_env->SetStaticBooleanField(cls, id, v);
}

void JEnv::SetStaticByteField(jclass cls, jfieldID id, jbyte v)
{
    tls_env->SetStaticByteField(cls, id, v);
}

void JEnv::SetStaticCharField(jclass cls, jfieldID id, jchar v)
{
    tls_env->SetStaticCharField(cls, id, v);
}

void JEnv::SetStaticShortField(jclass cls, jfieldID id, jshort v)
{
    tls_env->SetStaticShortField(cls, id, v);
}

void JEnv::SetStaticIntField(jclass cls, jfieldID id, jint v)
{
    tls_env->SetStaticIntField(cls, id, v);
}

void JEnv::SetStaticLongField(jclass cls, jfieldID id, jlong v)
{
    tls_env->SetStaticLongField(cls, id, v);
}

void JEnv::SetStaticFloatField(jclass cls, jfieldID id, jfloat v)
{
    tls_env->SetStaticFloatField(cls, id, v);
}

void JEnv::SetStaticDoubleField(jclass cls, jfieldID id, jdouble v)
{
    tls_env->SetStaticDoubleField(cls, id, v);
}

jfieldID JEnv::GetFieldID(jclass cls, string const &name, string const &sig)
{
    return tls_env->GetFieldID(cls, name.c_str(), sig.c_str());
}

jobject JEnv::GetObjectField(jobject obj, jfieldID fid)
{
    return tls_env->GetObjectField(obj, fid);
}

jboolean JEnv::GetBooleanField(jobject obj, jfieldID fid)
{
    return tls_env->GetBooleanField(obj, fid);
}

jbyte JEnv::GetByteField(jobject obj, jfieldID fid)
{
    return tls_env->GetByteField(obj, fid);
}

jchar JEnv::GetCharField(jobject obj, jfieldID fid)
{
    return tls_env->GetCharField(obj, fid);
}

jshort JEnv::GetShortField(jobject obj, jfieldID fid)
{
    return tls_env->GetShortField(obj, fid);
}

jint JEnv::GetIntField(jobject obj, jfieldID fid)
{
    return tls_env->GetIntField(obj, fid);
}

jlong JEnv::GetLongField(jobject obj, jfieldID fid)
{
    return tls_env->GetLongField(obj, fid);
}

jfloat JEnv::GetFloatField(jobject obj, jfieldID fid)
{
    return tls_env->GetFloatField(obj, fid);
}

jdouble JEnv::GetDoubleField(jobject obj, jfieldID fid)
{
    return tls_env->GetDoubleField(obj, fid);
}

void JEnv::SetObjectField(jobject obj, jfieldID id, jobject v)
{
    tls_env->SetObjectField(obj, id, v);
}

void JEnv::SetBooleanField(jobject obj, jfieldID id, jboolean v)
{
    tls_env->SetBooleanField(obj, id, v);
}

void JEnv::SetByteField(jobject obj, jfieldID id, jbyte v)
{
    tls_env->SetByteField(obj, id, v);
}

void JEnv::SetCharField(jobject obj, jfieldID id, jchar v)
{
    tls_env->SetCharField(obj, id, v);
}

void JEnv::SetShortField(jobject obj, jfieldID id, jshort v)
{
    tls_env->SetShortField(obj, id, v);
}

void JEnv::SetIntField(jobject obj, jfieldID id, jint v)
{
    tls_env->SetIntField(obj, id, v);
}

void JEnv::SetLongField(jobject obj, jfieldID id, jlong v)
{
    tls_env->SetLongField(obj, id, v);
}

void JEnv::SetFloatField(jobject obj, jfieldID id, jfloat v)
{
    tls_env->SetFloatField(obj, id, v);
}

void JEnv::SetDoubleField(jobject obj, jfieldID id, jdouble v)
{
    tls_env->SetDoubleField(obj, id, v);
}


jmethodID JEnv::GetMethodID(jclass cls, string const &name, string const &sig)
{
    return tls_env->GetMethodID(cls, name.c_str(), sig.c_str());
}

jmethodID JEnv::GetStaticMethodID(jclass cls, string const& name, string const& sig)
{
    return tls_env->GetStaticMethodID(cls, name.c_str(), sig.c_str());
}

#define JENV_IMPL_EXPAND(func, pre)                                                                                 \
    switch (vals.size())                                                                                            \
    {                                                                                                               \
    case 0:                                                                                                         \
        return tls_env->func(pre);                                                                                  \
    case 1:                                                                                                         \
        return tls_env->func(pre, vals[0]);                                                                         \
    case 2:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1]);                                                                \
    case 3:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2]);                                                       \
    case 4:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3]);                                              \
    case 5:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3], vals[4]);                                     \
    case 6:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);                            \
    case 7:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6]);                   \
    case 8:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7]);          \
    case 9:                                                                                                         \
        return tls_env->func(pre, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7], vals[8]); \
    default:                                                                                                        \
        break;                                                                                                      \
    }

jobject JEnv::NewObject(jclass cls, jmethodID id, JValues const &vals)
{
    JENV_IMPL_EXPAND(NewObject, cls NNT_COMMA id);
    return nullptr;
}

jclass JEnv::GetObjectClass(jobject obj)
{
    return tls_env->GetObjectClass(obj);
}

jboolean JEnv::CallStaticBooleanMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticBooleanMethod, cls NNT_COMMA id);
    return false;
}

jbyte JEnv::CallStaticByteMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticByteMethod, cls NNT_COMMA id);
    return 0;
}

jchar JEnv::CallStaticCharMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticCharMethod, cls NNT_COMMA id);
    return 0;
}

jshort JEnv::CallStaticShortMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticShortMethod, cls NNT_COMMA id);
    return 0;
}

jint JEnv::CallStaticIntMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticIntMethod, cls NNT_COMMA id);
    return 0;
}

jlong JEnv::CallStaticLongMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticLongMethod, cls NNT_COMMA id);
    return 0;
}

jfloat JEnv::CallStaticFloatMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticFloatMethod, cls NNT_COMMA id);
    return 0;
}

jdouble JEnv::CallStaticDoubleMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticDoubleMethod, cls NNT_COMMA id);
    return 0;
}

jobject JEnv::CallStaticObjectMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticObjectMethod, cls NNT_COMMA id);
    return 0;
}

void JEnv::CallStaticVoidMethod(jclass cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallStaticVoidMethod, cls NNT_COMMA id);
}

jboolean JEnv::CallBooleanMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallBooleanMethod, cls NNT_COMMA id);
    return false;
}

jbyte JEnv::CallByteMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallByteMethod, cls NNT_COMMA id);
    return 0;
}

jchar JEnv::CallCharMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallCharMethod, cls NNT_COMMA id);
    return 0;
}

jshort JEnv::CallShortMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallShortMethod, cls NNT_COMMA id);
    return 0;
}

jint JEnv::CallIntMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallIntMethod, cls NNT_COMMA id);
    return 0;
}

jlong JEnv::CallLongMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallLongMethod, cls NNT_COMMA id);
    return 0;
}

jfloat JEnv::CallFloatMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallFloatMethod, cls NNT_COMMA id);
    return 0;
}

jdouble JEnv::CallDoubleMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallDoubleMethod, cls NNT_COMMA id);
    return 0;
}

jobject JEnv::CallObjectMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallObjectMethod, cls NNT_COMMA id);
    return nullptr;
}

void JEnv::CallVoidMethod(jobject cls, jmethodID id, JValues const& vals)
{
    JENV_IMPL_EXPAND(CallVoidMethod, cls NNT_COMMA id);
}

jobject JEnv::NewLocalRef(jobject obj)
{
    return tls_env->NewLocalRef(obj);
}

void JEnv::DeleteLocalRef(jobject obj)
{
    tls_env->DeleteLocalRef(obj);
}

jobject JEnv::NewGlobalRef(jobject obj)
{
    return tls_env->NewGlobalRef(obj);
}

void JEnv::DeleteGlobalRef(jobject obj)
{
    return tls_env->DeleteGlobalRef(obj);
}

jsize JEnv::GetStringUTFLength(jstring jstr)
{
    return tls_env->GetStringUTFLength(jstr);
}

string JEnv::GetStringUTFChars(jstring jstr)
{
    jboolean cp = false;
    char const* cs = tls_env->GetStringUTFChars(jstr, &cp);
    if (cs) {
        string r = cs;
        if (cp)
            tls_env->ReleaseStringUTFChars(jstr, cs);
        return r;
    }
    return "";
}

jstring JEnv::NewStringUTF(string const& str)
{
    return tls_env->NewStringUTF(str.c_str());
}

namespace TypeSignature
{
    const JTypeSignature CLASS = "Ljava/lang/Class;";
    const JTypeSignature STRING = "Ljava/lang/String;";
    const JTypeSignature OBJECT = "Ljava/lang/Object;";
    const JTypeSignature BOOLEAN = "Z";
    const JTypeSignature BYTE = "B";
    const JTypeSignature CHAR = "C";
    const JTypeSignature SHORT = "S";
    const JTypeSignature INT = "I";
    const JTypeSignature LONG = "J";
    const JTypeSignature FLOAT = "F";
    const JTypeSignature DOUBLE = "D";
    const JTypeSignature VOID = "V";
    const JTypeSignature BYTEARRAY = "[B";
} // namespace TypeSignature

ExceptionGuard::~ExceptionGuard()
{
    if (!gs_env->ExceptionCheck())
        return;
    jthrowable err = gs_env->ExceptionOccurred();
    gs_env->ExceptionClear();
    if (_print) {
        JEntry<jre::Throwable> obj(make_shared<JObject>(err));
        string msg = *obj->toString(obj);
        Logger::Error("捕获JNI异常 " + msg);
    }
}

void Logger::Debug(string const& msg)
{
    AJNI_LOGD("%s", msg.c_str());
}

void Logger::Info(string const& msg)
{
    AJNI_LOGI("%s", msg.c_str());
}

void Logger::Warn(string const& msg)
{
    AJNI_LOGW("%s", msg.c_str());
}

void Logger::Error(string const& msg)
{
    AJNI_LOGE("%s", msg.c_str());
}

void Logger::Fatal(string const& msg)
{
    AJNI_LOGF("%s", msg.c_str());
}

JClassName::JClassName(JClassPath const& cp)
{
    *this = ::CROSS_NS::replace((string const&)cp, "/", ".");
}

JTypeSignature::JTypeSignature(JClassPath const& cp)
{
    *this = cp;
}

JTypeSignature& JTypeSignature::operator = (JClassPath const& cp)
{
    if (!cp.size()) {
        *this = "V";
        goto LABEL_RETURN;
    }
    if (cp[0] == 'L') {
        *this = (string const&)cp;
        goto LABEL_RETURN;
    }
    if (cp.find('/') != string::npos) {
        *this = "L" + (string const&)cp + ";";
        goto LABEL_RETURN;
    }
    *this = (string const&)cp;
    LABEL_RETURN:
    return *this;
}

AJNI_END
