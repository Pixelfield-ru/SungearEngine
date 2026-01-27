#include "Main.h"

#include "SGCore/Main/CoreMain.h"
#include <SGCore/UI/UIElement.h>
#include <SGCore/UI/Deserialization/DeserializeField.h>

#include "SGCore/UI/Elements/Div.h"
#include "SGCore/UI/Elements/UIRoot.h"

std::string testString = "<root><div>Test text <div>Other text</div></div></root>";

void coreInit()
{
    // SGCore::Scope<SGCore::UI::UIElement> test = std::make_unique<SGCore::UI::Div>();

    auto source = SGCore::UI::XMLSourceTreeView::create(testString);
    SGCore::Scope<SGCore::UI::UIRoot> uiRoot;
    SGCore::UI::Deserialization::Deserializer<
        SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue,
        SGCore::Scope<SGCore::UI::UIRoot>
    >::deserializeInto(
        source.getRoot(),
        uiRoot,
        SGCore::UI::Deserialization::DeserScope<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue>(nullptr)
    );


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
