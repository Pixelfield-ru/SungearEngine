//
// Created by Ilya on 25.11.2023.
//

#include "PBRFRPCamerasCollector.h"
#include "PBRFRPCamera.h"

SGCore::PBRFRPCamerasCollector::PBRFRPCamerasCollector()
{
    m_componentsCollector.configureCachingFunction<PBRFRPCamera, Transform>();
}
