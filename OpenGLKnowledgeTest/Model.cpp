#include "Model.h"

unsigned int textureFromFile(const char* name, std::string dir);

Model::Model(std::string path) {
    loadModel(path);
}

void Model::draw(Shader& shader) {
    for (int i = 0; i < meshes.size(); ++i)
        meshes[i].draw(shader);
}

// Uses assimp to load the model into an assimp scene object.
void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    // Creates the assimp scene object
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the node's meshes (if any)
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Vertex vertex;

    // 1. Store the vertex data from the assimp mesh
    for (int i = 0; i < mesh->mNumVertices; ++i) {
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        // We only care about the first set of texture coodinates. Assimp allows 0 - 8 texture coordinates.
        if (mesh->mTextureCoords[0]) // some meshes don't contain texture coordinates
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.TexCoords = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    // 2. Store the indices from the assimp mesh
    for (int i = 0; i < mesh->mNumFaces; ++i) {
        for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
            // For each face of the mesh, push each vertex index of the face to the indices vector.
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    // 3. Store the materials (textures) from the assimp mesh
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Pushes all the diffuse maps into the textures vector
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Pushes all the specular maps into the textures vector
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

// Loads the material data, creates a texture from the data, and store it into a Texture vector
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName) {
    std::vector<Texture> textures;

    for (int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str; // texture name
        mat->GetTexture(type, i, &str);
        bool skip = false;

        // Checks if the texture has been loaded previously, and if so, skips the loading process
        for (int j = 0; j < textures_loaded.size(); ++j) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                return textures;
            }
        }

        // Texture has not been loaded previously. Loads and generates the texture
        Texture texture;
        texture.id = textureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        textures_loaded.push_back(texture);
    }

    return textures;
}

unsigned int textureFromFile(const char* name, std::string dir) {
    Texture2D tex{ dir + '/' + name, 0 };
    return tex.id;
}

