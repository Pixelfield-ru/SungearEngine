//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIELEMENTATTRIBUTETYPE_H
#define SUNGEARENGINE_UIELEMENTATTRIBUTETYPE_H

namespace SGCore::UI
{
    enum struct UIElementAttributeType
    {
        AT_CLASS,   // for any element
        AT_TYPE,    // for 'include' element
        AT_PATH     // for 'include' element
    };
}

#endif //SUNGEARENGINE_UIELEMENTATTRIBUTETYPE_H
