#pragma once
#include <iostream>
#include <string>
#include <map>


/*
Config format:
Comment lines start with # (no comments after data lines)
Spaces allowed anywhere
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
