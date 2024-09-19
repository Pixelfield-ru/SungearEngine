//
// Created by stuka on 20.09.2024.
//

struct Annotation
{
    Annotation() = default;
    Annotation(const Annotation&) noexcept = default;
    Annotation(Annotation&&) noexcept = default;
    /*explicit Annotation(const std::string& value)
    {
        m_value = value;
    }*/

    Annotation& operator[](const std::string& childName) noexcept
    {
        auto it = std::find_if(m_children.begin(), m_children.end(), [&childName](const auto& child) {
            return childName == child.first;
        });

        if(it == m_children.end())
        {
            m_children.push_back({ childName, { } });
            return m_children.rbegin()->second;
        }
        else
        {
            return it->second;
        }
    }

    Annotation& operator=(const std::string& value) noexcept
    {
        m_value = value;
        return *this;
    }

    Annotation& operator=(const Annotation&) noexcept = default;
    Annotation& operator=(Annotation&&) noexcept = default;

private:
    std::string m_value;
    std::vector<std::pair<std::string, Annotation>> m_children;
};

std::unordered_map<std::string, std::vector<Annotation>> annotations;

void gg()
{
    {
        Annotation myStruct;
        myStruct["fullName"] = "TestNamespace::MyStruct";
        myStruct["type"] = "component";
        myStruct["templates"]["T"] = "typename";

        myStruct["members"]["m_myMember"];

        myStruct["functions"]["getMyMember"]["getterFor"] = "m_myMember";
        myStruct["functions"]["setMyMember"]["setterFor"] = "m_myMember";

        annotations["structs"].push_back(myStruct);
    }
}
