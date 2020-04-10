#include "ajni.h"
#include "test.h"
#include "android.h"
#include "inspect.h"

AJNI_USE

class BlockView : public android::View {
public:
    BlockView() : android::View("com/wybosys/ajni/BlockView") {
        construct.argtyps = { jt::Context, jt::AttributeSet };
    }
};

void AJNI_FUNC(AJni_Test)(JNIEnv *env, jobject self, jobject activity) {
    AJNI_LOGD("启动测试");

    JEntry<BlockView> blockview(activity, jnull);
    blockview->setBackgroundColor(blockview, (int)0xFFFF0000);
}
