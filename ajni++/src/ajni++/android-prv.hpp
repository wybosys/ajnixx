#ifndef __AJNI_ANDROID_PRIVATE_H_INCLUDED
#define __AJNI_ANDROID_PRIVATE_H_INCLUDED

#ifndef __AJNI_PRIVATE__
#error "禁止在外部引用该文件"
#endif

#define _AJNI_LOG_IDR "log@ajni++"
#define AJNI_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGI(...) __android_log_print(ANDROID_LOG_INFO, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGW(...) __android_log_print(ANDROID_LOG_WARN, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, _AJNI_LOG_IDR, __VA_ARGS__)
#define AJNI_LOGF(...) __android_log_print(ANDROID_LOG_FATAL, _AJNI_LOG_IDR, __VA_ARGS__)

AJNI_BEGIN

// AJNI日志接口
class Logger
{
public:

    static void Debug(string const&);
    static void Info(string const&);
    static void Warn(string const&);
    static void Error(string const&);
    static void Fatal(string const&);
};

AJNI_END

extern void AJNI_LOGGER_ERROR(::std::string const&);

#endif
