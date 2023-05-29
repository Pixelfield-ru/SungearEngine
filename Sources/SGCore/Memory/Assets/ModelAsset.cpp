//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::ModelAsset::load(const std::string_view&)
{
    return shared_from_this();
}