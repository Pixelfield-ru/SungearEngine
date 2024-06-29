//
// Created by ilya on 28.06.24.
//
#include "AnnotationsProcessor.h"

#include <SGCore/pch.h>

std::string SGCore::AnnotationsProcessor::Annotation::validateAcceptableArgs() const noexcept
{
    for(const auto& argPair : m_currentArgs)
    {
        auto foundIt = m_acceptableArgs.find(argPair.first);
        
        if(foundIt == m_acceptableArgs.end())
        {
            return fmt::format("Error in annotation '{0}'. Unknown argument: {1}", m_name, argPair.first);
        }
        
        if(foundIt->second.m_valuesCnt != argPair.second.m_valuesCnt && foundIt->second.m_valuesCnt != -1)
        {
            return fmt::format("Error in annotation '{0}'. Invalid number of arguments {1}. Requires {2} arguments",  m_name, argPair.second.m_valuesCnt, foundIt->second.m_valuesCnt);
        }
    }
    
    return "";
}

bool SGCore::AnnotationsProcessor::Member::isAnnotationProvided(const std::string& name) const noexcept
{
    return m_annotations.contains(name);
}

SGCore::AnnotationsProcessor::Member*
SGCore::AnnotationsProcessor::SourceStruct::getMember(const std::string& name) noexcept
{
    auto foundIt = m_members.find({ name });
    if(foundIt != m_members.end())
    {
        return &foundIt->second;
    }
    
    return nullptr;
}

SGCore::AnnotationsProcessor::AnnotationsProcessor() noexcept
{
    Annotation sgStructAnnotation;
    sgStructAnnotation.m_name = "sg_struct";
    
    sgStructAnnotation.m_acceptableArgs["fullName"].m_name = "fullName";
    
    sgStructAnnotation.validate = [](Annotation& annotation) -> std::string {
        std::string argsAcceptableErr = annotation.validateAcceptableArgs();
        
        if(!argsAcceptableErr.empty())
        {
            return argsAcceptableErr;
        }
        
        return "";
    };
}

void SGCore::AnnotationsProcessor::processAnnotations(const std::filesystem::path& inDirectory, bool recursively)
{
    std::vector<fs::path> files;
    
    if(recursively)
    {
        for(fs::recursive_directory_iterator i(inDirectory), end; i != end; ++i)
        {
            if(!is_directory(i->path()))
            {
                files.emplace_back(i->path());
            }
        }
    }
    
    processAnnotations(files);
}

void SGCore::AnnotationsProcessor::processAnnotations(const std::vector<std::filesystem::path>& files)
{
    for(const auto& f : files)
    {
        std::string u8FilePath = Utils::toUTF8<char16_t>(f.u16string());
        if(!fs::exists(f))
        {
            std::printf("File '%s' not exists\n", u8FilePath.c_str());
            continue;
        }
        
        if(f.extension() != ".h" && f.extension() != ".hpp" && f.extension() != ".cpp" && f.extension() != ".cxx")
        {
            // std::printf("Not supported extension of file\n", u8FilePath.c_str());
            continue;
        }
        
        std::string fileText = FileUtils::readFile(u8FilePath);
        
        std::vector<std::string> lines;
        std::vector<std::string> lineWords;
        
        Utils::splitString(fileText, '\n', lines);
        
        bool isOneLineComment = false;
        bool isMultilineComment = false;
        
        for(std::uint64_t lI = 0; lI < lines.size(); ++lI)
        {
            const std::string& line = lines[lI];
            
            Utils::splitString(line, ' ', lineWords);
            
            for(std::uint64_t wI = 0; wI < lineWords.size(); ++wI)
            {
                const std::string& word = lineWords[wI];
                
                if(word.contains("//"))
                {
                    isOneLineComment = true;
                }
                
                if(word.contains("/*"))
                {
                    isMultilineComment = true;
                }
                
                if(word.contains("*/"))
                {
                    isMultilineComment = false;
                }
                
                if(isOneLineComment || isMultilineComment)
                {
                    continue;
                }
                
                // NOW WE CAN PARSING
                
                // m_annotationRegex.
                /*if(word.contains("sg_serializable"))
                {
                    std::printf("Serializable detected in file %s!\n", u8FilePath.c_str());
                }*/
            }
            
            isOneLineComment = false;
            lineWords.clear();
        }
        
        // std::printf("File '%s' proceed\n", u8FilePath.c_str());
    }
}
