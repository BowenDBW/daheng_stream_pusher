#include "ini_parser.h"

std::string IniParser::CONFIG_PATH = "config.ini";

mINI::INIFile* IniParser::file = new mINI::INIFile(CONFIG_PATH);

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

std::string IniParser::getValue(std::string root, std::string key) {
    mINI::INIStructure ini;
    file->read(ini);
    std::string value = ini[root][key];
    return value;
}

void IniParser::setValue(std::string root, std::string key, std::string value) {
    mINI::INIStructure ini;
    file->read(ini);
    
    ini[root][key] = value;

    file->write(ini);
}
