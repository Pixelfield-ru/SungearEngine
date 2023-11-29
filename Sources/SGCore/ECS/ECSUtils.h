//
// Created by stuka on 29.11.2023.
//

#ifndef SUNGEARENGINE_ECSUTILS_H
#define SUNGEARENGINE_ECSUTILS_H

#define SG_BEGIN_ITERATE_CACHED_ENTITIES(cachedEntities, layerName, entityName)                         \
             for(auto& layerName ## Iter : cachedEntities)                                              \
             {                                                                                          \
                auto& layerName = layerName ## Iter.second;                                             \
                for(auto& entityName ## Iter : layerName)                                               \
                {                                                                                       \
                    auto& entityName = entityName ## Iter.second;

#define SG_END_ITERATE_CACHED_ENTITIES }}

#endif //SUNGEARENGINE_ECSUTILS_H
