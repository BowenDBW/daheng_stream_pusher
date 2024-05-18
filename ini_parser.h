#pragma once

#include <QDateTime>
#include <QTextEdit>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>

enum LogLevel {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
};

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
    static std::vector<IniNode>::size_type setValue(std::string root, std::string key, std::string value);

    static bool hasInit;

    static std::string CONFIG_PATH;

    static int logLevel;

    static std::string logPath;

    static QTextEdit* logEdit;

    static std::string& trimString(std::string& str);

    /// <summary>
    ///     INI文件内容的存储变量
    /// </summary>
    static std::map<std::string, SubNode>* map_ini;

    /// <summary>
    ///     读取INI文件
    /// </summary>
    static void readIni();

    /// <summary>
    ///     获取INI文件的结点数
    /// </summary>
    /// <returns></returns>
    static std::vector<IniNode>::size_type getSize();

    /// <summary>
    ///     写入INI文件
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    static int writeIni(std::string path);

    /// <summary>
    ///     清空
    /// </summary>
    static void clear();
};

//INI文件结点存储结构
class IniNode
{
public:
    IniNode(std::string root, std::string key, std::string value)
    {
        this->root = root;
        this->key = key;
        this->value = value;
    }
    std::string root;
    std::string key;
    std::string value;
};

//键值对结构体
class SubNode
{
public:
    static void InsertElement(std::string key, std::string value)
    {
        sub_node.insert(std::pair<std::string, std::string>(key, value));
    }

    static std::map<std::string, std::string> sub_node;
};