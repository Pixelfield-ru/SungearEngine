#pragma once
#include <range/v3/detail/prologue.hpp>

#include "Deserialization/DeserializeField.h"
#include "Parser/UISourceTreeView.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/UI/Style/Style.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "Deserialization/ImplDeserializable.h"
#include "Parser/XML/XMLSourceTreeView.h"
#include <unordered_map>
#include <string>

#define SG_DECLARE_UI_ELEMENT_TYPE(name) \
    static consteval size_t getTypeHashStatic() noexcept \
    { \
        static constexpr size_t type_hash = SGCore::constexprHash(#name); \
        return type_hash; \
    } \
    size_t getTypeHash() const noexcept override \
    { \
        static constexpr size_t type_hash = SGCore::constexprHash(#name); \
        return type_hash; \
    }

namespace SGCore::UI
{
    struct UIElementMesh;

    struct UIElement
    {
        virtual ~UIElement() = default;

        friend struct UIDocument;

        Signal<void(UIElement* self)> onPointerDown;
        Signal<void(UIElement* self)> onPointerUp;
        Signal<void(UIElement* self)> onPointerHold;
        Signal<void(UIElement* self)> onPointerHover;

        std::vector<Scope<UIElement>> m_children;
        Weak<UIElement> m_parent;
        std::unordered_set<std::string> m_places;

        Scope<Style> m_style = MakeScope<Style>();

        AssetRef<IShader> m_shader {};

        // not copyable when calling copy()
        Ref<UIElementMesh> m_meshData {};

        /**
         *
         * @return False if was not rendered, true - rendered.
         */
        virtual bool draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                          const Transform& elementTransform,
                          UIElementCache& elementCache) noexcept { return false; }

        /**
         *
         * @param parentElementCache May be nullptr.
         * @param thisLastElementCache
         * @param thisElementCache
         * @param parentTransform May be nullptr.
         * @param ownTransform
         */
        void calculateLayout(const UIElementCache* parentElementCache,
                             const UIElementCache& thisLastElementCache,
                             UIElementCache& thisElementCache,
                             const Transform* parentTransform,
                             Transform& ownTransform) noexcept;

        virtual void useUniforms(UIElementCache& thisElementCache) const noexcept;

        void regenerateMesh(const UIElementCache* parentElementCache,
                            UIElementCache& thisElementCache) noexcept;

        Ref<UIElement> findElement(const std::string& name) const noexcept;
        SGCore::UI::UIElement* const findPlace(const std::string& placeName) const noexcept;

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, UIElement*, UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) noexcept
        {
            func(m_parent.lock().get(), this);
            if(breakToken) return;

            for(const auto& child : m_children)
            {
                child->iterate(func, breakToken);
                if(breakToken) return;
            }
        }

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, const UIElement*, const UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) const noexcept
        {
            func(m_parent.lock().get(), this);
            if(breakToken) return;

            for(const auto& child : m_children)
            {
                child->iterate(func, breakToken);
                if(breakToken) return;
            }
        }

        virtual size_t getTypeHash() const noexcept = 0;

        [[nodiscard]] virtual Scope<UIElement> copy() const noexcept = 0;

        SG_IMPL_DESERIALIZABLE(UIElement);

    private:

        static std::unordered_map<
            std::string_view,
            // This method should create new Derived and place it into target, then try to deserialize value into target pointer
            // (check implementation for example)
            Deserialization::DeserializeIntoResultType(*)(UISourceTreeViewValue&, UIElement*& target, Deserialization::DeserScope& scope)
        > m_registry;

        static Deserialization::DeserializeIntoResultType(*m_createTextComponent)(std::string_view, UIElement*& target, Deserialization::DeserScope& scope);

    public:

        SG_IMPL_DESERIALIZABLE_CONTAINER(UIElement) {
            if (const auto component = value.tryGetComponent()) {
                UIElement* pointer; // MANAGED IN "prepareWithPointer(field, POINTER)"
                const auto& method = m_registry.find(component->getName());
                if (method == m_registry.end()) {
                    return std::format("Unknown component {}", component->getName());
                }
                auto result = method->second(component->getValue(), pointer, scope);
                // Pointer initialized and contains deserialization value (if it was successful)
                Deserialization::IsValidContainer<Container>::prepareWithPointer(field, pointer);
                // DO MANAGE POINTER BEFORE RETURNING A ERROR

                if (result) { return result; } // if not return error;

                return std::nullopt;
            }

            if (auto text = value.tryGetString()) {
                UIElement* pointer; // MANAGED IN "prepareWithPointer(field, POINTER)"
                auto result = m_createTextComponent(*text, pointer, scope);
                // Pointer initialized and contains deserialization value (if it was successful)
                Deserialization::IsValidContainer<Container>::prepareWithPointer(field, pointer);
                // DO MANAGE POINTER BEFORE RETURNING A ERROR

                if (result) { return result; } // if not return error;

                return std::nullopt;
            }

            return "Error: Component or Text expected";
        }

    protected:
        virtual void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                                       const Transform* parentTransform, Transform& ownTransform) = 0;

        /**
         * Generates a mesh based on the selector (i.e. selector != nullptr).\n
         * You must implement this function in the UIElement derived classes.\n
         * You need to generate vertex positions, uv coordinates of vertices, and vertex colors.\n
         * Calls at the beginning of the calculateLayout function.\n
         * Calls only if mesh has not been generated before (i.e. mesh == nullptr).\n
         * IN THIS FUNCTION, YOU DO NOT NEED TO CREATE A MESH INSTANCE. YOU ONLY NEED TO GENERATE VERTICES.
         */
        virtual void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept = 0;

        virtual void doCopy(UIElement& to) const noexcept;

    private:
        void checkForMeshGenerating(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept;
    };

    static_assert(SGCore::UI::Deserialization::ImplDeserializable<UIElement>);
    static_assert(SGCore::UI::Deserialization::ImplDeserializableContainer<Scope<UIElement>>);


}