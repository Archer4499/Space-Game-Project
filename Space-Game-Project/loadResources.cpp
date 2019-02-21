#include "loadResources.h"

#include <glad/glad.h>

#include <unordered_map>

#include "logging.h"
#include "fmt\format.h"
#include "stringUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::string readFile(const char *filePath) {
    fmt::memory_buffer out;
    size_t totalLen = 0;

    FILE *fp = _fsopen(filePath, "r", _SH_DENYWR);
    LOG_RETURN(ERR, fp == NULL, "", "Could not read file: {}", filePath);

    char buffer[1024];
    while (size_t len = fread(buffer, 1, sizeof(buffer), fp)) {
        fmt::format_to(out, "{}", buffer);
        totalLen += len;
        // This deals with not being able to use fmt::format_to_n because it is giving an error about not being able to access a private member
        out.resize(totalLen);
    }
    fclose(fp);
    return fmt::to_string(out);
}


int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertexShaderStr = readFile(vertexPath);
    std::string fragmentShaderStr = readFile(fragmentPath);
    const char *vertexShaderSrc = vertexShaderStr.c_str();
    const char *fragmentShaderSrc = fragmentShaderStr.c_str();

    int success = 0;
    int logLength = 0;
    char infoLog[512];

    // Compile vertex shader
    LOG(DEBUG, "Compiling vertex shader.");
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG(ERR, "Vertex Shader Compilation Failed: {}", infoLog);
            return 1;
        } else {
            LOG(DEBUG, "{}", infoLog);
        }
    }

    // Compile fragment shader
    LOG(DEBUG, "Compiling fragment shader.");
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader
    success = 0;
    logLength = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG(ERR, "Fragment Shader Compilation Failed: {}", infoLog);
            return 1;
        } else {
            LOG(DEBUG, "{}", infoLog);
        }
    }


    // Link shaders
    LOG(DEBUG, "Linking program");
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG(ERR, "Shader Program Linking Failed: {}", infoLog);
            return 1;
        } else {
            LOG(DEBUG, "{}", infoLog);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

int loadTexture(const char *texturePath, unsigned int &texID) {
    int texWidth, texHeight, nrComponents;

    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(texturePath, &texWidth, &texHeight, &nrComponents, 0);
    if (!data) {
        LOG(ERR, "Failed to load texture: {}", texturePath);
        stbi_image_free(data);
        return 1;
    }

    GLenum format;
    if (nrComponents == 1) {
        format = GL_RED;
    } else if (nrComponents == 3) {
        format = GL_RGB;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    } else if (nrComponents == 4) {
        format = GL_RGBA;
    } else {
        LOG(ERR, "Texture: {} not RED, RGB or RGBA.", texturePath);
        stbi_image_free(data);
        return 1;
    }

    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    LOG(DEBUG, "Loaded texture: {}, w = {}, h = {}, channels = {}", texturePath, texWidth, texHeight, nrComponents);

    stbi_image_free(data);

    return 0;
}

int loadSprite(const char *spritePath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices) {
    // Return 0 if success
    // Return 1 if failed to load sprite file

    struct Vertex {
        vec2 pos;
        vec2 texCoord;
    };

    std::vector<Vertex> vertices;
    numVertices = 0;

    std::string spriteFile = readFile(spritePath);
    LOG_RETURN(ERR, spriteFile.empty(), 1, "Sprite file: {} empty, no objects loaded.", spritePath);


    size_t i = 0;
    Vertex vertex;

    while (i < spriteFile.length()) {
        while (skipComments(spriteFile, i)) if (i >= spriteFile.length()) break; // Skip any consecutive comment lines

        float x = std::stof(stringUntilSpace(spriteFile, i));
        LOG_RETURN(ERR, i >= spriteFile.length(), 1, "Sprite file: {} invalid at char: {}", spritePath, i);
        float y = std::stof(stringUntilSpace(spriteFile, i));
        LOG_RETURN(ERR, i >= spriteFile.length(), 1, "Sprite file: {} invalid at char: {}", spritePath, i);
        float tx = std::stof(stringUntilSpace(spriteFile, i));
        LOG_RETURN(ERR, i >= spriteFile.length(), 1, "Sprite file: {} invalid at char: {}", spritePath, i);
        float ty = std::stof(stringUntilSpace(spriteFile, i));

        vertex = {{x, y}, {tx, ty}};
        vertices.push_back(vertex);
        numVertices++;
    }

    LOG_RETURN(ERR, vertices.empty(), 1, "No valid sprite info in file: {}", spritePath);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0].pos[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    // texture coords attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));


    glBindVertexArray(0);


    LOG(DEBUG, "Loaded model: {}, Vertices = {}", spritePath, numVertices);

    return 0;
}

int loadAllResources(const char *listPath, std::map<std::string, unsigned int> &shaders, std::vector<InstanceObject> &allObjects) {
    // TODO(Derek): Handle spaces in input file paths
    // TODO(Derek): sanitize input
    LOG(DEBUG, "Loading all objects from: {}", listPath);

    std::string list = readFile(listPath);
    LOG_RETURN(WARN, list.empty(), 1, "Object list file empty, no objects loaded.");

    size_t i = 0;

    std::string ver = stringUntilSpace(list, i);

    if (ver == "v1.5") {
        // File v1.5
        std::unordered_map<std::string, RenderObject> renderObjects;
        std::string name, shaderName, vertFile, fragFile, spriteFile, texFile, posStr, rotStr, scaleStr, colorStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            // TODO(Derek): sanitize name
            char lineType = list[i++];
            if (lineType == '=') {
                // Shader
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid shader name at char: {}", i);

                vertFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, vertFile.empty(), 1, "Object list file contains invalid shader vertex file name at char: {}", i);

                fragFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, fragFile.empty(), 1, "Object list file contains invalid shader fragment file name at char: {}", i);

                LOG(DEBUG, "Loading Shader: {}", name);
                unsigned int shaderProgram = glCreateProgram();
                int ret = loadShader(vertFile.c_str(), fragFile.c_str(), shaderProgram);
                LOG_RETURN(WARN, ret, 1, "Failed to load shader {} from: {}, {}", name, vertFile, fragFile);

                auto result = shaders.emplace(name, shaderProgram);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate shader name at char: {}", i);
            } else if (lineType == ':') {
                // RenderObject (Sprite and texture)
                // TODO(Derek): allow no texture and just use colour from InstanceObject
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid object name at char: {}", i);

                spriteFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, spriteFile.empty(), 1, "Object list file contains invalid sprite file name at char: {}", i);

                texFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, texFile.empty(), 1, "Object list file contains invalid texture file name at char: {}", i);

                unsigned int VAO, VBO, numVertices;
                if (loadSprite(spriteFile.c_str(), VAO, VBO, numVertices)) return 1;

                unsigned int texID;
                if (loadTexture(texFile.c_str(), texID)) return 1;


                RenderObject renderObj;
                renderObj.models.push_back({VAO, VBO, numVertices, texID});

                auto result = renderObjects.emplace(name, renderObj);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate object name at char: {}", i);
            } else if (lineType == '@') {
                // InstanceObject
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid object name at char: {}", i);
                LOG_RETURN(WARN, !renderObjects.count(name), 1, "Object list file contains undefined object name on line at char: {}", i);

                shaderName = stringUntilSpace(list, i);
                LOG_RETURN(WARN, shaderName.empty(), 1, "Object list file contains invalid shader name at char: {}", i);
                LOG_RETURN(WARN, !shaders.count(shaderName), 1, "Object list file contains undefined shader name on line at char: {}", i);

                posStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, posStr.empty(), 1, "Object list file contains invalid position at char: {}", i);

                rotStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, rotStr.empty(), 1, "Object list file contains invalid rotation at char: {}", i);

                scaleStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, scaleStr.empty(), 1, "Object list file contains invalid scale at char: {}", i);

                colorStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, colorStr.empty(), 1, "Object list file contains invalid colour at char: {}", i);

                float rot;
                vec2 pos, scale;
                vec3 color;
                float x, y, z;
                size_t end1, end2;

                try {
                    // Parsing "f,f  f  f,f  f,f,f"
                    // ++ skips the ','
                    // Pos
                    x = std::stof(posStr, &end1);
                    y = std::stof(posStr.substr(++end1));
                    pos = {x, y};

                    // Rotation
                    rot = std::stof(rotStr);

                    // Scale
                    x = std::stof(scaleStr, &end1);
                    y = std::stof(scaleStr.substr(++end1));
                    scale = {x, y};

                    // Colour
                    x = std::stof(colorStr, &end1);
                    y = std::stof(colorStr.substr(++end1), &end2);
                    z = std::stof(colorStr.substr(end1 + (++end2)));
                    color = {x, y, z};

                } catch(...) {
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line at char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{}:{} with colour {}", name, pos, rot, scale, color);

                // Already checked above if shader and name exist in maps
                InstanceObject obj = {pos, rot, scale, color, shaders[shaderName], renderObjects[name]};
                allObjects.push_back(obj);
            }
        }
    } else if (ver[0] == 'v') {
        LOG_RETURN(WARN, true, 1, "Object list file {} not supported", ver);
    } else {
        LOG_RETURN(WARN, true, 1, "Object list file either invalid or version not supported");
    }

    return 0;
}
