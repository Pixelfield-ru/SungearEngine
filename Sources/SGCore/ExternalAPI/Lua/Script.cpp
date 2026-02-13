//
// Created by stuka on 12.02.2026.
//

#include "Script.h"

#include "PackagesRepository.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::Lua::Script::execute() noexcept
{
    if(!m_loadResult.valid()) return;

    const auto executionResult = m_loadResult();
    if(!executionResult.valid())
    {
        const sol::error err = executionResult;
        LOG_E(SGCORE_TAG,
              "Lua script execution error:\n{}\nIn file: '{}'",
              err.what(),
              Utils::toUTF8(getPath().resolved().u16string()));
    }
}

SGCore::AssetRef<SGCore::TextFileAsset> SGCore::Lua::Script::getFile() noexcept
{
    return m_scriptFile.lock();
}

void SGCore::Lua::Script::doLoad(const InterpolatedPath& path)
{
    m_scriptFile = getParentAssetManager()->loadAsset<TextFileAsset>(path);

    initializeLuaState();
}

void SGCore::Lua::Script::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    // nothing to do...
}

void SGCore::Lua::Script::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    auto lockedFile = m_scriptFile.lock();
    if(!lockedFile)
    {
        doLoad(getPath());
        return;
    }

    lockedFile->reloadFromDisk(loadPolicy, lazyLoadInThread);

    initializeLuaState();
}

void SGCore::Lua::Script::onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
{
    AssetManager::resolveWeakAssetReference(updatedAssetManager, m_scriptFile);
}

void SGCore::Lua::Script::initializeLuaState() noexcept
{
    auto lockedFile = m_scriptFile.lock();

    if(!lockedFile) return;

    m_luaState = {};
    m_luaState.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string, sol::lib::math, sol::lib::package);

    // substituting 'require' function by my function...
    // used to load scripts even if they are in binary assets bundle
    sol::function originalRequire = m_luaState["require"];

    m_luaState.set_function("__sg_require", [this, originalRequire](const std::string& moduleName) {
        // if it is native package
        if(auto package = PackagesRepository::getPackage(moduleName))
        {
            auto moduleResult = m_luaState.script(package->getFile()->getData());
            PackagesRepository::getPackage(moduleName)->loadInState(m_luaState, moduleResult);

            return moduleResult;
        }

        const std::string fileName = moduleName + ".lua";

        auto assetManager = getParentAssetManager();

        const auto textFiles = assetManager->getAssetsWithType<TextFileAsset>();

        // trying to find existing file...
        for(const auto& file : textFiles)
        {
            if(file->getPath().resolved().filename() == fileName)
            {
                return m_luaState.script(file->getData());
            }
        }

        // ===================================================
        // trying to load non-existing file...
        const auto packagePath = m_luaState.get<sol::table>("package").get<std::string>("path");

        std::vector<std::string> paths;
        std::stringstream ss(packagePath);
        std::string tmpPath;

        std::filesystem::path foundFile;

        while(std::getline(ss, tmpPath, ';'))
        {
            paths.push_back(tmpPath);
        }

        for(const auto& pattern : paths)
        {
            auto specificPath = pattern;
            const auto placementPos = specificPath.find('?');

            if(placementPos != std::string::npos)
            {
                specificPath.replace(placementPos, 1, moduleName);
            }
            else
            {
                // adding file name if placement was not found
                specificPath = pattern + "/" + fileName;
            }

            const auto fsPath = std::filesystem::path(specificPath).lexically_normal();

            // if file exists then we found file!
            if(std::filesystem::exists(fsPath))
            {
                foundFile = fsPath;
                break;
            }
        }

        // trying to load included module script as text file
        if(auto includedModuleScript = assetManager->loadAsset<TextFileAsset>(foundFile))
        {
            return m_luaState.script(includedModuleScript->getData());
        }
        // ===================================================

        // something very special!
        // in theory, this case can only happen if the file simply does not exist or the paths in package.path are specified incorrectly
        LOG_W(SGCORE_TAG,
            "Lua warning: '__sg_require' was not found module '{}'. Fallback to bult-in 'require'. Module will not be found in binary assets bundle in the future.\nIn script: '{}'",
            moduleName,
            Utils::toUTF8(getPath().resolved().u16string()))

        return originalRequire(moduleName);
    });

    m_luaState["require"] = m_luaState["__sg_require"];

    // loading packages...
    // loading only after '__sg_require' function substitution
    // todo: maybe optimize and use only necessary packages?
    // PackagesRepository::loadAllPackagesInLua(m_luaState);

    // loading script...
    m_loadResult = m_luaState.load(lockedFile->getData());

    if(!m_loadResult.valid())
    {
        const sol::error err = m_loadResult;

        LOG_E(SGCORE_TAG,
              "Lua script compilation error:\n{}\nIn file: '{}'",
              err.what(),
              Utils::toUTF8(getPath().resolved().u16string()));
    }
}
