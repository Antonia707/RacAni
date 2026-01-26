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
		std::cout << "ucitana svojstva materijala" << std::endl;

        // koristi se zanji materijal
        int i = scene->mNumMaterials - 1;
		std::cout << "materijal[" << i << "]:\n";

		aiString naziv;
        std::cout << "naziv: ";
		scene->mMaterials[i]->Get(AI_MATKEY_NAME, naziv);
		std::cout << naziv.C_Str() << std::endl;

		std::cout << "pozicija teksture: ";
		aiString texturePosition;
		int width, height, nrChannels;
		unsigned char* data;
		if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
			std::cout << texturePosition.C_Str() << std::endl;

            std::string directory = name.substr(0, name.find_last_of("/\\") + 1);
            std::string texPath = directory + texturePosition.C_Str();
            std::cout << texPath << std::endl;

            Texture* texture = new Texture(texPath);
            object.setTexture(texture);
		}

		glm::vec3 ambientColor;
		aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;
		float shininessK;

		std::cout << "ambient: ";
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
		std::cout << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;
        newMaterial->setAmbientColor(glm::vec3(ambientK.r, ambientK.g, ambientK.b));

		std::cout << "diffuse: ";
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
		std::cout << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;
        newMaterial->setDifuzeColor(glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b));

		std::cout << "specular: ";
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
		std::cout << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;
        newMaterial->setSpecularColor(glm::vec3(specularK.r, specularK.g, specularK.b));

		std::cout << "shininess ";
		scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininessK);
		std::cout << shininessK << std::endl;
        newMaterial->setSpecularExponent(shininessK);

		// emissive komponenta se ne trazi za implementirati u lab. vježbama
		std::cout << "emissive: ";
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveK);
		std::cout << emissiveK.r << " " << emissiveK.g << " " << emissiveK.b << std::endl;

		std::cout << "- - - - - - - - - - - - -" << std::endl;

    }
    object.setMaterial(newMaterial);


    delete[] vertices;
    delete[] normals;
    delete[] uvs;
    delete[] indices;

    return true;
}
