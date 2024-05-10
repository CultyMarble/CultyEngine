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

    return EXIT_SUCCESS;
}