//
// Created by stuka on 01.05.2025.
//

#ifndef SUNGEARENGINE_PBRRPDECALSPASS_H
#define SUNGEARENGINE_PBRRPDECALSPASS_H

#include "SGCore/Render/BaseRenderPasses/IDecalsPass.h"

namespace SGCore
{
    struct PBRRPDecalsPass : public IDecalsPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        AssetRef<IShader> m_instancingShader;
    };
}

#endif // SUNGEARENGINE_PBRRPDECALSPASS_H
