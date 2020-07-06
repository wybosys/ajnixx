#include "ajni++.hpp"
#define __AJNI_PRIVATE__
#include "android-prv.hpp"

AJNI_BEGIN

void Logger::Debug(string const& msg)
{
    AJNI_LOGD("%s", msg.c_str());
}

void Logger::Info(string const& msg)
{
    AJNI_LOGI("%s", msg.c_str());
}

void Logger::Warn(string const& msg)
{
    AJNI_LOGW("%s", msg.c_str());
}

void Logger::Error(string const& msg)
{
    AJNI_LOGE("%s", msg.c_str());
}

void Logger::Fatal(string const& msg)
{
    AJNI_LOGF("%s", msg.c_str());
}

AJNI_END

void AJNI_LOGGER_ERROR(::std::string const& msg)
{
    AJNI_NS::Logger::Error(msg);
}
