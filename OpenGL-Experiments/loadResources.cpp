#include "loadResources.h"

#include <glad/glad.h>

#include <string>

#include "logging.h"
#include "fmt\format.h"
#include "stringUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


std::string readFile(const char *filePath) {
    fmt::memory_buffer out;
    size_t totalLen = 0;

    if (FILE *fp = _fsopen(filePath, "r", _SH_DENYWR)) {
        char buffer[1024];
        while (size_t len = fread(buffer, 1, sizeof(buffer), fp)) {
            fmt::format_to(out, "{}", buffer);
            totalLen += len;
            // This deals with not being able to use fmt::format_to_n because it is giving an error about not being able to access a private member
            out.resize(totalLen);
        }
        fclose(fp);
        return fmt::to_string(out);
    } else {
        LOG_F(ERR, "Could not read file: {}", filePath);
        return "";
    }
}


int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertexShaderStr = readFile(vertexPath);
    std::string fragmentShaderStr = readFile(fragmentPath);
    const char *vertexShaderSrc = vertexShaderStr.c_str();
    const char *fragmentShaderSrc = fragmentShaderStr.c_str();

    int success;
    int logLength;
    char infoLog[512];

    // Compile vertex shader
    LOG_F(DEBUG, "Compiling vertex shader.");
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG_F(ERR, "Vertex Shader Compilation Failed: {}", infoLog);
            return 1;
        } else {
            LOG_F(DEBUG, "{}", infoLog);
        }
    }

    // Compile fragment shader
    LOG_F(DEBUG, "Compiling fragment shader.");
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG_F(ERR, "Fragment Shader Compilation Failed: {}", infoLog);
            return 1;
        } else {
            LOG_F(DEBUG, "{}", infoLog);
        }
    }


    // Link shaders
    LOG_F(DEBUG, "Linking program");
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            LOG_F(ERR, "Shader Program Linking Failed: {}", infoLog);
            return 1;
        } else {
            LOG_F(DEBUG, "{}", infoLog);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}


int loadTexture(const char *texturePath, unsigned int &texID) {
    int texWidth, texHeight, nrChannels;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load(texturePath, &texWidth, &texHeight, &nrChannels, 0);
    if (!data) {
        LOG_F(ERR, "Failed to load texture: {}", texturePath);
        return 1;
    }
    LOG_F(DEBUG, "Loaded texture: {}, w = {}, h = {}, channels = {}", texturePath, texWidth, texHeight, nrChannels);

    if (nrChannels == 3) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        LOG_F(ERR, "Texture: {} not RGB or RGBA.", texturePath);
        return 1;
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return 0;
}


int loadModel(const char *modelPath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices) {
    // Return 0 if success
    // Return 1 if failed to load model file
    // NOTE(optimisation): use map or set for a unique collection of vertices(with texCoords/normals) and re-index that
    //      ^ Doesn't really help much for the effort required
    // NOTE: possibly use different VBOs for different shapes

    struct Vertex {
        vec3 pos;
        vec3 normal;
        vec2 texCoord;
    };

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    std::vector<Vertex> buffer;

    VBO = 0;
    numVertices = 0;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath);

    if (!err.empty()) {
        LOG_F(ERR, "LoadObj error: {}", err);
        return 1;
    }

    if (!ret) {
        LOG_F(ERR, "Loading object: {} failed.", modelPath);
        return 1;
    }

    if (attrib.normals.size() == 0) {
        LOG_F(ERR, "Object: {} has no normals.", modelPath);
        return 1;
    }

    buffer.reserve(shapes.size() * shapes[0].mesh.indices.size() * sizeof(Vertex));

    // Load vertices from indices
    for (size_t i = 0; i < shapes.size(); ++i) {
        for (size_t k = 0; k < shapes[i].mesh.indices.size(); ++k) {
            tinyobj::index_t idx = shapes[i].mesh.indices[k];
            Vertex vert;

            vert.pos = vec3(&attrib.vertices[3 * idx.vertex_index]);
            vert.normal = vec3(&attrib.normals[3 * idx.normal_index]);

            if (attrib.texcoords.size() > 0) {
                vert.texCoord = vec2(&attrib.texcoords[2 * idx.texcoord_index]);
            } else {
                vert.texCoord = vec2(0.0f);
            }

            buffer.push_back(vert);

            ++numVertices;
        }
    }

    if (buffer.size() > 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(Vertex), &buffer[0].pos[0], GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
        // normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
        // texture coord attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

        glBindVertexArray(0);
    } else {
        LOG_F(ERR, "No valid object info in file: {}", modelPath);
        return 1;
    }


    LOG_F(DEBUG, "Loaded model: {}, Vertices = {}, TexCoords = {}, Indices = {}", modelPath, attrib.vertices.size()/3, attrib.texcoords.size()/2, numVertices);
    // LOG_F(INFO, "Transformed into: {} vertices", numVertices);

    return 0;
}


