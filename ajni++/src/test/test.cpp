#include <ajni++.hpp>
#include <kotlin.hpp>
#include <sstream>

USE_AJNI;

AJNI_IMP_LOADED({})
AJNI_IMP_UNLOADED({})

class Info : public kotlin::JClass
{
public:

    Info() : kotlin::JClass(CLASSPATH)
    {}

    static const JClassPath CLASSPATH;
};

const JClassPath Info::CLASSPATH = "com/nnt/ajnixx/Info";

class Test : public kotlin::JClass
{
public:

    Test() : kotlin::JClass(CLASSPATH),
    test0(*this), ftest0(*this),
    Test0(*this), fTest0(*this),
    finfo(*this)
    {
        test0.name = "test0";
        test0.sreturn = TypeSignature::STRING;

        ftest0.name = "test0";
        ftest0.stype = TypeSignature::STRING;

        Test0.name = "Test0";
        Test0.sreturn = TypeSignature::STRING;

        fTest0.name = "Test0";
        fTest0.stype = TypeSignature::STRING;

        finfo.name = "info";
        finfo.sreturn = Info::CLASSPATH;
    }

    JMemberMethod test0;
    JMemberField ftest0;
    kotlin::JStaticMethod Test0;
    JStaticField fTest0;
    JMemberMethod finfo;

    static const JClassPath CLASSPATH;
};

const JClassPath Test::CLASSPATH = "com/nnt/ajnixx/Test";

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

    // 获得数据对象
    JEntry<Info> obj_info(obj->finfo(obj));
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ::std::ostringstream oss;
    Test0(oss);
    return JString(oss.str()).asReturn();
}
