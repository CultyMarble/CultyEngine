#include <CultyEngine/Inc/CultyEngine.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::MathC;

struct Arguments
{
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;
    float scale = 1.0f;
};

Vector3 ToVector3(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        static_cast<float>(v.z)
    };
}

Vector2 ToTexCoord(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
    };
}

Color ToColor(const aiColor3D& color)
{
    return {
        static_cast<float>(color.r),
        static_cast<float>(color.g),
        static_cast<float>(color.b),
        1.0f
    };
}

std::optional<Arguments> ParsArgs(int argc, char* argv[])
{
    if (argc < 3)
        return std::nullopt;

    Arguments args;
    args.inputFileName = argv[argc - 2];
    args.outputFileName = argv[argc - 1];

    for (int i = 1; i + 2 < argc; ++i)
    {
        if (strcmp(argv[i], "-scale") == 0)
        {
            args.scale = atof(argv[i + 1]);
            ++i;
        }
    }

    return args;
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args,
    const std::filesystem::path& fileName)
{
    printf("Extracting embedded texture %s...\n", fileName.u8string().c_str());

    std::string fullFileName = args.outputFileName.u8string();
    fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
    fullFileName += fileName.filename().u8string();

    FILE* file = nullptr;
    auto err = fopen_s(&file, fullFileName.c_str(), "wb");
    if (err != 0 || file == nullptr)
    {
        printf("Error: Failed to open file %s for saving...\n", fullFileName.c_str());
        return;
    }

    size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
    ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
    fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial,
    aiTextureType textureType, const Arguments& args, const std::string& suffix,
    uint32_t materialIndex)
{
    const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
    if (textureCount == 0)
        return "";

    std::filesystem::path textureName;
    aiString texturePath;
    if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
    {
        if (texturePath.C_Str()[0] == '*') // embedded in the root
        {
            std::string fileName = args.inputFileName.u8string();
            fileName.erase(fileName.length() - 4);
            fileName += suffix;
            fileName += texturePath.C_Str()[1];

            ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

            int textureIndex = atoi(texturePath.C_Str() + 1);
            ASSERT(textureIndex < scene->mNumTextures, "Error: Invalid texture index!");

            const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
            ASSERT(embeddedTexture->mHeight == 0, "Error: Uncompressed texture found!");

            if (embeddedTexture->CheckFormat("jpg"))
            {
                fileName += ".jpg";
            }
            else if (embeddedTexture->CheckFormat("png"))
            {
                fileName += ".png";
            }
            else
            {
                ASSERT(false, "Error: Unrecognized texture format!");
            }

            ExportEmbeddedTexture(embeddedTexture, args, fileName);
            printf("Adding texture %s\n", fileName.c_str());
            textureName = fileName;
        }
        else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
        {
            std::filesystem::path embeddedFilePath = texturePath.C_Str();
            std::string fileName = args.inputFileName.u8string();
            fileName.erase(fileName.length() - 4);
            fileName += suffix;
            fileName += "_" + std::to_string(materialIndex);
            fileName += embeddedFilePath.extension().u8string();

            ExportEmbeddedTexture(embeddedTexture, args, fileName);
            printf("Adding texture %s\n", fileName.c_str());
            textureName = fileName;
        }
        else
        {
            std::filesystem::path filePath = texturePath.C_Str();
            std::string fileName = filePath.filename().u8string();

            printf("Adding texture %s\n", fileName.c_str());
            textureName = fileName;
        }
    }
    return textureName.filename().u8string();
}

