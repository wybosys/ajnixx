#ifndef _AJNI_ANDROID_H
#define _AJNI_ANDROID_H

AJNI_BEGIN_NS(jt)
extern const string Context;
extern const string AttributeSet;
extern const string View;
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

AJNI_END_NS

#endif