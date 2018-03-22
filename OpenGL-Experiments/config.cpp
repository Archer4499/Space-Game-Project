#pragma warning (disable : 4996)
#include <iostream>
#include <fstream>

#include "config.h"


int skipSpaces(std::string str, int i) {
    while (str[i] == ' ') ++i;

    return i;
}


int loadConfig(Config *conf, std::string fileName) {
    // Return 0 if config file loaded properly
    // Return 1 and conf contains default values on file read error
    // Return 2 if invalid file format, conf not guaranteed to contain default values
    // TODO(Derek): fix non default values on return 2


    // Set default values
    conf->data["width"] = "800";
    conf->data["height"] = "600";


    std::ifstream fs(fileName);

    if (fs.is_open()) {
        std::string line;

        while (std::getline(fs, line)) {
            unsigned int i = 0;

            // Line:
            i = skipSpaces(line, i);

            // Skip Comment Lines
            if (line[i] == '#') continue;

            // Tag
            std::string tag;

            while (line[i] != ':') {
                tag += line[i++];
            }

            i = skipSpaces(line, i);

            // Value
            if (line[i++] == ':') {
                i = skipSpaces(line, i);

                std::string value;

                while (i < line.length()) {
                    // Ignore comments at end of line (no spaces in values)
                    if (line[i] == '#' || line[i] == ' ') {
                        if (value.length() == 0) {
                            // Invalid value
                            return 2;
                        }
                        break;
                    }
                    value += line[i++];
                }

                // Assign value
                conf->data[tag] = value;
            } else {
                // Invalid line
                return 2;
            }
        }
    } else {
        // File could not be opened
        return 1;
    }

    fs.close();

    return 0;
}


int saveConfig(Config *conf, std::string fileName) {
    // Returns 0 on successful save
    // Returns 1 on file error
    // TODO(Derek): Keep format/comments from old file

    std::ofstream fs(fileName);

    if (fs.is_open()) {
        for (std::map<std::string, std::string>::iterator it = (conf->data).begin(); it != (conf->data).end(); ++it) {
            fs << it->first << ": " << it->second << std::endl;
        }
    } else {
        // File could not be opened
        return 1;
    }

    fs.close();


    return 0;
}
