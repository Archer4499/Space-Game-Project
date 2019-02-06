#pragma once
#include <string>
#include <map>


/*
Config format:
Comments start with #, either on their own line or at end of line after a space
Data lines contain a tag then a space then a value
No spaces in either tags or values
Example:

# Comment
tag value # Comment

*/

struct Config {
    std::map<std::string, std::string> data;

    Config() {
        this->data["width"] =      "800";
        this->data["height"] =     "600";
        this->data["fullscreen"] = "0";
        this->data["vsync"] =      "true";
    }

    std::string getString(std::string label);
    int         getInt(std::string label);
    bool        getBool(std::string label);
};


int loadConfig(Config *conf, const char *fileName);

int saveConfig(Config *conf, const char *fileName);
