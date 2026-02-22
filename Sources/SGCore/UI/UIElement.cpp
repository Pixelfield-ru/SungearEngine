#include "UIElement.h"

#include "Elements/Div.h"
#include "Elements/Text.h"
#include "UIElementMesh/UIElementMesh.h"

void SGCore::UI::UIElement::useUniforms(UIElementCache& thisElementCache) const noexcept
{
    if(!m_shader) return;

    auto& elementCache = thisElementCache;

    if(m_meshData)
    {
        m_shader->useVectorf("u_meshAABB", glm::vec4 {
            m_meshData->m_aabb.m_min.x,
            m_meshData->m_aabb.m_min.y,
            m_meshData->m_aabb.m_max.y,
            m_meshData->m_aabb.m_max.y,
        });
    }

    m_shader->useVectorf("u_elementSize", elementCache.m_finalSize);

    m_shader->useVectorf("u_backgroundColor", elementCache.m_backgroundColor);

    elementCache.m_totalBorderSize.x = std::ranges::max({
        elementCache.m_borderRadiusCache[0].x,
        elementCache.m_borderRadiusCache[1].x,
        elementCache.m_borderRadiusCache[2].x,
        elementCache.m_borderRadiusCache[3].x
    });

    elementCache.m_totalBorderSize.y = std::ranges::max({
        elementCache.m_borderRadiusCache[0].y,
        elementCache.m_borderRadiusCache[1].y,
        elementCache.m_borderRadiusCache[2].y,
        elementCache.m_borderRadiusCache[3].y
    });

    m_shader->useVectorf("u_totalBorderSize", elementCache.m_totalBorderSize);

    m_shader->useInteger("u_layer", elementCache.m_layer);
}

void SGCore::UI::UIElement::calculateLayout(const UIElementCache* parentElementCache,
                                            const UIElementCache& thisLastElementCache,
                                            UIElementCache& thisElementCache,
                                            const Transform* parentTransform,
                                            Transform& ownTransform) noexcept
{
    const bool isMeshGenerated = m_meshData != nullptr;

    checkForMeshGenerating(parentElementCache, thisElementCache);

    doCalculateLayout(parentElementCache, thisElementCache, parentTransform, ownTransform);

    // ======================================== next are the conditions under which the mesh will be regenerated

    if(!isMeshGenerated) return;

    if(thisLastElementCache.m_borderRadiusCache[0] != thisElementCache.m_borderRadiusCache[0] ||
       thisLastElementCache.m_borderRadiusCache[1] != thisElementCache.m_borderRadiusCache[1] ||
       thisLastElementCache.m_borderRadiusCache[2] != thisElementCache.m_borderRadiusCache[2] ||
       thisLastElementCache.m_borderRadiusCache[3] != thisElementCache.m_borderRadiusCache[3])
    {
        regenerateMesh(parentElementCache, thisElementCache);
    }
}

void SGCore::UI::UIElement::regenerateMesh(const UIElementCache* parentElementCache,
                                           UIElementCache& thisElementCache) noexcept
{
    if(thisElementCache.m_currentFrameStyles.empty()) return;

    if(m_meshData)
    {
        m_meshData->m_indices.clear();
        m_meshData->m_vertices.clear();
    }

    for(auto* style : thisElementCache.m_currentFrameStyles)
    {
        style->calculateCache(parentElementCache, thisElementCache);
    }

    doGenerateMesh(parentElementCache, thisElementCache);

    if(m_meshData)
    {
        m_meshData->update();
        m_meshData->m_aabb.calculate(m_meshData->m_vertices);
    }
}

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIElement::findElement(const std::string& name) const noexcept
{
    for(const auto& child : m_children)
    {
        // TODO
        // if(child->m_name == name) return child;

        if(auto found = child->findElement(name)) return found;
    }

    return nullptr;
}

SGCore::UI::UIElement* const SGCore::UI::UIElement::findPlace(const std::string& placeName) const noexcept
{
    for(const auto& child : m_children)
    {
        if(child->m_places.contains(placeName)) return child.get();

        if(auto found = child->findPlace(placeName)) return found;
    }

    return nullptr;
}

template<typename T>
auto createRegistryEntry() {
    return [](SGCore::UI::UISourceTreeViewValue& value, SGCore::UI::UIElement*& element, SGCore::UI::Deserialization::DeserScope& scope) -> SGCore::UI::Deserialization::DeserializeIntoResultType {
        element = new T{};
        auto divPointer = dynamic_cast<T*>(element); // TODO: static_cast?

        return SGCore::UI::Deserialization::Deserializer<T>::deserializeInto(value, *divPointer, scope);
    };
}

decltype(SGCore::UI::UIElement::m_registry) SGCore::UI::UIElement::m_registry = {
    {"div", createRegistryEntry<Div>()}
};

decltype(SGCore::UI::UIElement::m_createTextComponent) SGCore::UI::UIElement::m_createTextComponent =
    [](std::string_view text, UIElement*& element, Deserialization::DeserScope& scope) -> Deserialization::DeserializeIntoResultType {
        element = new Text();
        auto textPointer = dynamic_cast<Text*>(element);
        textPointer->m_text = text;

        return std::nullopt;
};

void SGCore::UI::UIElement::doCopy(UIElement& to) const noexcept
{
    to.m_style = MakeScope<Style>(*m_style);
    // to->m_name = m_name;

    to.m_shader = m_shader;

    to.m_places = m_places;

    for(const auto& child : m_children)
    {
        to.m_children.push_back(child->copy());
    }
}

void SGCore::UI::UIElement::checkForMeshGenerating(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept {
    if(!m_meshData && !thisElementCache.m_currentFrameStyles.empty())
    {
        m_meshData = MakeRef<UIElementMesh>();

        for(auto* style : thisElementCache.m_currentFrameStyles)
        {
            style->calculateCache(parentElementCache, thisElementCache);
        }

        doGenerateMesh(parentElementCache, thisElementCache);

        m_meshData->prepare();

        if(m_meshData)
        {
            m_meshData->m_aabb.calculate(m_meshData->m_vertices);
        }
    }
}

/*#define sg_deser_type SGCore::UI::UIElement
#define sg_deser_children m_children
#define sg_deser_properties(prop) prop(style)
#include "Deserialization/ImplDeserializableStruct.h"*/