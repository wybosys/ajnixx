#include <ajni++/ajni++.hpp>
#include <sstream>

#include <cross/cross.hpp>
#include <cross/timer.hpp>

USE_AJNI;
USE_STL;
USE_CROSS;

AJNI_IMP_LOADED({})
AJNI_IMP_UNLOADED({})

class Info: public kotlin::JClass
{
public:

    Info()
        : kotlin::JClass(CLASSPATH),
          fabc(*this), fcde(*this), fnul(*this),
          proc(*this)
    {
        fabc.name = "abc";
        fabc.stype = TypeSignature::STRING;

        fcde.name = "cde";
        fcde.stype = TypeSignature::INT;

        fnul.name = "nul";
        fnul.stype = TypeSignature::OBJECT;

        proc.name = "proc";
        proc.sreturn = TypeSignature::VOID;
    }

    kotlin::JMemberField fabc, fcde, fnul;
    kotlin::JMemberMethod proc;

    static const JClassPath CLASSPATH;
};

const JClassPath Info::CLASSPATH = "com/nnt/ajnixx/Info";

class Test: public kotlin::JClass
{
public:

    Test()
        : kotlin::JClass(CLASSPATH),
          test0(*this), ftest0(*this),
          Test0(*this), fTest0(*this),
          finfo(*this), fvalueasync(*this), finfoasync(*this), fnullasync(*this)
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

        fvalueasync.name = "value_async";
        fvalueasync.sreturn = TypeSignature::VOID;

        finfoasync.name = "info_async";
        finfoasync.sreturn = TypeSignature::VOID;

        fnullasync.name = "null_async";
        fnullasync.sreturn = TypeSignature::VOID;
    }

    kotlin::JMemberMethod test0;
    kotlin::JMemberField ftest0;
    kotlin::JStaticMethod Test0;
    kotlin::JStaticField fTest0;
    kotlin::JMemberMethod finfo;
    kotlin::JMemberMethod fvalueasync, finfoasync, fnullasync;

    static const JClassPath CLASSPATH;
};

const JClassPath Test::CLASSPATH = "com/nnt/ajnixx/Test";

void Test0(::std::ostringstream &oss)
{
    shared_ptr<JClass> cls;
    cls = make_shared<JClass>("java/lang/Integer");
    if (!cls) {
        oss << "没有找到 Int 类对象" << endl;
    }
    else {
        oss << "找到 Int 类对象" << endl;
    }

    cls = Env.context().register_class<Test>();
    if (!cls) {
        oss << "没找到 Test 类" << endl;
        return;
    }
    else {
        oss << "找到 Test 类" << endl;
    }

    // 实例化对象
    JEntry<Test> obj(*cls->construct());
    oss << obj->test0(obj) << endl;
    oss << obj->ftest0(obj) << endl;
    oss << obj->Test0() << endl;
    oss << obj->fTest0() << endl;

    // 获得数据对象
    JEntry<Info> obj_info(*obj->finfo(obj));
    oss << obj_info->fabc(obj_info) << endl;
    oss << obj_info->fcde(obj_info) << endl;
    oss << obj_info->fnul(obj_info) << endl;
    obj_info->fabc(obj_info, "modified abc");
    oss << obj_info->fabc(obj_info) << endl;

    obj->finfoasync(obj, (JCallback)[&](arg_type const &v0) -> return_type
    {
        ostringstream oss;
        JEntry<Info> info(v0);
        auto v = info->fnul(info);
        oss << "收到异步回调数据 ";
        oss << info->fabc(info);
        oss << " ";
        oss << info->fcde(info);
        oss << info->fnul(info);
        oss << endl;
        Logger::Info(oss.str());
        return nullptr;
    }, "xxxxxxxxxxxxxxx", (JCallback)[&]() -> return_type
    {
        return nullptr;
    });

    // 测试异步
    obj->fvalueasync(obj, (JCallback)[&](arg_type const &v0, arg_type const &v1) -> return_type
    {
        ostringstream oss;
        oss << "收到异步回调数据 " << v0 << " " << v1 << endl;
        Logger::Info(oss.str());
        return nullptr;
    });

    obj->fnullasync(obj, (JCallback)[&](arg_type const &v0) -> return_type
    {
        ostringstream oss;
        if (!v0.isnil())
            oss << "应该为nil但是收到不为nil" << endl;
        Logger::Info(oss.str());
        return nullptr;
    });

    // 获得全局函数
    oss << "全局" << endl;
    kotlin::JGlobalMethod mth(Test::CLASSPATH);
    mth.name = "GetInfo";
    mth.sreturn = Info::CLASSPATH;
    JEntry<Info> gobj_info(*mth());
    oss << gobj_info->fabc(gobj_info) << endl;

    kotlin::JGlobalField mtf(Test::CLASSPATH);
    mtf.name = "abc";
    mtf.stype = TypeSignature::INT;
    oss << mtf() << endl;
    mtf(456);
    oss << mtf() << endl;
}

void Test1()
{
    // 测试将Java类拿到手上不释放等待回调再释放
    JEntry<Test> t(*Test().construct());
    JEntry<Info> f(*t->finfo(t));
    // 增加引用计数，防止f被gc掉
    f.grab();
    // 放到timer中过一会再执行
    Timer::SetTimeout(5, [=]()
    {
        f->proc(f, "C++延迟调用Java对象的方法");
        // 释放
        f.drop();
    });
}

AJNI_API(jstring) AJNI_COMPANION_FUNC(Test, Test)(JNIEnv *env, jobject thiz)
{
    ostringstream oss;
    Test0(oss);
    Test1();
    return JString(oss.str()).asReturn();
}
