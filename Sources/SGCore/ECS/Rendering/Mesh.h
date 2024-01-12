#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ECS/IComponent.h"
#include "SGCore/Main/CoreMain.h"
#include "Pipelines/IRenderPipeline.h"
// #include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct Mesh : public IComponent
    {
        Mesh() = default;

        explicit Mesh(const Ref<IRenderPipeline>& pipeline) noexcept;

        MeshDataRenderInfo m_meshDataRenderInfo;
        Ref<IMeshData> m_meshData = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());

    protected:
        void loadShader(const Ref<IRenderPipeline>& pipeline, const std::string& shaderPath) noexcept;

        std::string m_shaderPath = "GeometryShader";

    private:
        void init() noexcept override { }
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
