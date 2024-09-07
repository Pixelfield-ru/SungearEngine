//
// Created by stuka on 19.01.2024.
//

#include "MeshBase.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "RenderPipelinesManager.h"

SGCore::MeshBase::MeshBase()
{
    m_meshData = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());
}

void SGCore::MeshBase::setMeshData(const SGCore::Ref<SGCore::IMeshData>& meshData) noexcept
{
    m_meshData = meshData;
    if(!m_material)
    {
        m_material = meshData->m_material;
    }
}

SGCore::Ref<SGCore::IMeshData> SGCore::MeshBase::getMeshData() const noexcept
{
    return m_meshData;
}

void SGCore::MeshBase::setMaterial(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    m_material = material;
}

SGCore::Ref<SGCore::IMaterial> SGCore::MeshBase::getMaterial() const noexcept
{
    return m_material;
}


