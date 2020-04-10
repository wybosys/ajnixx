#include "ajni.h"
#include "test.h"
#include "android.h"
#include "inspect.h"

AJNI_USE

class R_id : public JClass
{
public:

    R_id(): JClass("com/wybosys/ajni/R$id"),
    main(*this)
    {
        main.name = "main";
        main.is_static = true;
        main.typ = jt::Int;
    }

    JField main;
};

class BlockView : public android::View {
public:
    BlockView() : android::View("com/wybosys/ajni/BlockView") {
        construct.argtyps = { jt::Context, jt::AttributeSet };
    }
};

void AJNI_FUNC(AJni_Test)(JNIEnv *env, jobject self, jobject activity) {
    AJNI_LOGD("启动测试");

    JEntry<BlockView> bv(activity, jnull);
    bv->setBackgroundColor(bv, (int)0xFFFF0000);

    R_id rid;
    auto act = JEntry<android::Activity>::Attach(activity);
    auto lyt = JEntry<android::ConstraintLayout>::Attach((*act)->findViewById(*act, (int)rid.main()));

    JEntry<android::ViewGroup::LayoutParams> lp(100, 100);
    (*lyt)->addView(*lyt, (jobject)bv, (jobject)lp);
}
