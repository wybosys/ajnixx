#include <ajni++.hpp>
#include <sstream>

USE_AJNI;

AJNI_IMP_LOADED({})
AJNI_IMP_UNLOADED({})

class Test : public JClass
{
public:

    Test() : JClass(CLASSPATH),
    test0(*this), ftest0(*this), fTest0(*this)
    {
        test0.name = "test0";
        test0.sreturn = TypeSignature::STRING;

        ftest0.name = "test0";
        ftest0.stype = TypeSignature::STRING;

        fTest0.name = "Test0";
        fTest0.sreturn = TypeSignature::STRING;
    }

    JMemberMethod test0;
    JField ftest0;
    JStaticMethod fTest0;

    static const string CLASSPATH;
};

const string Test::CLASSPATH = "com/nnt/ajnixx/Test";

void Test0(::std::ostringstream& oss)
{
    auto cls = Env.context().register_class<Test>();
    if (!cls) {
        oss << "没找到 Test 类" << endl;
        return;
    } else {
        oss << "找到 Test 类" << endl;
    }

    // 实例化对象
    JEntry<Test> obj(cls->construct());
    oss << obj->test0(obj) << endl;
    oss << obj->ftest0(obj) << endl;
    oss << obj->fTest0() << endl;
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ::std::ostringstream oss;
    Test0(oss);
    return JString(oss.str()).asReturn();
}
