#include "Meta.h"
#include <SGCore/MetaInfo/MetaInfo.h>

extern "C" DLEXPORT void addMetaInfo()
{
    /**
     *
     *
     * ADD YOUR META TO DESCRIBE STRUCT LIKE THIS:
     * - code
     * {
     *      Meta meta;
     *      meta["fullName"] = "MyNamespace::MyStruct"; // REQUIRED FIELD
     *      meta["filePath"] = *myProjectPath / "Sources/Path/To/FileWithStruct.h"; // REQUIRED FIELD
     *      meta["type"] = "component"; // OPTIONAL FIELD
     *
     *      meta["members"]["m_myMember"]; // 'members' IS OPTIONAL FIELD
     *
     *      SGCore::MetaInfo::getMeta()["structs"].push_back(meta); // REQUIRED LINE
     * }
     * - end code
     */
}