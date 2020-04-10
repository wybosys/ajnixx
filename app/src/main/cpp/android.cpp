#include "ajni.h"
#include "android.h"

AJNI_BEGIN_NS(jt)
const string Context = "Landroid/content/Context;";
const string AttributeSet = "Landroid/util/AttributeSet;";
const string View = "Landroid/view/View;";
const string ViewGroup = "Landroid/view/ViewGroup;";
const string ViewGroupLayoutParams = "Landroid/view/ViewGroup$LayoutParams;";
AJNI_END_NS

AJNI_BEGIN_NS(android)

Activity::Activity(const JClassPath &path)
: JClass(path),
findViewById(*this)
{
    findViewById.name = "findViewById";
    findViewById.returntyp = jt::View;
}

View::View(const JClassPath &path)
: JClass(path),
setBackgroundColor(*this)
{
    setBackgroundColor.name = "setBackgroundColor";
    setBackgroundColor.returntyp = jt::Void;
}

ViewGroup::ViewGroup(const JClassPath &path)
: View(path),
addView(*this)
{
    addView.name = "addView";
    addView.returntyp = jt::Void;
    addView.argtyps = { jt::View, jt::ViewGroupLayoutParams };
}

ViewGroup::LayoutParams::LayoutParams(const JClassPath &path)
: JClass(path)
{
    construct.argtyps = { jt::Int, jt::Int };
}

ConstraintLayout::ConstraintLayout(const JClassPath &path)
: ViewGroup(path)
{

}

AJNI_END_NS