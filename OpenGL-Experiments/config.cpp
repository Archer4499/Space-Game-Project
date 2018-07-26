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
        eatSpaces(line, i);

        // Skip Comment Lines
        if (line[i] == '#') continue;

        // Tag
        std::string tag;

        while (line[i] != ':' && i < line.length()) {
            tag += line[i++];
        }

        if (tag.length() == 0 || line[i++] != ':') {
            LOG_F(ERR, "Invalid line in config file: {} at char index:{}", fileName, static_cast<int>(fs.tellg()) - (line.length()-i-1));
            return 2;
        }

        eatSpaces(line, i);

        // Value
        std::string value;

        // Ignore comments at end of line (no spaces in values)
        while (line[i] != '#' && !isspace(line[i]) && i < line.length()) {
            value += line[i++];
        }
        if (value.length() == 0) {
            LOG_F(ERR, "Invalid value in config file: {} at char index:{}", fileName, static_cast<int>(fs.tellg()) - (line.length()-i));
            return 2;
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
