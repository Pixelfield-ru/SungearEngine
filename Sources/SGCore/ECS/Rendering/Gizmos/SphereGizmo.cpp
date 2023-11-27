#include "SphereGizmo.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

SGCore::SphereGizmo::SphereGizmo()
{
    m_meshData = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());
}
