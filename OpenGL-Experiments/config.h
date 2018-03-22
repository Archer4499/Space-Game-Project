#pragma once
#include <string>
#include <map>


/*
Config format:
Comments start with #, either on their own line or after data
Spaces allowed anywhere except in values
Data format:
    tag:value
*/


struct Config {
    std::map<std::string, std::string> data;

    Config() {
        this->data["width"] =   "800";
        this->data["height"] =  "600";
    }
};


int loadConfig(Config *conf, std::string fileName="config.cfg");

int saveConfig(Config *conf, std::string fileName="config.cfg");
