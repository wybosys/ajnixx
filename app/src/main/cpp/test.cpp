#include "ajni.h"
#include "test.h"
#include "android.h"
#include "inspect.h"

AJNI_USE

class BlockView: public android::Activity {
public:
    BlockView(): android::Activity("com/wybosys/ajni/BlockView") {}
};

void AJNI_FUNC(AJni_Test)(JNIEnv *env, jobject self, jobject activity) {
    AJNI_LOGD("启动测试");

    JEntry<BlockView> blockview;
}