// -scale 0.01 input output
int main(int argc, char* argv[])
{
    const auto argOpt = ParsArgs(argc, argv);
    if (argOpt.has_value() == false)
    {
        std::printf("No arguments present in the import fbx model...\n");
        return EXIT_FAILURE;
    }

    const Arguments& args = argOpt.value();
    Assimp::Importer importer;
    const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;
    const aiScene* scene = importer.ReadFile(args.inputFileName.u8string().c_str(), flags);
    if (scene == nullptr)
    {
        std::printf("Error: %s\n", importer.GetErrorString());
        return EXIT_FAILURE;
    }

    std::printf("Importing %s...\n", args.inputFileName.u8string().c_str());

    Model model;
    if (scene->HasMeshes())
    {
        std::printf("Reading mesh data...\n");
        for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const aiMesh* assimpMesh = scene->mMeshes[meshIndex];

            if (assimpMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
                continue;

            const uint32_t numVertices = assimpMesh->mNumVertices;
            const uint32_t numFaces = assimpMesh->mNumFaces;
            const uint32_t numIndices = numFaces * 3;

            Model::MeshData& meshData = model.meshData.emplace_back();

            std::printf("Reading material index...\n");
            meshData.materialIndex = assimpMesh->mMaterialIndex;

            std::printf("Reading vertices...\n");
            Mesh& mesh = meshData.mesh;
            mesh.vertices.reserve(numVertices);

            const aiVector3D* positions = assimpMesh->mVertices;
            const aiVector3D* normals = assimpMesh->mNormals;
            const aiVector3D* tangents = assimpMesh->HasTangentsAndBitangents() ? assimpMesh->mTangents : nullptr;
            const aiVector3D* texCoords = assimpMesh->HasTextureCoords(0) ? assimpMesh->mTextureCoords[0] : nullptr;
            for (uint32_t v = 0; v < numVertices; ++v)
            {
                Vertex& vertex = mesh.vertices.emplace_back();
                vertex.position = ToVector3(positions[v]) * args.scale;
                vertex.normal = ToVector3(normals[v]);
                vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
                vertex.uvCoord = texCoords ? ToTexCoord(texCoords[v]) : Vector2::Zero;
            }

            std::printf("Reading indices...\n");
            mesh.indices.reserve(numIndices);
            const aiFace* aiFaces = assimpMesh->mFaces;
            for (uint32_t f = 0; f < numFaces; ++f)
            {
                const aiFace& assimpFace = aiFaces[f];
                for (uint32_t i = 0; i < 3; ++i)
                {
                    mesh.indices.push_back(assimpFace.mIndices[i]);
                }
            }
        }
    }

    if (scene->HasMaterials())
    {
        printf("Reading material data...\n");

        const uint32_t numMaterials = scene->mNumMaterials;
        model.materialData.reserve(numMaterials);
        for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
        {
            const aiMaterial* assimpMaterial = scene->mMaterials[materialIndex];

            aiColor3D ambient;
            aiColor3D diffuse;
            aiColor3D emissive;
            aiColor3D specular;

            ai_real specularPower = 1.0f;
            assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
            assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            assimpMaterial->Get(AI_MATKEY_SHININESS, specularPower);

            Model::MaterialData& materialData = model.materialData.emplace_back();
            materialData.material.ambient = ToColor(ambient);
            materialData.material.diffuse = ToColor(diffuse);
            materialData.material.emissive = ToColor(emissive);
            materialData.material.specular = ToColor(specular);
            materialData.material.power = static_cast<float>(specularPower);

            materialData.diffuseMapName = FindTexture(scene, assimpMaterial, aiTextureType_DIFFUSE, args, "_diff", materialIndex);
            materialData.normalMapName = FindTexture(scene, assimpMaterial, aiTextureType_NORMALS, args, "_norm", materialIndex);
            materialData.bumpMapName = FindTexture(scene, assimpMaterial, aiTextureType_DISPLACEMENT, args, "_bump", materialIndex);
            materialData.specularMapName = FindTexture(scene, assimpMaterial, aiTextureType_SPECULAR, args, "_spec", materialIndex);
        }
    }

    std::printf("Saving model...\n");
    if (ModelIO::SaveModel(args.outputFileName, model))
    {
        std::printf("Save model success...\n");
    }
    else
    {
        std::printf("Failed to save model data [%s]...\n", args.outputFileName.u8string().c_str());
    }

    std::printf("Saving materials...\n");
    if (ModelIO::SaveMaterial(args.outputFileName, model))
    {
        std::printf("Save materials success...\n");
    }
    else
    {
        std::printf("Failed to save materials data...\n");
    }

    return EXIT_SUCCESS;
}