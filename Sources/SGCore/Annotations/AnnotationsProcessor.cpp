//
// Created by ilya on 28.06.24.
//
#include "AnnotationsProcessor.h"

#include <SGCore/pch.h>

std::string SGCore::AnnotationsProcessor::Annotation::validateAcceptableArgs(const Annotation& annotationToValidate) const noexcept
{
    decltype(Annotation::m_acceptableArgs) necessaryArgs = m_acceptableArgs;
    
    for(const auto& argPair : annotationToValidate.m_currentArgs)
    {
        auto foundIt = m_acceptableArgs.find(argPair.first);
        
        if(foundIt == m_acceptableArgs.end())
        {
            return fmt::format("Error in annotation '{0}'. Unknown argument: {1}", m_name, argPair.first);
        }
        
        if(foundIt->second.m_requiredValuesCount < argPair.second.m_values.size() && foundIt->second.m_requiredValuesCount != -1)
        {
            return fmt::format("Error in annotation '{0}'. Invalid number of arguments {1}. Requires {2} arguments or less", m_name, argPair.second.m_requiredValuesCount, foundIt->second.m_requiredValuesCount);
        }
        
        necessaryArgs.erase(argPair.first);
    }
    
    std::string necessaryArgsErr;
    
    for(const auto& argPair : necessaryArgs)
    {
        if(!argPair.second.m_isUnnecessary)
        {
            necessaryArgsErr += fmt::format("Error in annotation '{0}'. Argument '{1}' is necessary (missing).\n", m_name, argPair.first);
        }
    }
    
    if(!necessaryArgsErr.empty())
    {
        necessaryArgsErr += annotationToValidate.formArgumentsString();
        
        return necessaryArgsErr;
    }
    
    return "";
}

