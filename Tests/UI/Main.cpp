#include "Main.h"

#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>

#include "SGCore/UI/Deserialization/DeserializeField.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/UI/Parser/XML/XMLSourceTreeView.h"

#include <iostream>
#include <expected>

struct Test2
{
    #define deser_struct Test2
    #define deser_properties(prop) \
        prop(test) \
        prop(test2)

    int m_test = -1;
    int m_test2 = -1;

    template<typename UISourceTreeViewValue> requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue>
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, deser_struct& field) {

        std::optional<typename UISourceTreeViewValue::UISourceTreeViewObject> valueAsMaybeObject = value.tryGetObject();
        if (!valueAsMaybeObject.has_value()) { return "wrong object type"; }
        auto object = *valueAsMaybeObject;

        {
            auto iter = object.properties();
            for (auto prop : iter) {
                auto propName = prop.getName();

                if (propName == "test") {
                    if (auto error = SGCore::UI::Deserialization::Deserializer<UISourceTreeViewValue, decltype(m_test)>::deserializeInto(prop.getValue(), field.m_test)) {
                        return std::format("Error when parsing field {}.{}, error: {}", typeid(deser_struct).name(), "test", *error);
                    }

                    continue;
                }

                if (propName == "test2") {
                    if (auto error = SGCore::UI::Deserialization::Deserializer<UISourceTreeViewValue, decltype(m_test2)>::deserializeInto(prop.getValue(), field.m_test2)) {
                        return std::format("Error when parsing field {}.{}, error: {}", typeid(deser_struct).name(), "test2", *error);
                    }

                    continue;
                }

                return std::format("Unknown property {}", propName);
            }
        }
        /*if (!SGCore::UI::Deserializer<UISourceTreeViewValue, decltype(m_test)>::deserializeInto(object.getProperty("test"), field.m_test))
        { return false; }
        if (!SGCore::UI::Deserializer<UISourceTreeViewValue, decltype(m_test2)>::deserializeInto(object.getProperty("test2"), field.m_test2))
        { return false; }*/
        return std::nullopt;
    }
};

struct Test
{
    #undef deser_struct
    #define deser_struct Test
    #define deser_child_prop_require(prop) prop(objTest)

    Test2 m_objTest;

    template<typename UISourceTreeViewValue> requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue>
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, deser_struct& field) {
        std::optional<typename UISourceTreeViewValue::UISourceTreeViewObject> valueAsMaybeObject = value.tryGetObject();
        if (!valueAsMaybeObject.has_value()) { return "wrong object type"; }
        auto object = *valueAsMaybeObject;

        {
            auto objectChild = SGCore::UI::UITreeUtils::getSingleChildOfObject<UISourceTreeViewValue>(object);
            if (!objectChild.has_value()) {
                return std::format("Error when parsing {}: {}", "objTest", objectChild.error());
            }

            if (auto childDeserResult = decltype(m_objTest)::deserializeInto(*objectChild, field.m_objTest)) {
                return std::format("Error when parsing {}: {}", "objTest", *childDeserResult);
            }
        }


        return std::nullopt;
    }
};

std::string testString = "<object><property id='123' name='test'>5</property></object>";

void coreInit()
{
    Test test {};

    const auto xmlTree = SGCore::UI::XMLSourceTreeView::create(testString);
    auto deserResult = Test::deserializeInto(xmlTree.getRoot(), test);
    std::cout << "DESER RESULT HAS ERROR: " << (deserResult.has_value() ? "true" : "false") << std::endl;
    if (deserResult) {
        std::cout << "Parsing error: " << *deserResult << std::endl;
    }
    std::cout << "val " << test.m_objTest.m_test << std::endl;
}

void onUpdate(const double& dt, const double& fixedDt)
{

}

void onFixedUpdate(const double& dt, const double& fixedDt)
{

}

int main()
{
    SGCore::CoreMain::onInit.connect<&coreInit>();
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    SGCore::CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    SGCore::CoreMain::init();
    SGCore::CoreMain::startCycle();

    return 0;
}
