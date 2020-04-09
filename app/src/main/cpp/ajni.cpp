#include "ajni.h"

AJNI_BEGIN

static JNIEnv *gs_env = nullptr;

AJNI_END

void AJNI_FUNC(AJni_Init)(JNIEnv *env, jobject self) {
    ajni::gs_env = env;
}