std::string SGCore::AnnotationsProcessor::Annotation::formArgumentsString() const noexcept
{
    std::string args = "Arguments: ";
    for(auto i = m_currentArgs.begin(); i != m_currentArgs.end(); ++i)
    {
        const auto& arg = i;
        
        args += arg->first + " = [";
        for(const auto& val : arg->second.m_values)
        {
            args += val;
        }
        args += "], ";
    }
    
    if(m_currentArgs.empty())
    {
        args + " no args";
    }
    
    return args;
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
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_struct";
        
        newAnnotation.m_acceptableArgs["fullName"].m_name = "fullName";
        
        newAnnotation.validate = [newAnnotation, this](Annotation& annotation,
                                                           const std::vector<std::string>& words, std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = newAnnotation.validateAcceptableArgs(annotation);
            
            std::printf("word: %s\n", words[wordIndex].c_str());
            
            std::int64_t parenthesisCnt = 0;
            bool isAnnotationBegan = false;
            for(std::int64_t i = wordIndex; i < words.size(); ++i)
            {
                const auto& word = words[i];
                
                for(const auto& a : m_annotations)
                {
                    if(word.contains(a.first))
                    {
                        isAnnotationBegan = true;
                        break;
                    }
                }
                
                if(isAnnotationBegan)
                {
                    for(const auto& c : word)
                    {
                        if(c == '(')
                        {
                            ++parenthesisCnt;
                        }
                        else if(c == ')')
                        {
                            --parenthesisCnt;
                        }
                    }
                }
                
                if(!isAnnotationBegan)
                {
                    std::printf("word11: %s\n", words[i].c_str());
                    
                    if(words[i] != "struct")
                    {
                        return "sg_struct annotation requires struct declaration after declaration of sg_struct. " + annotation.formArgumentsString();
                    }
                    else
                    {
                        break;
                    }
                }
                
                if(parenthesisCnt == 0)
                {
                    isAnnotationBegan = false;
                }
            }
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };
        
        m_annotations["sg_struct"] = newAnnotation;
    }
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_component";
        
        newAnnotation.validate = [newAnnotation](Annotation& annotation, const std::vector<std::string>& words, std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = newAnnotation.validateAcceptableArgs(annotation);
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };
        
        m_annotations["sg_component"] = newAnnotation;
    }
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
        
        fileText = prettifyCode(fileText);
        
        std::vector<std::string> lineWords;
        Utils::splitString(fileText, ' ', lineWords);
        
        auto nameDividerRegexBeginIt =
                std::sregex_iterator(fileText.begin(), fileText.end(), m_nameAndArgsDivideRegex);
        auto regexEndIt = std::sregex_iterator();
        
        bool isParenthesisOpened = false;
        
        for(auto it = nameDividerRegexBeginIt; it != regexEndIt; ++it)
        {
            const std::smatch& match = *it;
            
            const std::string& annotationName = match[1];
            const std::string& annotationArgs = match[2];
            
            fileText.insert(match.position() + match[0].length(), " ");
            
            auto annotationIt = m_annotations.find(annotationName);
            
            if(annotationIt != m_annotations.end())
            {
                Annotation annotation;
                annotation.m_name = annotationName;
                
                bool isArray = false;
                bool isString = false;
                bool enableValueWriting = false;
                bool isAppendingChar = true;
                std::string currentValue;
                std::string currentArg;
                
                for(std::uint64_t i = 0; i < annotationArgs.length(); ++i)
                {
                    isAppendingChar = true;
                    
                    const auto& c = annotationArgs[i];
                    
                    if(c == '=')
                    {
                        enableValueWriting = !enableValueWriting;
                        isAppendingChar = false;
                    }
                    
                    if(c == '"')
                    {
                        isString = !isString;
                        if(isString)
                        {
                            currentValue = "";
                        }
                        isAppendingChar = false;
                    }
                    
                    // new argument
                    if((c == ',' || i == annotationArgs.length() - 1) && !isArray)
                    {
                        currentValue = Utils::trim(currentValue);
                        currentArg = Utils::trim(currentArg);
                        
                        annotation.m_currentArgs[currentArg].m_name = currentArg;
                        annotation.m_currentArgs[currentArg].m_values.push_back(currentValue);
                        
                        std::printf("annotation: %s, arg: %s, value: %s\n", annotation.m_name.c_str(), currentArg.c_str(), currentValue.c_str());
                        
                        currentArg = "";
                        currentValue = "";
                        enableValueWriting = false;
                        continue;
                    }
                    else if((c == ',' || c == ']') && isArray)
                    {
                        currentValue = Utils::trim(currentValue);
                        currentArg = Utils::trim(currentArg);
                        
                        annotation.m_currentArgs[currentArg].m_name = currentArg;
                        annotation.m_currentArgs[currentArg].m_values.push_back(currentValue);
                        
                        std::printf("annotation: %s, arg: %s, array value: %s\n", annotation.m_name.c_str(), currentArg.c_str(), currentValue.c_str());
                        
                        currentValue = "";
                        
                        continue;
                    }
                    
                    if(c == '[')
                    {
                        isArray = true;
                        continue;
                    }
                    if(c == ']')
                    {
                        isArray = false;
                        continue;
                    }
                    
                    // writing arg
                    if(isAppendingChar)
                    {
                        if(!enableValueWriting)
                        {
                            currentArg += c;
                        }
                        else
                        {
                            currentValue += c;
                        }
                    }
                }
                
                std::int64_t wordIndex = 0;
                {
                    std::int64_t len = 0;
                    for(const auto& w : lineWords)
                    {
                        len += w.length() + 1;
                        ++wordIndex;
                        if(len >= match.position())
                        {
                            break;
                        }
                    }
                }
                
                std::string annotationValidationError = m_annotations[annotation.m_name].validate(annotation, lineWords, wordIndex);
                if(!annotationValidationError.empty())
                {
                    throw std::runtime_error(annotationValidationError.append("\n\tFile: ").append(u8FilePath));
                }
            }
        }
        
        std::cout << fileText << std::endl;
    }
}

std::string SGCore::AnnotationsProcessor::prettifyCode(const std::string& code) noexcept
{
    std::string outputStr;
    
    std::vector<std::string> lines;
    std::vector<std::string> lineWords;
    
    Utils::splitString(code, '\n', lines);
    
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
            
            auto foundMultilineCloseBracket = word.find("*/");
            if(foundMultilineCloseBracket != std::string::npos)
            {
                isMultilineComment = false;
            }
            
            if(isOneLineComment || isMultilineComment)
            {
                continue;
            }
            
            // NOW WE CAN PARSING
            
            if(foundMultilineCloseBracket != std::string::npos)
            {
                outputStr += word.substr(foundMultilineCloseBracket + 2, word.length() - (foundMultilineCloseBracket + 2));
            }
            else
            {
                outputStr += word + " ";
            }
        }
        
        lineWords.clear();
        
        // outputStr += "\n";
        
        if(isOneLineComment)
        {
            isOneLineComment = false;
            continue;
        }
        
        isOneLineComment = false;
    }
    
    outputStr = Utils::reduce(outputStr);
    
    return outputStr;
}
