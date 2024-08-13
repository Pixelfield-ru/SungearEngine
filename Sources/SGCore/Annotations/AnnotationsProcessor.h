//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
#define SUNGEARENGINE_ANNOTATIONSPROCESSOR_H

#include <SGCore/pch.h>

#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    template<typename>
    struct SerializerSpec;

    struct AnnotationsProcessor
    {
        sg_serializer_as_friend(AnnotationsProcessor)

        enum class AnnotationActiveState
        {
            INACTIVE,
            STAY_ACTIVE
        };
        
        struct AnnotationArg
        {
            sg_serializer_as_friend(AnnotationArg)

            std::string m_name { };
            bool m_isUnnecessary = false;
            // -1 means for variadic count of arguments
            std::int64_t m_requiredValuesCount = 1;
            std::vector<std::any> m_values;
        };
        
        struct Annotation
        {
            sg_serializer_as_friend(Annotation)

            std::string m_name { };
            
            std::filesystem::path m_filePath;
            
            // first - name
            std::unordered_map<std::string, AnnotationArg> m_acceptableArgs { };
            std::unordered_map<std::string, AnnotationArg> m_currentArgs { };
            std::vector<AnnotationArg> m_anonymousArgs;
            
            std::function<std::string(Annotation& annotation, const std::vector<std::string>& words, std::int64_t wordIndex)> validate { };
            
            std::string validateAcceptableArgs(const Annotation& annotationToValidate) const noexcept;
            
            std::function<AnnotationActiveState(Annotation& annotation, const std::string& type, const std::string& name, bool isHigher)> acceptVariable;
            
            std::string formArgumentsString() const noexcept;
        };
        
        struct Member
        {
            sg_serializer_as_friend(Member)

            std::string m_name;
            
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
        
        void processAnnotations(const std::filesystem::path& inDirectory, const std::vector<std::filesystem::path>& filesToExclude = { }, bool recursively = true);
        
        void processAnnotations(const std::vector<std::filesystem::path>& files);

        void saveToFile(const std::filesystem::path& filePath) const noexcept;
        void loadFromFile(const std::filesystem::path& filePath) const noexcept;

        std::string stringifyAnnotations() const noexcept;
        
        [[nodiscard]] const std::vector<Annotation>& getAnnotations() const noexcept;
        [[nodiscard]] std::unordered_map<std::string, Annotation>& getSupportedAnnotations() noexcept;
        [[nodiscard]] const std::unordered_map<std::string, Annotation>& getSupportedAnnotations() const noexcept;

    private:
        std::int64_t parseAnnotationArgument(Annotation& toAnnotation, const std::int64_t& charIdx,
                                             const std::string& fileText, const std::vector<std::string>& words,
                                             const std::int64_t& wordIdx) noexcept;
        
        // first - name
        std::unordered_map<std::string, Annotation> m_supportedAnnotations;
        std::vector<Annotation> m_currentAnnotations;
        // std::unordered_map<std::string, SourceStruct> m_structs;
        
        std::string prettifyCode(const std::string& code) noexcept;
        
        // dividing name of annotation and arguments of annotation. arguments are fused together in one group
        static inline std::regex m_nameAndArgsDivideRegex = std::regex(R"(([a-zA-Z_0-9]+) *\(([0-9a-zA-Z=:", \[\]]*)\))");
        static inline std::regex m_namespacesRegex = std::regex(R"((namespace|struct) ([a-zA-Z0-9_:]+) *\{ *(.*) *\})");
    };
}

#endif //SUNGEARENGINE_ANNOTATIONSPROCESSOR_H
