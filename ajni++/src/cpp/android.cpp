#include "core.hpp"
#include "ajni++.hpp"
#include "android.hpp"

AJNI_BEGIN_NS(android)

namespace TypeSignature
{
    const JTypeSignature CONTEXT = "Landroid/content/Context;";
    const JTypeSignature ATTRIBUTESET = "Landroid/util/AttributeSet;";
    const JTypeSignature VIEW = "Landroid/view/View;";
    const JTypeSignature VIEWGROUP = "Landroid/view/ViewGroup;";
    const JTypeSignature VIEWGROUP_LAYOUTPARAMS = "Landroid/view/ViewGroup$LayoutParams;";
} // namespace TypeSignature

Activity::Activity(const JClassPath &path)
    : JClass(path),
      findViewById(*this)
{
    findViewById.name = "findViewById";
    findViewById.sreturn = TypeSignature::VIEW;
}

View::View(const JClassPath &path)
    : JClass(path),
      setBackgroundColor(*this)
{
    setBackgroundColor.name = "setBackgroundColor";
    setBackgroundColor.sreturn = ajni::TypeSignature::VOID;
}

ViewGroup::ViewGroup(JClassPath const& path)
    : View(path),
      addView(*this)
{
    addView.name = "addView";
    addView.sreturn = ajni::TypeSignature::VOID;
    addView.sargs = {TypeSignature::VIEW, TypeSignature::VIEWGROUP_LAYOUTPARAMS};
}

ViewGroup::LayoutParams::LayoutParams(const JClassPath &path)
    : JClass(path)
{
    construct.sargs = {ajni::TypeSignature::INT, ajni::TypeSignature::INT};
}

ConstraintLayout::ConstraintLayout(const JClassPath &path)
    : ViewGroup(path)
{
}

AJNI_END_NS