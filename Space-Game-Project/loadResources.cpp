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
    // Return 1 if failed to load model file

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

/*int loadModel(const char *modelPath, const char *materialBaseDir, RenderObject &renderObj) {
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


    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath, materialBaseDir);

    LOG_IF(WARN, !err.empty(), "LoadObj error: {}", err);
    LOG_RETURN(ERR, !success, 1, "Loading object: {} failed.", modelPath);

    LOG_RETURN(ERR, shapes.empty(), 1, "No valid object info in file: {}", modelPath);

    LOG(DEBUG, "Loading model: {} with {} materials, {} shapes, {} vertices and {} texCoords", modelPath, shapes.size(), materials.size(), attrib.vertices.size()/3, attrib.texcoords.size()/2);


    std::unordered_map<std::string, unsigned int> textures; // TODO(Derek): make persistent across models
    renderObj.materials.resize(materials.size());

    // Materials
    unsigned int texID;
    for (size_t i = 0; i < materials.size(); ++i) {
        LOG(DEBUG, "    Loading material: {}", materials[i].name);

        // renderObj.materials[i].ambient       = vec3(materials[i].ambient);
        // renderObj.materials[i].diffuse       = vec3(materials[i].diffuse);
        // renderObj.materials[i].specular      = vec3(materials[i].specular);
        // renderObj.materials[i].transmittance = vec3(materials[i].transmittance);
        // renderObj.materials[i].emission      = vec3(materials[i].emission);
        // renderObj.materials[i].shininess     = materials[i].shininess;

        std::string texNames[8] = {materials[i].ambient_texname,
                                   materials[i].diffuse_texname,
                                   materials[i].specular_texname,
                                   materials[i].specular_highlight_texname,
                                   materials[i].bump_texname,
                                   materials[i].displacement_texname,
                                   materials[i].alpha_texname,
                                   materials[i].reflection_texname};

        for (int j = 0; j < 8; ++j) {
            if (!texNames[j].empty()) {
                std::string name = materialBaseDir + texNames[j];
                if (textures.count(name)) {
                    texID = textures[name];
                } else {
                    if (loadTexture((name).c_str(), texID)) return 1;
                    textures[name] = texID;
                }
                renderObj.materials[i].texID[j] = texID;
            } else {
                renderObj.materials[i].texID[j] = -1;
            }
        }
    }

    bool hasNormals = attrib.normals.size() > 0;
    bool hasTexCoords = attrib.texcoords.size() > 0;

    // Load vertices from indices
    for (const tinyobj::shape_t shape : shapes) {
        size_t numVertices = shape.mesh.indices.size();
        LOG_RETURN(ERR, numVertices == 0, 1, "No vertices in shape: {} in file: {}", shape.name, modelPath);

        std::vector<Vertex> buffer(numVertices);

        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); ++i) {
            // TODO(Derek): Do materials per face
            // size_t matID = shape.mesh.material_ids[i];
            // TODO(Derek): Use default material.
            // LOG_RETURN(ERR, (matID < 0 || matID >= materials.size()), 1, "Invalid material ID in shape: {} in file: {}", shape.name, modelPath);

            for (size_t k = 0; k < 3; ++k) {
                size_t j = (3*i)+k;
                tinyobj::index_t idx = shape.mesh.indices[j];

                buffer[j].pos = vec3(&attrib.vertices[3 * idx.vertex_index]);

                if (hasNormals) {
                    buffer[j].normal = vec3(&attrib.normals[3 * idx.normal_index]);
                }

                if (hasTexCoords) {
                    buffer[j].texCoord = vec2(&attrib.texcoords[2 * idx.texcoord_index]);
                } else {
                    buffer[j].texCoord = vec2(0.0f);
                }
            }
            if (!hasNormals) {
                vec3 norm = normalize(cross(buffer[3*i+2].pos-buffer[3*i+0].pos, buffer[3*i+1].pos-buffer[3*i+0].pos));
                buffer[3*i+0].normal = norm;
                buffer[3*i+1].normal = norm;
                buffer[3*i+2].normal = norm;
            }
        }

        unsigned int VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // TODO(Derek): how multiple VBOs??

        glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(Vertex), &buffer[0].pos[0], GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
        // normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
        // texture coord attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

        glBindVertexArray(0);

        LOG(DEBUG, "    Loaded shape: {} with {} indices", shape.name, numVertices);

        size_t matID = shape.mesh.material_ids[0];
        renderObj.models.push_back({VAO, VBO, numVertices, matID});
    }

    glBindVertexArray(0);
    return 0;
}*/


