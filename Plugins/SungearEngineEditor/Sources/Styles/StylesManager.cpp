//
// Created by stuka on 04.09.2024.
//

#include "StylesManager.h"

#include "Styles/Editor/Dark.h"

void SGE::StylesManager::init() noexcept
{
    SGCore::Ref<IStyle> darkStyle = SGCore::MakeRef<EditorStyles::Dark>();
    setCurrentStyle(darkStyle);

    m_styles.push_back(darkStyle);
}

void SGE::StylesManager::setCurrentStyle(const SGCore::Ref<IStyle>& style) noexcept
{
    m_currentStyle = style;
    m_currentStyle->apply();
}

SGCore::Ref<SGE::IStyle> SGE::StylesManager::getCurrentStyle() noexcept
{
    return m_currentStyle;
}

SGCore::Ref<SGE::IStyle> SGE::StylesManager::getStyle(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_styles.begin(), m_styles.end(), [&name](const SGCore::Ref<IStyle>& style) {
        return style->m_name == name;
    });

    return foundIt != m_styles.end() ? *foundIt : nullptr;
}

auto& SGE::StylesManager::getStyles() noexcept
{
    return m_styles;
}
