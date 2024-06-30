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
        enum class AnnotationActiveState
        {
            INACTIVE,
            STAY_ACTIVE
        };
        
        struct AnnotationArg
        {
            std::string m_name { };
            bool m_isUnnecessary = false;
            // -1 means for variadic count of arguments
            std::int64_t m_requiredValuesCount = 1;
            std::vector<std::string> m_values;
        };
        
        struct Annotation
        {
            std::string m_name { };
            
            // first - name
            std::unordered_map<std::string, AnnotationArg> m_acceptableArgs { };
            std::unordered_map<std::string, AnnotationArg> m_currentArgs { };
            
            std::function<std::string(Annotation& annotation, const std::vector<std::string>& words, std::int64_t wordIndex)> validate { };
            
            std::string validateAcceptableArgs(const Annotation& annotationToValidate) const noexcept;
            
            std::function<AnnotationActiveState(Annotation& annotation, const std::string& type, const std::string& name)> acceptVariable;
            
            std::string formArgumentsString() const noexcept;
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
        std::unordered_map<std::string, SourceStruct> m_structs;
        
        std::string m_activeStruct;
        
        std::string prettifyCode(const std::string& code) noexcept;
        
        // dividing name of annotation and arguments of annotation. arguments are fused together in one group
        static inline std::regex m_nameAndArgsDivideRegex = std::regex(R"(([a-zA-Z_0-9]+) *\(([0-9a-zA-Z=:", \[\]]*)\))");
        static inline std::regex m_namespacesRegex = std::regex(R"((namespace|struct) ([a-zA-Z0-9_:]+) *\{ *(.*) *\})");
    };
}

#endif //SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
