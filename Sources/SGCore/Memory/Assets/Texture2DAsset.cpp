//
// Created by stuka on 25.05.2023.
//

//#include "Texture2DAsset.h"

#include "Texture2DAsset.h"
#include "SGCore/Logging/Log.h"
#include "SGCore/Utils/FileSmartPointer.h"

#include <stb/stb_image.h>

void Core::Memory::Assets::Texture2DAsset::load(const std::string_view& path)
{
    int width = 0;
    int height = 0;
    int channelsInFile = 0;
    int channelsDesired = 0;


    Core::Utils::FileSmartPointer fileSmartPointer(path.data(), "r");

    if(!fileSmartPointer.get())
    {
        Core::Logging::consolePrintf(Logging::MessageType::SG_ERROR, "Texture file can not be open! (path: " + std::string(path.data()) + ")");
    }

    //stbi_uc* textureData = stbi_load_from_file(fileSmartPointer.get(), &width, &height, &channelsInFile, channelsDesired);

    if(fclose(fileSmartPointer.get()) == -1)
    {
        Core::Logging::consolePrintf(Logging::MessageType::SG_ERROR, "Texture file can not be open! (path: " + std::string(path.data()) + ")");
    }
}

