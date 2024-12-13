//
// Created by stuka on 13.12.2024.
//

#ifndef SUNGEARENGINE_SGSLESUBSHADER_H
#define SUNGEARENGINE_SGSLESUBSHADER_H

#include "SGCore/Utils/SGSL/SGSLESubShaderType.h"
#include "SGCore/Serde/Defines.h"
#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct SGSLESubShader
    {
        sg_serde_as_friend();

        friend struct SGSLETranslator;
        friend struct ShaderAnalyzedFile;

        [[nodiscard]] auto getType() const noexcept
        {
            return m_type;
        }

        [[nodiscard]] const auto& getCode() const noexcept
        {
            return m_code;
        }

    private:
        SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
        std::string m_code;

        size_t m_codeOffsetInPackage = 0;
        size_t m_codeSizeInPackage = 0;
    };
}

#endif //SUNGEARENGINE_SGSLESUBSHADER_H
