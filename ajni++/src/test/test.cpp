#include <ajni++.hpp>

USE_AJNI;

AJNI_IMP_LOADED({});

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    return JString("AJNI++ STRING").asReturn();
}
