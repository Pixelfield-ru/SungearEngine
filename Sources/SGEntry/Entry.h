#ifndef SUNGEARENGINE_ENTRY_H
#define SUNGEARENGINE_ENTRY_H

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Annotations/Annotations.h>
#include <SGCore/Scene/Serializer.h>
#include <SGCore/Utils/TypeTraits.h>

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
