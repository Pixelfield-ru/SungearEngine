#ifndef SUNGEARENGINEEDITOR_META_H
#define SUNGEARENGINEEDITOR_META_H

#include <filesystem>

#ifdef _MSC_VER
#define DLEXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define DLEXPORT __attribute__((visibility("default")))
#endif

// THIS FIELD CAN NOT BE NULLPTR. WILL ALWAYS BE SET TO REAL PATH OF YOUR PROJECT
// DO NOT CHANGE!
extern "C" DLEXPORT std::filesystem::path* myProjectPath = nullptr;

// ENTRY
extern "C" DLEXPORT void addMetaInfo();

#endif //SUNGEARENGINEEDITOR_META_H
