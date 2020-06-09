#include <ajni++.hpp>
#include <sstream>

USE_AJNI;

AJNI_IMP_LOADED({})

class Test : public JClass
{
public:

    Test() : JClass(CLASSPATH) {}

    static const string CLASSPATH;
};

const string Test::CLASSPATH = "com/nnt/ajnixx/Test";

void Test0(::std::ostringstream& oss)
{
    auto cls = JContext::shared().register_class<Test>();
    if (cls) {
        oss << "没找到 Test 类" << endl;
        return;
    } else {
        oss << "找到 Test 类" << endl;
    }
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ::std::ostringstream oss;
    Test0(oss);
    return JString(oss.str()).asReturn();
}
