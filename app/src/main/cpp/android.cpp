#include "ajni.h"
#include "android.h"

AJNI_BEGIN_NS(jt)
const string Context = "Landroid/content/Context;";
const string AttributeSet = "Landroid/util/AttributeSet;";
AJNI_END_NS

AJNI_BEGIN_NS(android)

Activity::Activity(const JClassPath &path)
: JClass(path),
findViewById(*this)
{
    findViewById.name = "findViewById";
    findViewById.returntyp = jt::Object;
}

View::View(const JClassPath &path)
: JClass(path),
setBackgroundColor(*this)
{
    setBackgroundColor.name = "setBackgroundColor";
    setBackgroundColor.returntyp = jt::Void;
}

AJNI_END_NS