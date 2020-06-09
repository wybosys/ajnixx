#include <ajni++.hpp>

USE_AJNI;

AJNI_IMP_LOADED({});

AJNI_API(jstring) AJNI_FUNC(Test_00024Companion_Test)(JNIEnv *env)
{
    return JString("AJNI++ STRING").asReturn();
}
