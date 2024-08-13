//
// Created by ilya on 27.06.24.
//

#include "Formatter.h"
#include "Utils.h"

std::string SGCore::Formatter::format(const std::string& text) const
{
    std::string newText = text;
    
    for(const auto& p : m_markup)
    {
        std::string value;
        
        if(auto* valI = std::any_cast<std::int64_t>(&p.second))
        {
            value = std::to_string(*valI);
        }
        else if(auto* valUI = std::any_cast<std::uint64_t>(&p.second))
        {
            value = std::to_string(*valUI);
        }
        else if(auto* valD = std::any_cast<double>(&p.second))
        {
            value = std::to_string(*valD);
        }
        else if(auto* valS = std::any_cast<std::string>(&p.second))
        {
            value = *valS;
        }
        else if(auto* valCH = std::any_cast<const char*>(&p.second))
        {
            value = *valCH;
        }
        else
        {
            throw std::runtime_error(fmt::format("Key '{0}' has value with inacceptable type '{1}'", p.first, p.second.type().name()));
        }
        
        newText = Utils::replaceAll(newText, m_keyOpenExpr + p.first + m_keyCloseExpr, value);
    }
    
    return newText;
}

void SGCore::Formatter::clearMarkup() noexcept
{
    m_markup.clear();
}

std::any& SGCore::Formatter::operator[](const std::string& key) noexcept
{
    return m_markup[key];
}
