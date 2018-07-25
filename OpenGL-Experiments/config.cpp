#pragma warning (disable : 4996)
#include <fstream>

#include "config.h"
#include "stringUtils.h"
#include "logging.h"


std::string Config::getString(std::string label) {
    return data[label];
}
int Config::getInt(std::string label) {
    return std::stoi(data[label]);
}
bool Config::getBool(std::string label) {
    std::string value = data[label];
    return value == "true" || value == "1";
}


int loadConfig(Config *conf, const char *fileName) {
    // Return 0 if config file loaded properly
    // Return 1 and conf contains default values on file read error
    // Return 2 if invalid file format, conf not guaranteed to contain default values
    // TODO(Derek): fix non default values on return 2

    std::ifstream fs(fileName);

    if (!fs.is_open()) {
        LOG_F(ERR, "Error opening config file: {} during load", fileName);
        return 1;
    }

    std::string line;

    while (std::getline(fs, line)) {
        size_t i = 0;

        // Line:
        i = skipWhiteSpace(line, i);

        // Skip Comment Lines
        if (line[i] == '#') continue;

        // Tag
        std::string tag;

        while (line[i] != ':') {
            tag += line[i++];
        }


        // Value
        if (line[i++] != ':') {
            // Invalid line
            return 2;
        }

        i = skipWhiteSpace(line, i);

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
    }

    fs.close();

    return 0;
}


int saveConfig(Config *conf, const char *fileName) {
    // Returns 0 on successful save
    // Returns 1 on file error
    // TODO(Derek): Keep format/comments from old file

    FILE *fs = _fsopen(fileName, "w", _SH_DENYWR);

    if (fs == NULL) {
        LOG_F(ERR, "Error opening config file: {} during save", fileName);
        return 1;
    }

    for (std::map<std::string, std::string>::iterator it = (conf->data).begin(); it != (conf->data).end(); ++it) {
        fmt::print(fs, "{}: {}\n", it->first, it->second);
    }

    fclose(fs);

    return 0;
}
