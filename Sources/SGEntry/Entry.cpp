#include "Entry.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/RenderingBase.h"

#include <SGCore/Input/InputManager.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Memory/Assets/ModelAsset.h>

#ifdef PLATFORM_OS_WINDOWS
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
    __declspec(dllexport) DWORD NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif
#endif

#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/ImGuiWrap/Views/IView.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Utils/Formatter.h"
#include "SGCore/CodeGeneration/CodeGenerator.h"

#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"

#include <antlr4-runtime.h>
#include "SGCore/UI/ANTLR4CSS3Generated/css3Lexer.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3ParserBaseListener.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3ParserListener.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3ParserBaseVisitor.h"

/*class MyCSSListener : public css3ParserBaseListener {
public:
    void enterDeclaration(css3Parser::DeclarationContext* ctx) override {
        std::cout << "Property: " << ctx->property()->getText()
                  << ", Value: " << ctx->expr()->getText() << std::endl;
    }
};*/

using Styles = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

class CSSListener : public css3ParserBaseListener {
public:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> styles;

    std::string currentSelector;

    // Вызывается при входе в ruleset
    void enterSelector(css3Parser::SelectorContext* ctx) override {
        currentSelector = ctx->children[0]->children[0]->children[1]->getText(); // Извлекаем селектор
    }

    // Вызывается при входе в declaration
    void enterKnownDeclaration(css3Parser::KnownDeclarationContext* ctx) override {
        if (currentSelector.empty()) return;

        std::string property = ctx->property_()->getText();
        std::string value = ctx->expr()->getText();

        styles[currentSelector][property] = value;
    }

    void enterColor(css3Parser::ColorContext* ctx) override {
        auto components = ctx->color_component();
        std::string red = components[0]->getText();
        std::string green = components[1]->getText();
        std::string blue = components[2]->getText();
        std::string alpha = ctx->color_alpha() ? ctx->color_alpha()->getText() : "1.0";

        std::cout << "Parsed color: R=" << red
                  << ", G=" << green
                  << ", B=" << blue
                  << ", A=" << alpha
                  << std::endl;
    }

    const auto& getStyles() const {
        return styles;
    }
};

void parseCSS(const std::string& cssContent) {
    // Создаём входной поток
    antlr4::ANTLRInputStream input(cssContent);

    // Создаём лексер
    css3Lexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    // Создаём парсер
    css3Parser parser(&tokens);

    // Парсим CSS (начальное правило грамматики)
    css3Parser::StylesheetContext* tree = parser.stylesheet();

    // css3Parser::SelectorContext* context = parser.selector();

    CSSListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    const auto& styles = listener.getStyles();
    for (const auto& [selector, properties] : styles) {
        std::cout << "Selector: " << selector << std::endl;
        for (const auto& [property, value] : properties) {
            std::cout << "  " << property << ": " << value << ";" << std::endl;
        }
    }

    // Хранилище для стилей
    /*Styles styles;

    for (auto child : tree->children) {
        // Проверяем, является ли узел NestedStatementContext
        auto nestedStatement = dynamic_cast<css3Parser::NestedStatementContext*>(child);
        if (!nestedStatement) continue;

        // Ищем ruleset
        auto ruleset = dynamic_cast<css3Parser::RulesetContext*>(nestedStatement->children[0]);
        if (!ruleset) continue;

        // Извлечение селектора
        std::string selector;
        auto selectorGroup = dynamic_cast<css3Parser::SelectorGroupContext*>(ruleset->children[0]);
        if (selectorGroup) {
            selector = selectorGroup->getText();
        }

        // Извлечение деклараций
        std::unordered_map<std::string, std::string> declarations;
        for (auto child0 : ruleset->children) {
            // Проверяем тип узла
            auto declarationList = dynamic_cast<css3Parser::DeclarationListContext*>(child0);
            if (!declarationList) continue;

            for (auto declChild : declarationList->children) {
                auto declaration = dynamic_cast<css3Parser::DeclarationContext*>(declChild);

                if (!declaration) continue;

                // Извлекаем свойство и значение
                std::string property;
                std::string value;

                if (!declaration->children.empty()) {
                    property = declaration->children[0]->getText();
                    if (declaration->children.size() > 3) {
                        value = declaration->children[3]->getText();
                    }
                }

                if (!property.empty() && !value.empty()) {
                    declarations[property] = value;
                }
            }
        }

        if (!selector.empty()) {
            styles[selector] = declarations;
        }
    }

    // Вывод стилей
    for (const auto& [selector, properties] : styles) {
        std::cout << "Selector: " << selector << std::endl;
        for (const auto& [property, value] : properties) {
            std::cout << "  " << property << ": " << value << ";" << std::endl;
        }
    }*/
    /*MyCSSListener myCssListener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&myCssListener, tree);*/

    // Выводим дерево разбора
    // std::cout << "Дерево:\n" << tree->toStringTree(&parser) << std::endl;
}

