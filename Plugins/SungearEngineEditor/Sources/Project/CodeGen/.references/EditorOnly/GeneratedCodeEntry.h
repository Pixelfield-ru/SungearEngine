#ifndef GENERATEDCODECONNECTION_H
#define GENERATEDCODECONNECTION_H

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Utils/Utils.h>

void onInit();
void update(const double& dt, const double& fixedDt);
void fixedUpdate(const double& dt, const double& fixedDt);
void onInspectorViewRender();

SG_NOMANGLING SG_DLEXPORT void EditorGeneratedCodeEntry();
SG_NOMANGLING SG_DLEXPORT void EditorGeneratedCodeExit();

#endif // GENERATEDCODECONNECTION_H
