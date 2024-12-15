//
// Created by stuka on 19.11.2023.
//
#include "Paths.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IShader.h"

#include <memory>

void SGCore::Paths::init() noexcept
{
    m_defaultPaths = MakeRef<Paths>();

    m_defaultPaths->addPath("Shaders/Gizmos/ComplexGizmosShader",
                            "${enginePath}/Resources/sg_shaders/features/gizmos/complex_gizmos.sgshader"
    );

    m_defaultPaths->addPath("Shaders/Gizmos/LinesGizmosShader",
                            "${enginePath}/Resources/sg_shaders/features/gizmos/lines.sgshader"
    );

    m_defaultPaths->addPath("Shaders/LayeredPP/LayerFXShader",
                            "${enginePath}/Resources/sg_shaders/features/postprocessing/layered/layer_default_fx.sgshader"
    );

    m_defaultPaths->addPath("Shaders/InfiniteGrid",
                            "${enginePath}/Resources/sg_shaders/features/gizmos/infinite_grid.sgshader"
    );

    m_defaultPaths->addPath("Shaders/StandardOutline",
                            "${enginePath}/Resources/sg_shaders/features/outline.sgshader"
    );
}

SGCore::InterpolatedPath SGCore::Paths::addPath(const std::string& virtualPath,
                                                const InterpolatedPath& path) noexcept
{
    auto foundIt = m_paths.find(virtualPath);
    if(foundIt != m_paths.end()) return foundIt->second;

    m_paths[virtualPath] = path;

    return path;
}

std::optional<SGCore::InterpolatedPath> SGCore::Paths::operator[](const std::string& virtualPath) noexcept
{
    auto foundIt = m_paths.find(virtualPath);
    if(foundIt == m_paths.end()) return std::nullopt;

    return foundIt->second;
}
