#include <ajni++.hpp>
#include <sstream>

USE_AJNI;

AJNI_IMP_LOADED({})

void Test0(::std::ostringstream& oss)
{
    JClass cls("com/nnt/ajnixx/Test");
    if (!cls.exists()) {
        oss << "没找到 Test 类" << endl;
        return;
    }
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ::std::ostringstream oss;
    Test0(oss);
    return JString(oss.str()).asReturn();
}
