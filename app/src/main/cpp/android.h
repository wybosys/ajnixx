#ifndef _AJNI_ANDROID_H
#define _AJNI_ANDROID_H

AJNI_BEGIN_NS(jt)
extern const string Context;
extern const string AttributeSet;
extern const string View;
extern const string ViewGroup;
extern const string ViewGroupLayoutParams;
AJNI_END_NS

AJNI_BEGIN_NS(android)

class Activity: public JClass {
public:

    Activity(const JClassPath& = "androidx/appcompat/app/AppCompatActivity");

    JMethod findViewById;
};

class View: public JClass {
public:

    View(const JClassPath& = "android/view/View");

    JMethod setBackgroundColor;
};

class ViewGroup: public View {
public:

    class LayoutParams: public JClass {
    public:
        LayoutParams(const JClassPath& = "android/view/ViewGroup$LayoutParams");

        enum {
            FILL_PARENT = -1,
            MATCH_PARENT = -1,
            WRAP_CONTENT = -2
        };
    };

    ViewGroup(const JClassPath& = "android/view/ViewGroup");

    JMethod addView;
};

class ConstraintLayout: public ViewGroup {
public:

    ConstraintLayout(const JClassPath& = "androidx/constraintlayout/widget/ConstraintLayout");
};

AJNI_END_NS

#endif