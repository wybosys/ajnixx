#ifndef _AJNI_ANDROID_H
#define _AJNI_ANDROID_H

AJNI_BEGIN_NS(jt)
extern const string Context;
extern const string AttributeSet;
AJNI_END_NS

AJNI_BEGIN_NS(android)

class Activity: public JClass {
public:

    Activity(const JClassPath&);

};

class View: public JClass {
public:

    View(const JClassPath&);

    JMethod setBackgroundColor;
};

AJNI_END_NS

#endif