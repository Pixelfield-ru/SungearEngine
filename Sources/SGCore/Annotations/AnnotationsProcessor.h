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
            std::string m_name { };
            // -1 means for variadic count of arguments
            std::int64_t m_valuesCnt = 1;
            std::vector<std::string> m_values;
        };
        
        struct Annotation
        {
            std::string m_name { };
            
            // first - name
            std::unordered_map<std::string, AnnotationArg> m_acceptableArgs { };
            std::unordered_map<std::string, AnnotationArg> m_currentArgs { };
            
            std::function<std::string(Annotation& annotation)> validate { };
            
            std::string validateAcceptableArgs() const noexcept;
        };
        
        struct Member
        {
            std::string m_name;
            std::string m_type { };
            
            // first - name
            std::unordered_map<std::string, Annotation> m_annotations { };
            
            [[nodiscard]] bool isAnnotationProvided(const std::string& name) const noexcept;
        };
        
        struct SourceStruct
        {
            std::string m_name;
            
            std::filesystem::path m_sourceFile;
            
            // first - name
            std::unordered_map<std::string, Member> m_members;
            
            Member* getMember(const std::string& name) noexcept;
        };
        
        AnnotationsProcessor() noexcept;
        
        void processAnnotations(const std::filesystem::path& inDirectory, bool recursively = true);
        
        void processAnnotations(const std::vector<std::filesystem::path>& files);
        
    private:
        // first - name
        std::unordered_map<std::string, Annotation> m_annotations;
        
        static inline std::regex m_annotationRegex = std::regex(R"(([a-zA-Z_0-9]+)[( +]([a-zA-Z_0-9",= \\[\\]]+)[)])");
    };
}

#endif //SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
