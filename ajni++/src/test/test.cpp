#include <ajni++.hpp>
#include <kotlin.hpp>
#include <sstream>

USE_AJNI;

AJNI_IMP_LOADED({})
AJNI_IMP_UNLOADED({})

class Test : public kotlin::JClass
{
public:

    Test() : kotlin::JClass(CLASSPATH),
    test0(*this), ftest0(*this),
    Test0(*this), fTest0(*this)
    {
        test0.name = "test0";
        test0.sreturn = TypeSignature::STRING;

        ftest0.name = "test0";
        ftest0.stype = TypeSignature::STRING;

        Test0.name = "Test0";
        Test0.sreturn = TypeSignature::STRING;

        fTest0.name = "Test0";
        fTest0.stype = TypeSignature::STRING;
    }

    JMemberMethod test0;
    JMemberField ftest0;
    kotlin::JStaticMethod Test0;
    JStaticField fTest0;

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
    oss << obj->Test0() << endl;
    oss << obj->fTest0() << endl;
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ::std::ostringstream oss;
    Test0(oss);
    return JString(oss.str()).asReturn();
}