int loadAllResources(const char *listPath, std::map<std::string, unsigned int> &shaders, std::vector<InstanceObject> &allObjects) {
    // TODO(Derek): Handle spaces in input file paths
    // TODO(Derek): sanitize input
    LOG(DEBUG, "Loading all objects from: {}", listPath);

    std::string list = readFile(listPath);
    LOG_RETURN(WARN, list.empty(), 1, "Object list file empty, no objects loaded.");

    size_t i = 0;

    std::string ver = stringUntilSpace(list, i);
    if (ver == "v1.0") {
        // File v1.0
        LOG_RETURN(WARN, true, 1, "Object list file v1.0 not supported");
        /*std::string name, posStr, rotStr, scaleStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            name = stringUntilSpace(list, i);
            if (name != "") {
                posStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, posStr.empty(), 1, "Object list file invalid at char: {}", i);

                rotStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, rotStr.empty(), 1, "Object list file invalid at char: {}", i);

                scaleStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, scaleStr.empty(), 1, "Object list file invalid at char: {}", i);

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
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line with char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                unsigned int VAO, VBO, numVertices;
                if (loadMesh(name.c_str(), VAO, VBO, numVertices)) return 1;

                unsigned int texID = 0;

                RenderObject renObj = {VAO, VBO, numVertices, texID};
                InstanceObject obj = {pos, angle, rot, scale, renObj};
                allObjects.push_back(obj);
            }
        }*/
    } else if (ver == "v1.1") {
        // File v1.1
        LOG_RETURN(WARN, true, 1, "Object list file v1.1 not supported");
        /*std::map<std::string, RenderObject> renderObjects;
        std::string name, objFile, texFile, posStr, rotStr, scaleStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            // TODO(Derek): sanitize name
            if (list[i] == ':') {
                // RenderObject
                ++i;

                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid name at char: {}", i);

                objFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, objFile.empty(), 1, "Object list file contains invalid object file name at char: {}", i);

                texFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, texFile.empty(), 1, "Object list file contains invalid texture file name at char: {}", i);

                unsigned int VAO, VBO, numVertices;
                if (loadMesh(objFile.c_str(), VAO, VBO, numVertices)) return 1;

                unsigned int texID;
                if (loadTexture(texFile.c_str(), texID)) return 1;

                RenderObject renderObj = {VAO, VBO, numVertices, texID};
                auto result = renderObjects.emplace(name, renderObj);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate name at char: {}", i);
            } else if (list[i] == '@') {
                // InstanceObject
                ++i;
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid name at char: {}", i);
                LOG_RETURN(WARN, !renderObjects.count(name), 1, "Object list file contains undefined name on line at char: {}", i);

                posStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, posStr.empty(), 1, "Object list file contains invalid position at char: {}", i);

                rotStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, rotStr.empty(), 1, "Object list file contains invalid rotation at char: {}", i);

                scaleStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, scaleStr.empty(), 1, "Object list file contains invalid scale at char: {}", i);

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
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line with char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                InstanceObject obj = {pos, angle, rot, scale, renderObjects[name]};
                allObjects.push_back(obj);
            }
        }*/
    } else if (ver == "v1.2") {
        // File v1.2
        LOG_RETURN(WARN, true, 1, "Object list file v1.2 not supported");
        /*std::map<std::string, RenderObject> renderObjects;
        std::string name, objFile, texFile, materialBaseDir, posStr, rotStr, scaleStr;

        while (i < list.length()) {
            while (skipComments(list, i)) if (i >= list.length()) break; // Skip any consecutive comment lines
            // TODO(Derek): sanitize name
            if (list[i] == '&') {
                // Mesh and texture
                ++i;

                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid name at char: {}", i);

                objFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, objFile.empty(), 1, "Object list file contains invalid object file name at char: {}", i);

                texFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, texFile.empty(), 1, "Object list file contains invalid texture file name at char: {}", i);

                unsigned int VAO, VBO, numVertices;
                if (loadMesh(objFile.c_str(), VAO, VBO, numVertices)) return 1;

                unsigned int texID;
                if (loadTexture(texFile.c_str(), texID)) return 1;


                RenderObject renderObj;
                renderObj.models.push_back({VAO, VBO, numVertices, 0});
                renderObj.materials.push_back({{-1, texID, -1, -1, -1, -1, -1, -1}});

                auto result = renderObjects.emplace(name, renderObj);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate name at char: {}", i);
            } else if (list[i] == ':') {
                // Object (with .mtl info)
                ++i;

                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid name at char: {}", i);

                objFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, objFile.empty(), 1, "Object list file contains invalid object file name at char: {}", i);

                materialBaseDir = stringUntilSpace(list, i);
                LOG_RETURN(WARN, materialBaseDir.empty(), 1, "Object list file contains invalid material folder name at char: {}", i);

                RenderObject renderObj;
                if (loadModel(objFile.c_str(), materialBaseDir.c_str(), renderObj)) return 1;

                auto result = renderObjects.emplace(name, renderObj);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate name at char: {}", i);
            } else if (list[i] == '@') {
                // InstanceObject
                ++i;
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid name at char: {}", i);
                LOG_RETURN(WARN, !renderObjects.count(name), 1, "Object list file contains undefined name on line at char: {}", i);

                posStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, posStr.empty(), 1, "Object list file contains invalid position at char: {}", i);

                rotStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, rotStr.empty(), 1, "Object list file contains invalid rotation at char: {}", i);

                scaleStr = stringUntilSpace(list, i);
                LOG_RETURN(WARN, scaleStr.empty(), 1, "Object list file contains invalid scale at char: {}", i);

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
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line with char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                InstanceObject obj = {pos, angle, rot, scale, renderObjects[name]};
                allObjects.push_back(obj);
            }
        }*/
    } else if (ver == "v1.3") {
        // File v1.3
        LOG_RETURN(WARN, true, 1, "Object list file v1.2 not supported");
        /*std::unordered_map<std::string, RenderObject> renderObjects;
        std::string name, shaderName, vertFile, fragFile, objFile, texFile, materialBaseDir, posStr, rotStr, scaleStr;

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
            } else if (lineType == '&') {
                // Mesh and texture
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid object name at char: {}", i);

                objFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, objFile.empty(), 1, "Object list file contains invalid object file name at char: {}", i);

                texFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, texFile.empty(), 1, "Object list file contains invalid texture file name at char: {}", i);

                unsigned int VAO, VBO, numVertices;
                if (loadMesh(objFile.c_str(), VAO, VBO, numVertices)) return 1;

                unsigned int texID;
                if (loadTexture(texFile.c_str(), texID)) return 1;


                RenderObject renderObj;
                renderObj.models.push_back({VAO, VBO, numVertices, 0});
                renderObj.materials.push_back({{-1, texID, -1, -1, -1, -1, -1, -1}});

                auto result = renderObjects.emplace(name, renderObj);
                LOG_RETURN(WARN, !result.second, 1, "Object list file contains duplicate object name at char: {}", i);
            } else if (lineType == ':') {
                // Object (with .mtl info)
                name = stringUntilSpace(list, i);
                LOG_RETURN(WARN, name.empty(), 1, "Object list file contains invalid object name at char: {}", i);

                objFile = stringUntilSpace(list, i);
                LOG_RETURN(WARN, objFile.empty(), 1, "Object list file contains invalid object file name at char: {}", i);

                materialBaseDir = stringUntilSpace(list, i);
                LOG_RETURN(WARN, materialBaseDir.empty(), 1, "Object list file contains invalid material folder name at char: {}", i);

                RenderObject renderObj;
                if (loadModel(objFile.c_str(), materialBaseDir.c_str(), renderObj)) return 1;

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

                float angle;
                vec3 pos, rot, scale;

                try {
                    // Parsing "float,float,float f,f,f,f f,f,f"
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
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line with char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{},{}:{}", name, pos, angle, rot, scale);

                // Already checked above if shader and name exist in maps
                InstanceObject obj = {pos, angle, rot, scale, shaders[shaderName], renderObjects[name]};
                allObjects.push_back(obj);
            }
        }*/
    } else if (ver == "v1.4") {
        // File v1.4
        std::unordered_map<std::string, RenderObject> renderObjects;
        std::string name, shaderName, vertFile, fragFile, spriteFile, texFile, posStr, rotStr, scaleStr;

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
                // TODO(Derek): allow colour and/or texture
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

                float rot;
                vec2 pos, scale;

                try {
                    // Parsing "float,float f f,f"
                    // ++ skips the ','
                    size_t end = 0;
                    float x = std::stof(posStr, &end);
                    float y = std::stof(posStr.substr(++end));
                    pos = {x, y};

                    rot = std::stof(rotStr, &end);
                    // x = std::stof(rotStr.substr(++end1), &end2);
                    // y = std::stof(rotStr.substr(end1 + (++end2)), &end3);
                    // z = std::stof(rotStr.substr(end1 + end2 + (++end3)));
                    // rot = {x, y, z};

                    x = std::stof(scaleStr, &end);
                    y = std::stof(scaleStr.substr(++end));
                    scale = {x, y};
                } catch(...) {
                    LOG_RETURN(WARN, true, 1, "Object list file contains invalid float on line at char: {}", i);
                }

                LOG(DEBUG, "Loading: {} at {}:{}:{}", name, pos, rot, scale);

                // Already checked above if shader and name exist in maps
                InstanceObject obj = {pos, rot, scale, shaders[shaderName], renderObjects[name]};
                allObjects.push_back(obj);
            }
        }
    } else {
        LOG_RETURN(WARN, true, 1, "Object list file either invalid or version not supported");
    }

    return 0;
}
