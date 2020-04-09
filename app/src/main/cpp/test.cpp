#include "ajni.h"
#include "test.h"
#include "android.h"

AJNI_USE

void AJNI_FUNC(AJni_Test)(JNIEnv *env, jobject self, jobject activity) {
    AJNI_LOGD("启动测试");

    JActivity blockview("com/wybosys/ajni/BlockView");
    try {
        blockview.instance();
    } catch (exception e) {
        AJNI_LOGE("实例化blockview失败");
    }
}
