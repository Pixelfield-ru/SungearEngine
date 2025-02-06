//
// Created by stuka on 06.02.2025.
//

#ifndef TRANSFORMUTILS_H
#define TRANSFORMUTILS_H

namespace SGCore
{
    struct Transform;

    struct TransformUtils
    {
        /**
         * Calculates TRS matrices in childTransform for m_ownTransform and for m_finalTransform
         * @param childTransform
         * @param parentTransform Parent transform of childTransform. Can be nullptr (child does not have parent)
         * @return Is m_ownTransform of childTransform has been changed.
         */
        static bool calculateTransform(Transform& childTransform, const Transform* parentTransform) noexcept;
    };
}


#endif //TRANSFORMUTILS_H
