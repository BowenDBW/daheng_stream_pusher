#include "ini_parser.h"

bool IniParser::hasInit = false;
int IniParser::logLevel = LogLevel::INFO;

std::string IniParser::CONFIG_PATH = "config.ini";
std::map<std::string, SubNode>* IniParser::map_ini = new std::map<std::string, SubNode>();

bool IniParser::getShowHub() {
    std::string value = getValue("display", "showHud");
    if (value == "true") {
        return true;
    }
    else {
        return false;
    }
}

void IniParser::setShowHub(bool show) {
    if (show) {
        setValue("display", "showHud", "true");
    }
    else {
        setValue("display", "showHud", "false");
    }
}

bool IniParser::getShowOpencv() {
    std::string value = getValue("display", "showOpencv");
    if (value == "true") {
        return true;
    }
    else {
        return false;
    }
}

void IniParser::setShowOpencv(bool show) {
    if (show) {
        setValue("display", "showOpencv", "true");
    }
    else {
        setValue("display", "showOpencv", "false");
    }
}

bool IniParser::getSaveImage() {
    std::string value = getValue("display", "saveImage");
    if (value == "true") {
        return true;
    }
    else {
        return false;
    }
}

void IniParser::setSaveImage(bool save) {
    if (save) {
        setValue("display", "saveImage", "true");
    }
    else {
        setValue("display", "saveImage", "false");
    }
}

bool IniParser::getPushStream() {
    std::string value = getValue("display", "pushStream");
    if (value == "true") {
        return true;
    }
    else {
        return false;
    }
}

void IniParser::setPushStream(bool push) {
    if (push) {
        setValue("display", "pushStream", "true");
    }
    else {
        setValue("display", "pushStream", "false");
    }
}

int IniParser::getSaveInterval() {
    std::string value = getValue("storage", "saveInterval");
    return std::stoi(value);
}

void IniParser::setSaveInterval(int interval) {
    setValue("storage", "saveInterval", std::to_string(interval));
}

std::string IniParser::getSavePath() {
    std::string value = getValue("storage", "savePath");
    return value;
}

void IniParser::setSavePath(std::string path) {
    setValue("storage", "savePath", path);
}

std::string IniParser::getRtmpUrl() {
    std::string value = getValue("streaming", "url");
    return value;
}

void IniParser::setRtmpUrl(std::string url) {
    setValue("streaming", "url", url);
}

std::string IniParser::getLogPath() {
    std::string value = getValue("log", "logPath");
    return value;
}

void IniParser::setLogPath(std::string path) {
    setValue("log", "logPath", path);
}

std::string& IniParser::trimString(std::string& str)
{
    std::string::size_type pos = 0;
    while (str.npos != (pos = str.find(" ")))
        str = str.replace(pos, pos + 1, "");
    return str;
}

void IniParser::readIni()
{
    std::string path = CONFIG_PATH;
    std::ifstream in_conf_file(path.c_str());
    if (!in_conf_file) return;
    std::string str_line = "";
    std::string str_root = "";
    std::vector<IniNode> vec_ini;
    while (getline(in_conf_file, str_line))
    {
        std::string::size_type left_pos = 0;
        std::string::size_type right_pos = 0;
        std::string::size_type equal_div_pos = 0;
        std::string str_key = "";
        std::string str_value = "";
        if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
        {
            //cout << str_line.substr(left_pos+1, right_pos-1) << endl;
            str_root = str_line.substr(left_pos + 1, right_pos - 1);
        }

        if (str_line.npos != (equal_div_pos = str_line.find("=")))
        {
            str_key = str_line.substr(0, equal_div_pos);
            str_value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
            str_key = trimString(str_key);
            str_value = trimString(str_value);
            //cout << str_key << "=" << str_value << endl;
        }

        if ((!str_root.empty()) && (!str_key.empty()) && (!str_value.empty()))
        {
            IniNode ini_node(str_root, str_key, str_value);
            vec_ini.push_back(ini_node);
            //cout << vec_ini.size() << endl;
        }
    }
    in_conf_file.close();
    in_conf_file.clear();

    //vector convert to map
    std::map<std::string, std::string> map_tmp;
    for (std::vector<IniNode>::iterator itr = vec_ini.begin(); itr != vec_ini.end(); ++itr)
    {
        map_tmp.insert(std::pair<std::string, std::string>(itr->root, ""));
    } //提取出根节点
    for (std::map<std::string, std::string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
    {
        SubNode sn;
        for (std::vector<IniNode>::iterator sub_itr = vec_ini.begin(); sub_itr != vec_ini.end(); ++sub_itr)
        {
            if (sub_itr->root == itr->first)
            {
                sn.InsertElement(sub_itr->key, sub_itr->value);
            }
        }
        map_ini->insert(std::pair<std::string, SubNode>(itr->first, sn));
    }

    hasInit = true;
}

std::string IniParser::getValue(std::string root, std::string key)
{
    if (hasInit) {
        readIni();
    }
    std::map<std::string, SubNode>::iterator itr = map_ini->find(root);
    std::map<std::string, std::string>::iterator sub_itr = itr->second.sub_node.find(key);
    if (!(sub_itr->second).empty())
        return sub_itr->second;
    return "";
}

int IniParser::writeIni(std::string path)
{
    std::ofstream out_conf_file(path.c_str());
    if (!out_conf_file)
        return -1;
    //cout << map_ini.size() << endl;
    for (std::map<std::string, SubNode>::iterator itr = map_ini->begin(); itr != map_ini->end(); ++itr)
    {
        //cout << itr->first << endl;
        out_conf_file << "[" << itr->first << "]" << std::endl;
        for (std::map<std::string, std::string>::iterator sub_itr = itr->second.sub_node.begin(); sub_itr != itr->second.sub_node.end(); ++sub_itr)
        {
            //cout << sub_itr->first << "=" << sub_itr->second << endl;
            out_conf_file << sub_itr->first << "=" << sub_itr->second << std::endl;
        }
    }

    out_conf_file.close();
    out_conf_file.clear();

    return 1;
}

std::vector<IniNode>::size_type IniParser::setValue(std::string root, std::string key, std::string value)
{
    if (hasInit) {
        readIni();
    }
    std::map<std::string, SubNode>::iterator itr = map_ini->find(root);  //查找
    if (map_ini->end() != itr)
    {
        //itr->second.sub_node.insert(pair<string, string>(key, value));
        itr->second.sub_node[key] = value;
    } //根节点已经存在了，更新值
    else
    {
        SubNode sn;
        sn.InsertElement(key, value);
        map_ini->insert(std::pair<std::string, SubNode>(root, sn));
    } //根节点不存在，添加值

    return map_ini->size();
}

std::vector<IniNode>::size_type IniParser::getSize()
{ 
    return map_ini->size();
}

void IniParser::clear()
{ 
    map_ini->clear();
}
