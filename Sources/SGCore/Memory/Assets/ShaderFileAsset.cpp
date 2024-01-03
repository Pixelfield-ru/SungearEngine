//
// Created by Ilya on 18.09.2023.
//

#include "ShaderFileAsset.h"
#include "SGCore/Main/CoreMain.h"

SGCore::Ref<SGCore::IAsset> SGCore::ShaderFileAsset::load(const std::string& path)
{
    return shared_from_this();
}
