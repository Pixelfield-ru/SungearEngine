//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
#define SUNGEARENGINE_ANNOTATIONSPROCESSOR_H

#include <SGCore/pch.h>

#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    namespace fs = std::filesystem;
    
    struct AnnotationsProcessor
    {
        struct AnnotationArg
        {
            std::string m_name;
            std::any m_value;
        };
        
        struct Annotation
        {
            std::string m_name;
            
            // first - name
            std::unordered_map<std::string, AnnotationArg> m_arguments { };
            
            std::function<bool()> evaluate { };
        };
        
        struct Member
        {
            std::string m_name;
            std::string m_type { };
            
            // first - name
            std::unordered_map<std::string, Annotation> m_annotations { };
            
            [[nodiscard]] bool isAnnotationProvided(const std::string& name) const noexcept
            {
                return m_annotations.contains(name);
            }
        };
        
        struct SourceStruct
        {
            std::string m_name;
            
            std::filesystem::path m_sourceFile;
            
            // first - name
            std::unordered_map<std::string, Member> m_members;
            
            Member* getMember(const std::string& name) noexcept
            {
                auto foundIt = m_members.find({ name });
                if(foundIt != m_members.end())
                {
                    return &foundIt->second;
                }
                
                return nullptr;
            }
        };
        
        void processAnnotations(const std::filesystem::path& inDirectory, bool recursively = true)
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
        
        void processAnnotations(const std::vector<std::filesystem::path>& files)
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
                        
                        if(word.contains("sg_serializable"))
                        {
                            std::printf("Serializable detected in file %s!\n", u8FilePath.c_str());
                        }
                    }
                    
                    isOneLineComment = false;
                    lineWords.clear();
                }
                
                // std::printf("File '%s' proceed\n", u8FilePath.c_str());
            }
        }
        
    private:
        // first - name
        static inline std::unordered_map<std::string, Annotation> m_defaultAnnotations;
    };
}

#endif //SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
