#include "Scope.h"

//void SGCore::CodeGen::Generator::addBuiltinVariables() noexcept
//{
//    // adding structs from meta info
//    m_baseScope["structs"] = std::make_shared<Lang::Table>(Lang::Table());
//
//    size_t currentStructIdx = 0;
//    for (auto& metaStruct : MetaInfo::getMeta()["structs"])
//    {
//        // adding all variables of meta
//        std::shared_ptr<Lang::Table> newStruct = metaToObject(metaStruct);
//
//        // adding some other builtin members of struct ==============================================
//
//        // adding baseTypes of struct ==========
//
//        auto& structExtends = metaStruct["baseTypes"];
//
//        for (const auto& baseType : structExtends.getChildren())
//        {
//            (*newStruct)["baseTypes"][baseType->first].scast<Lang::String>()->m_value = baseType->first;
//            // if base type struct was not created then create base type struct
//            if (m_baseScope.getTable("structs")->hasMember(baseType->first))
//            {
//                auto newBaseStruct = std::make_shared<Lang::Table>(Lang::Table());
//
//                (*newBaseStruct)["derivedTypes"] = std::make_shared<Lang::Table>(Lang::Table());
//
//                (*newBaseStruct)["derivedTypes"][metaStruct["fullName"].getValue()].scast<Lang::String>()->m_value = metaStruct["fullName"].getValue();
//
//                m_baseScope.getTable("structs")->setMember(baseType->first, newBaseStruct);
//            }
//            else // if base type struct was already created then taking existing struct
//            {
//                auto& newBaseStruct = *(m_baseScope.getTable("structs")->getMember(baseType->first));
//
//                newBaseStruct["derivedTypes"] = std::make_shared<Lang::Table>(Lang::Table());
//                newBaseStruct["derivedTypes"][metaStruct["fullName"].getValue()].scast<Lang::String>()->m_value = metaStruct["fullName"].getValue();
//            }
//        }
//        // =====================================
//
//        auto& structTemplateArgs = metaStruct["template_args"];
//
//        // doing some actions with templates ==================
//        // structFullNameWithTemplates == structName or structFullNameWithTemplates == structName<allTemplateArgs> (if template exists)
//        std::string structFullNameWithTemplates = metaStruct["fullName"].getValue();
//        std::string structTemplateDecl;
//        const bool hasTemplateArgs = !structTemplateArgs.getChildren().empty();
//        if (hasTemplateArgs)
//        {
//            structFullNameWithTemplates += '<';
//        }
//        for (size_t i = 0; i < structTemplateArgs.getChildren().size(); ++i)
//        {
//            const auto& structTemplateArg = structTemplateArgs.getChildren()[i];
//            structFullNameWithTemplates += structTemplateArg->first;
//            structTemplateDecl += structTemplateArg->second.getValue() + " " + structTemplateArg->first;
//
//            if (i < structTemplateArgs.getChildren().size() - 1)
//            {
//                structFullNameWithTemplates += ", ";
//                structTemplateDecl += ", ";
//            }
//        }
//        if (hasTemplateArgs)
//        {
//            structFullNameWithTemplates += '>';
//        }
//        // =====================================================
//
//        (*newStruct)["fullNameWithTemplate"].scast<Lang::String>()->m_value = structFullNameWithTemplates;
//        if (!structTemplateDecl.empty())
//        {
//            (*newStruct)["templateDecl"].scast<Lang::String>()->m_value = structTemplateDecl;
//        }
//
//        m_baseScope.getTable("structs")->setMember(metaStruct["fullName"].getValue(), newStruct);
//
//        ++currentStructIdx;
//        // newStruct.m_members["fullNameWithTemplates"].m_value = structFullNameWithTemplates;
//    }
//}

//std::shared_ptr<SGCore::CodeGen::Lang::Table> SGCore::CodeGen::Generator::metaToObject(SGCore::MetaInfo::Meta& meta)
//{
//    auto newTable = std::make_shared<Lang::Table>(Lang::Table());
//
//    for (auto& p : meta.getChildren())
//    {
//        if (p->second.getChildren().empty()) {
//            auto newStr = std::make_shared<Lang::String>(Lang::String());
//            newStr->m_value = meta.getValue();
//            (*newTable)[p->first] = newStr;
//        }
//        else {
//            (*newTable)[p->first] = metaToObject(p->second);
//        }
//    }
//
//    return newTable;
//}

bool SGCore::CodeGen::Lang::Any::instanceof(const SGCore::CodeGen::Lang::Type& other) const noexcept
{
    if (m_extends.empty()) return false;

    auto it = std::find_if(m_extends.begin(), m_extends.end(), [&other](const Type& t) {
        return t.m_name == other.m_name;
        });

    if (it == m_extends.end())
    {
        for (const auto& t : m_extends)
        {
            return t.instanceof(other);
        }
    }

    return true;
}

bool SGCore::CodeGen::Lang::Any::instanceof(const std::string& typeName) const noexcept
{
    if (m_extends.empty()) return false;

    auto it = std::find_if(m_extends.begin(), m_extends.end(), [&typeName](const Type& t) {
        return t.m_name == typeName;
        });

    if (it == m_extends.end())
    {
        for (const auto& t : m_extends)
        {
            return t.instanceof(typeName);
        }
    }

    return true;
}

std::shared_ptr<SGCore::CodeGen::Lang::Any> SGCore::CodeGen::Lang::Table::getMember(const std::string& name) noexcept
{
    if (m_members.empty()) return nullptr;

    auto it = m_members.find(name);

    return it->second;
}

//SGCore::CodeGen::Lang::Table::Proxy& SGCore::CodeGen::Lang::Table::operator[](const std::string& memberName) noexcept
//{
//    if (hasMember(memberName))
//    {
//        return *m_members[memberName];
//    }
//    else
//    {
//        auto newMember = std::make_shared<Table>();
//        m_members[memberName] = newMember;
//        return *newMember;
//    }
//}

bool SGCore::CodeGen::Lang::Table::hasMember(const std::string& memberName) const noexcept
{
    return m_members.contains(memberName);
}

void SGCore::CodeGen::Lang::Table::removeMember(const std::string& memberName) noexcept
{
    m_members.erase(memberName);
}

size_t SGCore::CodeGen::Lang::Table::membersCount() const noexcept
{
    return m_members.size();
}

const std::unordered_map<std::string, std::shared_ptr<SGCore::CodeGen::Lang::Any>>&
SGCore::CodeGen::Lang::Table::getMembers() const noexcept
{
    return m_members;
}

//void SGCore::CodeGen::Lang::Table::setMemberPtr(const std::string& memberName, const std::shared_ptr<Table>& value) noexcept
//{
//    m_members[memberName] = value;
//}

