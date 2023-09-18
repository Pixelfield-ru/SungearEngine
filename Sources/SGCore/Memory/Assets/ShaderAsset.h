//
// Created by Ilya on 18.09.2023.
//

#ifndef SUNGEARENGINE_SHADERASSET_H
#define SUNGEARENGINE_SHADERASSET_H

#include "IAsset.h"

#include "SGCore/Graphics/API/IShader.h"

namespace Core::Memory::Assets
{
    class ShaderAsset : public IAsset, public std::enable_shared_from_this<ShaderAsset>
    {
    private:
        std::shared_ptr<Graphics::IShader> m_shader;

    public:
        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        std::shared_ptr<Graphics::IShader> getShader() const noexcept;
    };
}

#endif //SUNGEARENGINE_SHADERASSET_H
