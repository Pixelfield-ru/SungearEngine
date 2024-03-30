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