int loadModelOld(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO) {
    // Return 0 if success
    // Return 1 if failed to load model file
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

int loadAllObjects(const char *listPath, std::vector<InstanceObject> &allObjects) {
    // TODO(Derek): Handle spaces in file paths
    LOG_F(DEBUG, "Loading all objects from: {}", listPath);
    std::string list = readFile(listPath);
    if (list == "") {
        LOG_F(WARN, "Object list file empty, no objects loaded.");
        return 1;
    }

    size_t i = 0;

    std::string ver = stringUntilSpace(list, i);
    if (ver == "v1.0") {
        // File v1.0
        std::string name, posStr, rotStr, scaleStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            // TODO(Derek): sanitize name
            name = stringUntilSpace(list, i);
            if (name != "") {
                posStr = stringUntilSpace(list, i);
                if (posStr == "") {
                    LOG_F(WARN, "Object list file invalid at char: {}", i);
                    return 1;
                }
                rotStr = stringUntilSpace(list, i);
                if (rotStr == "") {
                    LOG_F(WARN, "Object list file invalid at char: {}", i);
                    return 1;
                }
                scaleStr = stringUntilSpace(list, i);
                if (scaleStr == "") {
                    LOG_F(WARN, "Object list file invalid at char: {}", i);
                    return 1;
                }

                float angle;
                vec3 pos, rot, scale;

                try {
                    // Parsing "float,float,float"
                    // ++ skips the ','
                    size_t end1 = 0, end2 = 0, end3 = 0;
                    float x = std::stof(posStr, &end1);
                    float y = std::stof(posStr.substr(++end1), &end2);
                    float z = std::stof(posStr.substr(end1 + (++end2)));
                    pos = {x, y, z};

                    angle = std::stof(rotStr, &end1);
                    x = std::stof(rotStr.substr(++end1), &end2);
                    y = std::stof(rotStr.substr(end1 + (++end2)), &end3);
                    z = std::stof(rotStr.substr(end1 + end2 + (++end3)));
                    rot = {x, y, z};

                    x = std::stof(scaleStr, &end1);
                    y = std::stof(scaleStr.substr(++end1), &end2);
                    z = std::stof(scaleStr.substr(end1 + (++end2)));
                    scale = {x, y, z};
                } catch(...) {
                    LOG_F(WARN, "Object list file contains invalid float on line with char: {}", i);
                    return 1;
                }

                LOG_F(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                unsigned int VAO, VBO, numVertices;
                if (loadModel(name.c_str(), VAO, VBO, numVertices)) {
                    return 1;
                }

                unsigned int texID = 0;

                RenderObject renObj = {VAO, VBO, numVertices, texID};
                InstanceObject obj = {pos, angle, rot, scale, renObj};
                allObjects.push_back(obj);
            }
        }
    } else if (ver == "v1.1") {
        // File v1.1
        std::map<std::string, RenderObject> renderObjects;
        std::string name, objFile, texFile, posStr, rotStr, scaleStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            // TODO(Derek): sanitize name
            if (list[i] == ':') {
                ++i;
                name = stringUntilSpace(list, i);
                if (name == "") {
                    LOG_F(WARN, "Object list file contains invalid name at char: {}", i);
                    return 1;
                }
                objFile = stringUntilSpace(list, i);
                if (objFile == "") {
                    LOG_F(WARN, "Object list file contains invalid object file name at char: {}", i);
                    return 1;
                }
                texFile = stringUntilSpace(list, i);
                if (texFile == "") {
                    LOG_F(WARN, "Object list file contains invalid texture file name at char: {}", i);
                    return 1;
                }

                unsigned int VAO, VBO, numVertices;
                if (loadModel(objFile.c_str(), VAO, VBO, numVertices)) {
                    return 1;
                }

                unsigned int texID;
                if (loadTexture(texFile.c_str(), texID)) {
                    return 1;
                }

                RenderObject renObj = {VAO, VBO, numVertices, texID};
                // TODO(Derek): Check whether name already exists
                renderObjects[name] = renObj;
            } else if (list[i] == '@') {
                ++i;
                name = stringUntilSpace(list, i);
                if (name == "") {
                    LOG_F(WARN, "Object list file contains invalid name at char: {}", i);
                    return 1;
                }
                posStr = stringUntilSpace(list, i);
                if (posStr == "") {
                    LOG_F(WARN, "Object list file contains invalid position at char: {}", i);
                    return 1;
                }
                rotStr = stringUntilSpace(list, i);
                if (rotStr == "") {
                    LOG_F(WARN, "Object list file contains invalid rotation at char: {}", i);
                    return 1;
                }
                scaleStr = stringUntilSpace(list, i);
                if (scaleStr == "") {
                    LOG_F(WARN, "Object list file contains invalid scale at char: {}", i);
                    return 1;
                }

                float angle;
                vec3 pos, rot, scale;

                try {
                    // Parsing "float,float,float"
                    // ++ skips the ','
                    size_t end1 = 0, end2 = 0, end3 = 0;
                    float x = std::stof(posStr, &end1);
                    float y = std::stof(posStr.substr(++end1), &end2);
                    float z = std::stof(posStr.substr(end1 + (++end2)));
                    pos = {x, y, z};

                    angle = std::stof(rotStr, &end1);
                    x = std::stof(rotStr.substr(++end1), &end2);
                    y = std::stof(rotStr.substr(end1 + (++end2)), &end3);
                    z = std::stof(rotStr.substr(end1 + end2 + (++end3)));
                    rot = {x, y, z};

                    x = std::stof(scaleStr, &end1);
                    y = std::stof(scaleStr.substr(++end1), &end2);
                    z = std::stof(scaleStr.substr(end1 + (++end2)));
                    scale = {x, y, z};
                } catch(...) {
                    LOG_F(WARN, "Object list file contains invalid float on line with char: {}", i);
                    return 1;
                }

                LOG_F(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                // TODO(Derek): check whether name exists first
                InstanceObject obj = {pos, angle, rot, scale, renderObjects[name]};
                allObjects.push_back(obj);
            }
        }
    } else {
        LOG_F(WARN, "Object list file either invalid or version not supported");
        return 1;
    }

    return 0;
}
