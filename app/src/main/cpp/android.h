#ifndef _AJNI_ANDROID_H
#define _AJNI_ANDROID_H

AJNI_BEGIN_NS(android)

class Activity: public JClass {
public:

    Activity(const JClassPath&);

};

AJNI_END_NS

#endif