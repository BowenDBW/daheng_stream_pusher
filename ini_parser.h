#pragma once

#include "ini.hpp"

#include <string>
#include <vector>

//INI文件操作类
class IniParser
{
public:

    static bool getShowHub();

    static void setShowHub(bool show);

    static bool getShowOpencv();

    static void setShowOpencv(bool show);

    static bool getSaveImage();

    static void setSaveImage(bool show);

    static bool getPushStream();

    static void setPushStream(bool show);

    static int getSaveInterval();

    static void setSaveInterval(int interval);

    static std::string getSavePath();

    static void setSavePath(std::string path);

    static std::string getRtmpUrl();

    static void setRtmpUrl(std::string url);

    static std::string getLogPath();

    static void setLogPath(std::string path);

private:

    static mINI::INIFile* file;

    static std::string CONFIG_PATH;

    /// <summary>
    ///     由根结点和键获取值
    /// </summary>
    /// <param name="root"></param>
    /// <param name="key"></param>
    /// <returns></returns>
    static std::string getValue(std::string root, std::string key);

    /// <summary>
    ///     设置根结点和键获取值
    /// </summary>
    /// <param name="root"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    static void setValue(std::string root, std::string key, std::string value);
};