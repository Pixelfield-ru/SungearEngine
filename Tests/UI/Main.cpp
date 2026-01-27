#include "Main.h"

#include "SGCore/Main/CoreMain.h"
#include <SGCore/UI/UIElement.h>
#include <SGCore/UI/Deserialization/DeserializeField.h>

#include "SGCore/UI/Elements/Div.h"
#include "SGCore/UI/Elements/Text.h"
#include "SGCore/UI/Elements/UIRoot.h"

std::string testString = "<div>Test text <div>Other text</div></div>";

void coreInit()
{
    pugi::xml_document doc;
    doc.load_string(testString.data());

    SGCore::UI::XML::XMLSourceTreeViewHandler handler {doc.root()};

    SGCore::UI::UIRoot root;
    auto result = SGCore::UI::Deserialization::Deserializer<SGCore::UI::UIRoot>::deserializeInto(handler.getRoot(), root, SGCore::UI::Deserialization::DeserScope{nullptr});
    if (result) {
        std::cout << "Deserialize failed!" << std::endl;
        std::cout << *result << std::endl;
    } else {
        bool tok = false;
        root.iterate([](SGCore::UI::UIElement* parent, SGCore::UI::UIElement* elem) {
            std::cout << typeid(*elem).name() << std::endl;
        }, tok);
        std::cout << root.m_children.size() << std::endl;
    }


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
