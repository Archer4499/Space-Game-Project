#pragma warning (disable : 4996)
#include <iostream>
#include <fstream>

#include "config.h"


int skipSpaces(char* buf, int i) {
    while (buf[i] == ' ') {
        i++;
    }

    return i;
}


int loadConfig(Config *conf, std::string fileName) {
    // Return 0 if config file loaded properly
    // Return 1 and conf contains default values on file read error
    // Return 2 if invalid file format, conf not guaranteed to contain default values
    // TODO(Derek): fix non default values on return 2
    // TODO(Derek): console logging at each non-zero return


    // Set default values
    conf->data["width"] = "800";
    conf->data["height"] = "600";


    if (FILE *fp = fopen(fileName.c_str(), "r")) {
        char buf[1024];
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            int i = 0;

            // Line:
            i = skipSpaces(buf, i);

            // Skip Comment Lines
            if (buf[i] == '#') continue;

            // Tag
            char tag[1024];
            int j = 0;
            while (buf[i] != ':') {
                tag[j++] = buf[i++];
            }
            int tagLength = j;

            i = skipSpaces(buf, i);

            // Value
            if (buf[i++] == ':') {
                i = skipSpaces(buf, i);

                char value[1024];
                j = 0;
                while (buf[i] != '\n') {
                    value[j++] = buf[i++];

                    // Buf not big enough to contain line
                    if (i == sizeof(buf)) {
                        fgets(buf, sizeof(buf), fp);
                        i = 0;
                    }
                }
                int valueLength = j;

                // Assign value
                conf->data[std::string(tag, tagLength)] = std::string(value, valueLength);
            } else {
                // Invalid line
                return 2;
            }

        }
        if (!feof(fp)) {
            // Error reading file
            return 1;
        }
        fclose(fp);
    } else {
        // Error opening file
        return 1;
    }

    return 0;
}


int saveConfig(Config *conf, std::string fileName) {
    // Returns 0 on successful save
    // Returns 1 on file error
    // TODO(Derek): Keep format/comments from old file

    std::ofstream fs (fileName);

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