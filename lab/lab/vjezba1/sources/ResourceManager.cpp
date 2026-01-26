#include <string>
#include <regex>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ResourceManager.h"
#include "Object.h"
#include "TriangleMesh.h"
#include "Texture.h"
#include "Lines.h"

bool ResourceManager::getScene(const std::string& name, Object &object) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(name.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_FlipUVs |
        aiProcess_GenNormals
    );

    if (!scene || !scene->HasMeshes()) {
		std::cerr << "Error loading model: " << name << std::endl;
        std::cerr << importer.GetErrorString();
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    std::cout << "ucitana poligonalna mreza" << std::endl;

    int vertexCount = mesh->mNumVertices;

    glm::vec3* vertices = new glm::vec3[vertexCount];
    glm::vec3* normals = new glm::vec3[vertexCount];
    glm::vec3* uvs = new glm::vec3[vertexCount];

    for (int v = 0; v < vertexCount; ++v) {
        aiVector3D pos = mesh->mVertices[v];
        vertices[v] = glm::vec3(pos.x, pos.y, pos.z);

        aiVector3D n = mesh->mNormals[v];
        normals[v] = glm::vec3(n.x, n.y, n.z);

        aiVector3D uv = mesh->mTextureCoords[0][v];
        uvs[v] = glm::vec3(uv.x, uv.y, uv.z);
    }

    int indexCount = mesh->mNumFaces * 3;
    unsigned int* indices = new unsigned int[indexCount];
    int k = 0;
    for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
        aiFace& face = mesh->mFaces[f];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices[k++] = face.mIndices[j];
        }
    }

    TriangleMesh* triangleMesh = new TriangleMesh();
    triangleMesh->setVertices(vertices, vertexCount);
    if (mesh->HasNormals()) triangleMesh->setNormals(normals, vertexCount);
    if (mesh->HasTextureCoords(0)) triangleMesh->setUVCoords(uvs, vertexCount);
    triangleMesh->setIndices(indices, indexCount);

    triangleMesh->normalize();

	object.setRenderable(triangleMesh);

    Material* newMaterial = new Material();
	if (scene->HasMaterials()) {

        // koristi se zanji materijal
        int i = scene->mNumMaterials - 1;

		aiString naziv;
		scene->mMaterials[i]->Get(AI_MATKEY_NAME, naziv);

		aiString texturePosition;
		int width, height, nrChannels;
		unsigned char* data;
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
            std::string directory = name.substr(0, name.find_last_of("/\\") + 1);
            std::string texPath = directory + texturePosition.C_Str();

            Texture* texture = new Texture(texPath);
            object.setTexture(texture);
		}

		glm::vec3 ambientColor;
		aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;
		float shininessK;

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
        newMaterial->setAmbientColor(glm::vec3(ambientK.r, ambientK.g, ambientK.b));

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
        newMaterial->setDifuzeColor(glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b));

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
        newMaterial->setSpecularColor(glm::vec3(specularK.r, specularK.g, specularK.b));

		scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininessK);
        newMaterial->setSpecularExponent(shininessK);

    }
    object.setMaterial(newMaterial);


    delete[] vertices;
    delete[] normals;
    delete[] uvs;
    delete[] indices;

    return true;
}

bool ResourceManager::getVertices(const std::string& name, Lines& lines) {
    std::ifstream file(name);
    if (!file.is_open()) {
        std::cerr << "Error loading vertex file: " << name << std::endl;
        return false;
    }

    std::vector<glm::vec3> vertices;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        // skip empty lines or comments
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        float x, y, z;

        if (!(iss >> x >> y >> z)) {
            std::cerr << "Warning: Invalid line " << lineNumber
                << " in " << name << " -> \"" << line << "\"" << std::endl;
            continue;
        }

        vertices.emplace_back(x, y, z);
    }

    file.close();

    if (vertices.empty()) {
        std::cerr << "Error: No valid vertices loaded from " << name << std::endl;
        return false;
    }

    // assign vertices to the Lines object
    lines.setVertices(vertices, static_cast<int>(vertices.size()));

    std::cout << "Loaded " << vertices.size()
        << " control points from " << name << std::endl;

    return true;
}