void coreInit()
{
    std::string cssCode = SGCore::FileUtils::readFile("window.css");

    parseCSS(cssCode);

    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() += onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() += onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    // std::printf("init...\n");

    // fixme: FOR TEST
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    LOG_E(SGCORE_TAG, "Русский язык");

    const SGCore::InterpolatedPath sgEditorPath = "${enginePath}/Plugins/SungearEngineEditor";

    // hardcoded sgeditor load
    auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor",
                                               sgEditorPath.resolved(),
                                               {},
                                               "cmake-build-debug");

    // std::cout << "plugin: " << sgEditorPlugin << ", sgeditor path: " << sgEditorPath << std::endl;

    const char* sungearRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    std::string sungearRootStr;
    if(sungearRoot)
    {
        sungearRootStr = sungearRoot;
    }

    SGCore::CodeGen::Generator generator;
    std::string generated = generator.generate(sungearRootStr + "/Sources/SGCore/CodeGeneration/SerdeGeneration/.templates/SerdeSpec.h");
    SGCore::FileUtils::writeToFile("generated.h", generated, false, true);

    using namespace SGCore;
    
    rapidjson::Document document;
    document.SetObject();
    
    /*TestNamespace::MyStruct<std::string> testStruct;
    
    Serializer::serialize(document, document, "testTransform", testStruct);*/
    
    /*Transform testTransform;
    Mesh mesh;
    RenderingBase renderingBase;

    Ref<AudioTrackAsset> audio = AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("b2.ogg");
    audio->m_serializationType = SerializationType::SERIALIZE_DATA;

    Serializer::serialize(document, document, "test", audio);*/

    /*SGCore::Ref<SGCore::AudioTrackAsset> audio =
            SGCore::AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("b2.ogg");
    SGCore::Ref<SGCore::AudioBuffer> audioBuf =
            SGCore::MakeRef<SGCore::AudioBuffer>();
    audioBuf->create();

    audioBuf->putData(audio->getAudioTrack().getNumChannels(),
                      audio->getAudioTrack().getBitsPerSample(),
                      audio->getAudioTrack().getDataBuffer(),
                      audio->getAudioTrack().getDataBufferSize(),
                      audio->getAudioTrack().getSampleRate());*/

    /*AudioSource audioSource;
    audioSource.attachAudioTrack(audioBuf);

    Serializer::serialize(document, document, "test", audioSource);*/

    std::shared_ptr<Base> tst = std::make_shared<Derived0>();
    // Base* tst = new Derived0();
    tst->a = -1;
    /*dynamic_cast<Derived0*>(tst.get())->b = 20.1f;
    dynamic_cast<Derived0*>(tst.get())->str1 = u"abra";
    dynamic_cast<Derived0*>(tst.get())->myVec3 = { -3, 5, 1 };
    dynamic_cast<Derived0*>(tst.get())->unMap["v0"] = 20.0f;*/
    /*dynamic_cast<Derived0*>(tst)->b = 20.1f;
    dynamic_cast<Derived0*>(tst)->str1 = u"abra";*/
    //dynamic_cast<Derived*>(tst)->b = 4;
    const int i = 3;
    FileUtils::writeToFile("serializer_test.txt", Serde::Serializer::toFormat(tst), false, true);

    // Serializer::serialize(document, document, "testSerde", annotationsProcessor);

    std::string outputLog;

    std::shared_ptr<Base> deser;
    // Serde::Serializer::fromFormat<Serde::custom_derived_types<Derived0>>(FileUtils::readFile("serializer_test.txt"), deser, Serde::FormatType::JSON, outputLog, 1);
    Serde::Serializer::fromFormat(FileUtils::readFile("serializer_test.txt"), deser, outputLog);

    using t0 = types_container<int, float>;
    using t1 = types_container<float, double>;

    using cat = reverse_types_container_t<types_container_cat_t<t0, t1>>;

    std::cout << "cat: " << typeid(cat).name() << std::endl;

    // auto deser = Serde::Serializer::deserialize<std::unique_ptr<Base>>(document, "testSerde", outputLog);

    // std::printf("deser: %i, %f\n", deser->a, 0.0f);

    /*rapidjson::Document fromDocument;
    fromDocument.Parse(FileUtils::readFile("serializer_test.txt").c_str());

    std::string outputLog;
    auto deserializedStruct = Serializer::deserialize<AnnotationsProcessor>(document, "testSerde", outputLog);

    std::printf("file path: %s\n", Utils::toUTF8<char16_t>(deserializedStruct.getAnnotations()[0].m_filePath.u16string()).c_str());*/
    // std::printf("deserialized struct: %s, %i\n", deserializedStruct.m_name.c_str(), deserializedStruct.m_bool);
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->fixedUpdate(dt, fixedDt);
    }
}

void onUpdate(const double& dt, const double& fixedDt)
{
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    SGCore::ImGuiWrap::IView::getRoot()->render();
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P) &&
        ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        if(SGCore::PluginsManager::isPluginExists("SungearEngineEditor"))
        {
            SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", {}, "cmake-build-debug");
        }
        else
        {
            char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");

            if(sgSourcesPath)
            {
                std::string sgEditorPath = std::string(sgSourcesPath) + "/Plugins/SungearEngineEditor";
                SGCore::PluginsManager::loadPlugin("SungearEngineEditor",
                                                   sgEditorPath,
                                                   {},
                                                   "cmake-build-debug");
            }
        }
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_O) &&
            ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        std::printf("unloaded plugin\n");
        SGCore::PluginsManager::unloadAllPlugins();
    }

    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
    }
    
    /*try
    {
        void* ptr = nullptr;
        *((int*) ptr) = 5;
    }
    catch(std::exception e)
    {
        std::cout << e.what() << std::endl;
    }*/
}

int main()
{
    using namespace SGCore;
    //std::printf("definitions: %i\n", register_definition<SGCore::SerializerSpec<std::int8_t>>::counter);
    
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    CoreMain::start();
    
    return 0;
}