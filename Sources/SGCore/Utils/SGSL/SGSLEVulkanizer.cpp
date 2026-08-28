//
// Created by 8bitniksis on 17.08.2026.
//

#include "SGSLEVulkanizer.h"

#include <algorithm>
#include <cctype>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace
{
    constexpr std::size_t npos = std::string::npos;

    bool isIdentifierChar(char c) noexcept
    {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

    bool isIdentifierStart(char c) noexcept
    {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    std::string trim(std::string_view text) noexcept
    {
        const auto begin = text.find_first_not_of(" \t\r\n\f\v");
        if(begin == npos) return "";
        const auto end = text.find_last_not_of(" \t\r\n\f\v");
        return std::string(text.substr(begin, end - begin + 1));
    }

    std::size_t countNewlines(std::string_view text) noexcept
    {
        return static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
    }

    struct Token
    {
        std::string m_text;
        std::size_t m_pos { };
    };

    /// Tokenizes a single statement (comments already excluded by the caller's ranges).
    std::vector<Token> tokenize(std::string_view code, std::size_t begin, std::size_t end) noexcept
    {
        std::vector<Token> tokens;
        std::size_t i = begin;

        while(i < end)
        {
            const char c = code[i];

            if(std::isspace(static_cast<unsigned char>(c)))
            {
                ++i;
                continue;
            }

            if(c == '/' && i + 1 < end && code[i + 1] == '/')
            {
                while(i < end && code[i] != '\n') ++i;
                continue;
            }

            if(c == '/' && i + 1 < end && code[i + 1] == '*')
            {
                const auto close = code.find("*/", i + 2);
                i = close == npos || close + 2 > end ? end : close + 2;
                continue;
            }

            if(isIdentifierChar(c) || c == '.')
            {
                const std::size_t start = i;
                while(i < end && (isIdentifierChar(code[i]) || code[i] == '.')) ++i;
                tokens.push_back({ std::string(code.substr(start, i - start)), start });
                continue;
            }

            tokens.push_back({ std::string(1, c), i });
            ++i;
        }

        return tokens;
    }

    struct Declaration
    {
        std::size_t m_begin { };
        std::size_t m_end { };
        /// Index of '(' of an existing layout(...) qualifier, npos if there is none.
        std::size_t m_layoutParenOpen = npos;
        std::string m_layoutContent;
        bool m_isBlock { };
        bool m_isStorage { };
        std::string m_typeName;
        std::string m_name;
        /// Original array suffix text, e.g. "[3]"; empty if not an array.
        std::string m_arraySuffix;
        std::uint32_t m_arrayCount = 1;
        bool m_hadInitializer { };
        std::string m_initializer;
        std::vector<std::string> m_conditions;
        std::optional<std::uint32_t> m_explicitBinding;
        bool m_layoutHasSet { };
    };

    struct StageScan
    {
        std::vector<Declaration> m_declarations;
        /// Global-scope `struct Name { ... };` declarations: name → end offset of the statement.
        std::unordered_map<std::string, std::size_t> m_structEnds;
        std::size_t m_firstStatementStart = npos;
        bool m_usesFragColor { };
    };

    bool containsIdentifier(std::string_view code, std::string_view identifier) noexcept
    {
        std::size_t pos = 0;
        while((pos = code.find(identifier, pos)) != npos)
        {
            const bool leftOk = pos == 0 || !isIdentifierChar(code[pos - 1]);
            const bool rightOk = pos + identifier.size() >= code.size() || !isIdentifierChar(code[pos + identifier.size()]);
            if(leftOk && rightOk) return true;
            pos += identifier.size();
        }
        return false;
    }

    std::size_t replaceIdentifier(std::string& code, std::string_view from, std::string_view to) noexcept
    {
        std::size_t replaced = 0;
        std::size_t pos = 0;
        while((pos = code.find(from, pos)) != npos)
        {
            const bool leftOk = pos == 0 || !isIdentifierChar(code[pos - 1]);
            const bool rightOk = pos + from.size() >= code.size() || !isIdentifierChar(code[pos + from.size()]);
            if(leftOk && rightOk)
            {
                code.replace(pos, from.size(), to);
                pos += to.size();
                ++replaced;
            }
            else
            {
                pos += from.size();
            }
        }
        return replaced;
    }

    /// Rewrites references to a legacy block member: `name` → `instance.name`. A name preceded by
    /// '.' is a field of some struct and is left alone; comments are rewritten harmlessly.
    std::size_t replaceMemberReferences(std::string& code, std::string_view name, std::string_view instance) noexcept
    {
        const std::string replacement = std::string(instance) + "." + std::string(name);
        std::size_t replaced = 0;
        std::size_t pos = 0;
        while((pos = code.find(name, pos)) != npos)
        {
            std::size_t before = pos;
            while(before > 0 && (code[before - 1] == ' ' || code[before - 1] == '\t')) --before;
            const bool afterDot = before > 0 && code[before - 1] == '.';
            const bool leftOk = pos == 0 || !isIdentifierChar(code[pos - 1]);
            const bool rightOk = pos + name.size() >= code.size() || !isIdentifierChar(code[pos + name.size()]);
            if(leftOk && rightOk && !afterDot)
            {
                code.replace(pos, name.size(), replacement);
                pos += replacement.size();
                ++replaced;
            }
            else
            {
                pos += name.size();
            }
        }
        return replaced;
    }

    /// Reads a preprocessor line starting at \p i (which points at '#'), handling '\' continuations.
    /// Returns [directive, rest] and advances \p i past the line.
    std::pair<std::string, std::string> readDirective(std::string_view code, std::size_t& i) noexcept
    {
        std::string line;
        ++i;
        while(i < code.size())
        {
            const char c = code[i];
            if(c == '\\' && i + 1 < code.size() && (code[i + 1] == '\n' || code[i + 1] == '\r'))
            {
                i += code[i + 1] == '\r' && i + 2 < code.size() && code[i + 2] == '\n' ? 3 : 2;
                line += ' ';
                continue;
            }
            if(c == '\n') break;
            line += c;
            ++i;
        }

        // strip trailing comment
        if(const auto comment = line.find("//"); comment != npos) line.erase(comment);

        const std::string trimmed = trim(line);
        const auto space = trimmed.find_first_of(" \t");
        if(space == npos) return { trimmed, "" };
        return { trimmed.substr(0, space), trim(trimmed.substr(space)) };
    }

    std::string joinConditions(const std::vector<std::string>& stack) noexcept
    {
        std::string result;
        for(const auto& condition : stack)
        {
            if(!result.empty()) result += " && ";
            result += "(" + condition + ")";
        }
        return result;
    }

    /// Finds the end (exclusive) of the statement starting at \p begin: the ';' at brace depth 0,
    /// or the closing '}' of a function body when no ';' follows it.
    std::size_t findStatementEnd(std::string_view code, std::size_t begin) noexcept
    {
        int depth = 0;
        std::size_t i = begin;
        while(i < code.size())
        {
            const char c = code[i];

            if(c == '/' && i + 1 < code.size() && code[i + 1] == '/')
            {
                while(i < code.size() && code[i] != '\n') ++i;
                continue;
            }
            if(c == '/' && i + 1 < code.size() && code[i + 1] == '*')
            {
                const auto close = code.find("*/", i + 2);
                i = close == npos ? code.size() : close + 2;
                continue;
            }

            if(c == '{')
            {
                ++depth;
            }
            else if(c == '}')
            {
                --depth;
                if(depth == 0)
                {
                    std::size_t j = i + 1;
                    while(j < code.size() && std::isspace(static_cast<unsigned char>(code[j]))) ++j;
                    return j < code.size() && code[j] == ';' ? j + 1 : i + 1;
                }
            }
            else if(c == ';' && depth == 0)
            {
                return i + 1;
            }

            ++i;
        }
        return code.size();
    }

    std::optional<Declaration> parseUniformStatement(std::string_view code,
                                                     std::size_t begin,
                                                     std::size_t end,
                                                     const std::vector<std::string>& conditions,
                                                     std::vector<std::string>& warnings) noexcept
    {
        const auto tokens = tokenize(code, begin, end);
        if(tokens.empty()) return std::nullopt;

        // block-ness: '{' before ';'
        bool isBlock = false;
        for(const auto& token : tokens)
        {
            if(token.m_text == "{") { isBlock = true; break; }
            if(token.m_text == ";") break;
        }

        Declaration declaration;
        declaration.m_begin = begin;
        declaration.m_end = end;
        declaration.m_isBlock = isBlock;
        declaration.m_conditions = conditions;

        std::size_t t = 0;

        // layout(...) qualifier
        if(tokens[t].m_text == "layout" && t + 1 < tokens.size() && tokens[t + 1].m_text == "(")
        {
            declaration.m_layoutParenOpen = tokens[t + 1].m_pos;
            std::size_t depth = 0;
            std::size_t k = t + 1;
            for(; k < tokens.size(); ++k)
            {
                if(tokens[k].m_text == "(") ++depth;
                else if(tokens[k].m_text == ")") { --depth; if(depth == 0) break; }
            }
            if(k >= tokens.size()) return std::nullopt;

            declaration.m_layoutContent = std::string(code.substr(tokens[t + 1].m_pos + 1, tokens[k].m_pos - tokens[t + 1].m_pos - 1));

            // explicit binding / set inside layout
            for(std::size_t j = t + 2; j + 2 < k; ++j)
            {
                if(tokens[j].m_text == "binding" && tokens[j + 1].m_text == "=")
                {
                    try { declaration.m_explicitBinding = static_cast<std::uint32_t>(std::stoul(tokens[j + 2].m_text)); }
                    catch(...) { warnings.push_back("non-numeric explicit binding in layout: '" + declaration.m_layoutContent + "'"); }
                }
                if(tokens[j].m_text == "set" && tokens[j + 1].m_text == "=")
                {
                    declaration.m_layoutHasSet = true;
                }
            }

            t = k + 1;
        }

        // storage / precision qualifiers before the storage keyword
        bool foundStorageKeyword = false;
        for(; t < tokens.size(); ++t)
        {
            const auto& text = tokens[t].m_text;
            if(text == "uniform") { foundStorageKeyword = true; ++t; break; }
            if(text == "buffer") { foundStorageKeyword = true; declaration.m_isStorage = true; ++t; break; }
            if(text == "highp" || text == "mediump" || text == "lowp" || text == "readonly" ||
               text == "writeonly" || text == "coherent" || text == "volatile" || text == "restrict")
            {
                continue;
            }
            return std::nullopt;
        }
        if(!foundStorageKeyword || t >= tokens.size()) return std::nullopt;

        if(isBlock)
        {
            // uniform BlockName { ... } [instance];
            declaration.m_name = tokens[t].m_text;
            if(!isIdentifierStart(declaration.m_name.empty() ? ' ' : declaration.m_name[0]))
            {
                warnings.push_back("could not read block name in statement at offset " + std::to_string(begin));
                return std::nullopt;
            }
            return declaration;
        }

        // uniform [precision] type name[N] [= init];
        while(t < tokens.size() && (tokens[t].m_text == "highp" || tokens[t].m_text == "mediump" || tokens[t].m_text == "lowp")) ++t;
        if(t + 1 >= tokens.size()) return std::nullopt;

        declaration.m_typeName = tokens[t].m_text;
        declaration.m_name = tokens[t + 1].m_text;
        if(!isIdentifierStart(declaration.m_name[0]))
        {
            warnings.push_back("could not read uniform name in statement at offset " + std::to_string(begin));
            return std::nullopt;
        }

        std::size_t k = t + 2;
        if(k < tokens.size() && tokens[k].m_text == "[")
        {
            std::size_t closeIdx = k;
            while(closeIdx < tokens.size() && tokens[closeIdx].m_text != "]") ++closeIdx;
            if(closeIdx >= tokens.size()) return std::nullopt;

            declaration.m_arraySuffix = std::string(code.substr(tokens[k].m_pos, tokens[closeIdx].m_pos - tokens[k].m_pos + 1));

            bool plainNumber = false;
            if(closeIdx == k + 2)
            {
                try
                {
                    declaration.m_arrayCount = static_cast<std::uint32_t>(std::stoul(tokens[k + 1].m_text));
                    plainNumber = true;
                }
                catch(...) { declaration.m_arrayCount = 1; }
            }
            if(!plainNumber && SGCore::SGSLEVulkanizer::isOpaqueType(declaration.m_typeName))
            {
                // sizes given through macros are common (`[SG_MAX_LIGHTS]`); the real count is only
                // known after preprocessing, so the reported binding count is a placeholder
                warnings.push_back("array size of '" + declaration.m_name + "' is not a plain number ('" +
                                   declaration.m_arraySuffix + "'); binding count reported as 1, take it from reflection");
            }
            k = closeIdx + 1;
        }

        for(; k < tokens.size(); ++k)
        {
            if(tokens[k].m_text == "=")
            {
                declaration.m_hadInitializer = true;
                std::size_t initEnd = end;
                for(std::size_t j = k + 1; j < tokens.size(); ++j)
                {
                    if(tokens[j].m_text == ";") { initEnd = tokens[j].m_pos; break; }
                }
                declaration.m_initializer = trim(code.substr(tokens[k].m_pos + 1, initEnd - tokens[k].m_pos - 1));
                break;
            }
            if(tokens[k].m_text == ",")
            {
                warnings.push_back("uniform '" + declaration.m_name + "' is declared in a comma list; only the first declarator is handled");
                break;
            }
            if(tokens[k].m_text == ";") break;
        }

        return declaration;
    }

    StageScan scanStage(const std::string& code, std::vector<std::string>& warnings) noexcept
    {
        StageScan scan;
        scan.m_usesFragColor = containsIdentifier(code, "gl_FragColor");

        std::vector<std::string> conditionStack;
        int depth = 0;
        std::size_t i = 0;

        while(i < code.size())
        {
            const char c = code[i];

            if(std::isspace(static_cast<unsigned char>(c)))
            {
                ++i;
                continue;
            }

            if(c == '/' && i + 1 < code.size() && code[i + 1] == '/')
            {
                while(i < code.size() && code[i] != '\n') ++i;
                continue;
            }
            if(c == '/' && i + 1 < code.size() && code[i + 1] == '*')
            {
                const auto close = code.find("*/", i + 2);
                i = close == npos ? code.size() : close + 2;
                continue;
            }

            if(c == '#')
            {
                // directive only if nothing but whitespace precedes it on the line
                std::size_t lineStart = i;
                while(lineStart > 0 && code[lineStart - 1] != '\n') --lineStart;
                const bool atLineStart = trim(std::string_view(code).substr(lineStart, i - lineStart)).empty();

                if(atLineStart)
                {
                    const auto [directive, rest] = readDirective(code, i);

                    if(directive == "ifdef") conditionStack.push_back("defined(" + rest + ")");
                    else if(directive == "ifndef") conditionStack.push_back("!defined(" + rest + ")");
                    else if(directive == "if") conditionStack.push_back(rest);
                    else if(directive == "elif")
                    {
                        if(conditionStack.empty()) warnings.push_back("#elif without #if");
                        else conditionStack.back() = "!(" + conditionStack.back() + ") && (" + rest + ")";
                    }
                    else if(directive == "else")
                    {
                        if(conditionStack.empty()) warnings.push_back("#else without #if");
                        else conditionStack.back() = "!(" + conditionStack.back() + ")";
                    }
                    else if(directive == "endif")
                    {
                        if(conditionStack.empty()) warnings.push_back("#endif without #if");
                        else conditionStack.pop_back();
                    }
                    continue;
                }
            }

            if(c == '{') { ++depth; ++i; continue; }
            if(c == '}') { --depth; ++i; continue; }
            if(depth > 0) { ++i; continue; }

            // statement at global scope
            if(scan.m_firstStatementStart == npos) scan.m_firstStatementStart = i;

            const std::size_t end = findStatementEnd(code, i);
            if(end <= i) { ++i; continue; }

            if(auto declaration = parseUniformStatement(code, i, end, conditionStack, warnings))
            {
                scan.m_declarations.push_back(std::move(*declaration));
            }
            else
            {
                const auto tokens = tokenize(code, i, end);
                if(tokens.size() >= 3 && tokens[0].m_text == "struct" && tokens[2].m_text == "{")
                {
                    scan.m_structEnds[tokens[1].m_text] = end;
                }
            }

            i = end;
        }

        if(!conditionStack.empty())
        {
            warnings.push_back("unbalanced #if: " + std::to_string(conditionStack.size()) + " condition(s) left open");
        }

        return scan;
    }

    struct Edit
    {
        std::size_t m_pos { };
        std::size_t m_length { };
        std::string m_text;
        /// Edits at equal positions are applied in ascending order; the last applied ends up first in text.
        int m_order { };
    };

    void applyEdits(std::string& code, std::vector<Edit>& edits) noexcept
    {
        std::stable_sort(edits.begin(), edits.end(), [](const Edit& a, const Edit& b) {
            if(a.m_pos != b.m_pos) return a.m_pos > b.m_pos;
            return a.m_order < b.m_order;
        });

        for(const auto& edit : edits)
        {
            code.replace(edit.m_pos, edit.m_length, edit.m_text);
        }
    }

    std::string layoutBindingText(bool withSet, std::uint32_t set, std::uint32_t binding) noexcept
    {
        return (withSet ? "set = " + std::to_string(set) + ", " : std::string()) + "binding = " + std::to_string(binding);
    }
}

bool SGCore::SGSLEVulkanizer::isOpaqueType(std::string_view typeName) noexcept
{
    constexpr std::string_view prefixes[] = {
        "sampler", "isampler", "usampler",
        "image", "iimage", "uimage",
        "texture", "itexture", "utexture",
        "subpassInput", "atomic_uint"
    };

    for(const auto prefix : prefixes)
    {
        if(typeName.starts_with(prefix)) return true;
    }
    return false;
}

SGCore::SGSLEVulkanizer::Report SGCore::SGSLEVulkanizer::vulkanize(std::vector<Stage>& stages, const Config& config) noexcept
{
    Report report;
    const bool withSet = config.m_target == Target::VULKAN;

    std::vector<StageScan> scans;
    scans.reserve(stages.size());
    for(const auto& stage : stages)
    {
        scans.push_back(scanStage(stage.m_code, report.m_warnings));
    }

    // ---- program-wide binding table (order of first appearance, stages in given order)

    struct ResourceEntry
    {
        ResourceKind m_kind { };
        std::uint32_t m_binding { };
        std::uint32_t m_count = 1;
        bool m_assigned { };
    };
    std::unordered_map<std::string, ResourceEntry> resources;
    std::vector<std::string> resourceOrder;

    struct LegacyMember
    {
        std::string m_declaration;
        std::string m_typeName;
        std::string m_name;
        std::string m_condition;
    };
    // One legacy block per stage: a stage only sees the types its own code declares, so a
    // program-wide union would reference structs missing from stages that never included them
    // (screen.glsl includes uniform_bufs_decl.glsl in the vertex stage only). A uniform declared
    // in several stages lands in each stage's block; consumers write it by name into every block.
    struct StageLegacy
    {
        std::string m_blockName;
        std::vector<LegacyMember> m_members;
        std::unordered_map<std::string, std::size_t> m_indexByName;
    };
    std::vector<StageLegacy> stageLegacy(stages.size());

    auto legacyBlockNameFor = [&](SGSLESubShaderType type)
    {
        const char* suffix = "unknown";
        switch(type)
        {
            case SST_VERTEX: suffix = "vertex"; break;
            case SST_FRAGMENT: suffix = "fragment"; break;
            case SST_GEOMETRY: suffix = "geometry"; break;
            case SST_COMPUTE: suffix = "compute"; break;
            case SST_TESS_CONTROL: suffix = "tess_control"; break;
            case SST_TESS_EVALUATION: suffix = "tess_eval"; break;
            default: break;
        }
        return config.m_legacyBlockName + "_" + suffix;
    };

    auto registerResource = [&](const std::string& name, ResourceKind kind, std::uint32_t count,
                                std::optional<std::uint32_t> explicitBinding)
    {
        auto it = resources.find(name);
        if(it != resources.end())
        {
            if(it->second.m_kind != kind)
            {
                report.m_warnings.push_back("resource '" + name + "' is declared with different kinds across stages");
            }
            if(explicitBinding && it->second.m_assigned && it->second.m_binding != *explicitBinding)
            {
                report.m_warnings.push_back("resource '" + name + "' has conflicting explicit bindings across stages");
            }
            return;
        }

        ResourceEntry entry;
        entry.m_kind = kind;
        entry.m_count = count;
        if(explicitBinding)
        {
            entry.m_binding = *explicitBinding;
            entry.m_assigned = true;
        }
        resources.emplace(name, entry);
        resourceOrder.push_back(name);
    };

    // Shader files are usually wrapped in a variant guard (`#if defined(SG_GLSL4) || ...`), so every
    // uniform sits under at least that condition. The longest condition prefix shared by all loose
    // uniforms of the program is treated as the file region: it is stripped from member guards and
    // the block is placed at the first uniform that lives directly in that region.
    std::vector<std::string> commonConditions;
    bool commonInitialized = false;
    for(const auto& scan : scans)
    {
        for(const auto& declaration : scan.m_declarations)
        {
            if(declaration.m_isBlock || isOpaqueType(declaration.m_typeName)) continue;
            if(!commonInitialized)
            {
                commonConditions = declaration.m_conditions;
                commonInitialized = true;
                continue;
            }
            std::size_t same = 0;
            while(same < commonConditions.size() && same < declaration.m_conditions.size() &&
                  commonConditions[same] == declaration.m_conditions[same]) ++same;
            commonConditions.resize(same);
        }
    }

    auto memberCondition = [&](const std::vector<std::string>& conditions)
    {
        return joinConditions(std::vector<std::string>(conditions.begin() + static_cast<std::ptrdiff_t>(commonConditions.size()),
                                                       conditions.end()));
    };

    for(std::size_t s = 0; s < stages.size(); ++s)
    {
        for(const auto& declaration : scans[s].m_declarations)
        {
            if(declaration.m_isBlock)
            {
                registerResource(declaration.m_name,
                                 declaration.m_isStorage ? ResourceKind::STORAGE_BLOCK : ResourceKind::UNIFORM_BLOCK,
                                 1, declaration.m_explicitBinding);
            }
            else if(isOpaqueType(declaration.m_typeName))
            {
                registerResource(declaration.m_name,
                                 declaration.m_typeName.find("image") != npos ? ResourceKind::IMAGE : ResourceKind::SAMPLER,
                                 declaration.m_arrayCount, declaration.m_explicitBinding);
            }
            else
            {
                auto& legacy = stageLegacy[s];
                if(legacy.m_blockName.empty())
                {
                    legacy.m_blockName = legacyBlockNameFor(stages[s].m_type);
                    registerResource(legacy.m_blockName, ResourceKind::LEGACY_UNIFORM_BLOCK, 1, std::nullopt);
                }

                const std::string condition = memberCondition(declaration.m_conditions);

                if(!legacy.m_indexByName.contains(declaration.m_name))
                {
                    legacy.m_indexByName.emplace(declaration.m_name, legacy.m_members.size());
                    legacy.m_members.push_back({ declaration.m_typeName + " " + declaration.m_name + declaration.m_arraySuffix + ";",
                                                 declaration.m_typeName, declaration.m_name, condition });
                }
                else
                {
                    report.m_warnings.push_back("uniform '" + declaration.m_name + "' is declared twice in stage " +
                                                std::to_string(std::to_underlying(stages[s].m_type)) + "; first one kept");
                }

                if(declaration.m_hadInitializer)
                {
                    ++report.m_initializersDropped;
                    report.m_defaults.push_back({ legacy.m_blockName, declaration.m_name, declaration.m_initializer });
                }
            }
        }
    }

    // assign bindings: explicit ones are reserved, the rest are numbered in order of appearance
    {
        std::vector<bool> used;
        for(const auto& name : resourceOrder)
        {
            const auto& entry = resources[name];
            if(entry.m_assigned)
            {
                if(used.size() <= entry.m_binding) used.resize(entry.m_binding + 1, false);
                if(used[entry.m_binding])
                {
                    report.m_warnings.push_back("explicit binding " + std::to_string(entry.m_binding) + " is used by more than one resource");
                }
                used[entry.m_binding] = true;
            }
        }

        std::uint32_t next = config.m_firstBinding;
        for(const auto& name : resourceOrder)
        {
            auto& entry = resources[name];
            if(entry.m_assigned) continue;
            while(next < used.size() && used[next]) ++next;
            entry.m_binding = next;
            entry.m_assigned = true;
            if(used.size() <= next) used.resize(next + 1, false);
            used[next] = true;
            ++next;
        }

        for(const auto& name : resourceOrder)
        {
            const auto& entry = resources[name];
            report.m_bindings.push_back({ name, entry.m_kind, config.m_descriptorSet, entry.m_binding, entry.m_count });

            switch(entry.m_kind)
            {
                case ResourceKind::UNIFORM_BLOCK: ++report.m_uniformBlocksBound; break;
                case ResourceKind::STORAGE_BLOCK: ++report.m_storageBlocksBound; break;
                case ResourceKind::SAMPLER:
                case ResourceKind::IMAGE: ++report.m_opaqueUniformsBound; break;
                case ResourceKind::LEGACY_UNIFORM_BLOCK: break;
            }
        }
    }

    // ---- rewrite each stage

    for(std::size_t s = 0; s < stages.size(); ++s)
    {
        auto& stage = stages[s];
        const auto& scan = scans[s];
        const auto& legacy = stageLegacy[s];
        std::vector<Edit> edits;

        // Named instance, not an anonymous block: members of anonymous blocks live in the program's
        // global namespace and glslang refuses to link two stages whose different anonymous blocks
        // share a member name. References are rewritten to `<instance>.<member>` below.
        const std::string legacyInstanceName = legacy.m_blockName.empty() ? "" : "sg_" + legacy.m_blockName;
        std::string legacyBlockText;
        if(!legacy.m_blockName.empty())
        {
            legacyBlockText = "layout(std140, " + layoutBindingText(withSet, config.m_descriptorSet, resources[legacy.m_blockName].m_binding) +
                              ") uniform " + legacy.m_blockName + "\n{\n";
            for(const auto& member : legacy.m_members)
            {
                if(!member.m_condition.empty()) legacyBlockText += "#if " + member.m_condition + "\n";
                legacyBlockText += "    " + member.m_declaration + "\n";
                if(!member.m_condition.empty()) legacyBlockText += "#endif\n";
            }
            legacyBlockText += "} " + legacyInstanceName + ";\n";
        }
        constexpr std::string_view legacyBlockPlaceholder = "@@SG_LEGACY_BLOCK@@";

        // The block goes where the first loose uniform living directly in the common region stood
        // (placing it under an extra #if would hide it from the rest of the stage) — but not before
        // every struct type used by its members is declared: a `uniform ObjectTransform t;` whose
        // struct comes from a later include must not precede that struct.
        std::size_t typesDeclaredAt = 0;
        for(const auto& member : legacy.m_members)
        {
            if(auto it = scan.m_structEnds.find(member.m_typeName); it != scan.m_structEnds.end())
            {
                typesDeclaredAt = std::max(typesDeclaredAt, it->second);
            }
        }

        std::size_t legacyBlockAt = npos;
        std::size_t firstLooseUniformAt = npos;
        std::size_t lastLooseUniformAt = npos;
        bool placedInRegion = false;
        for(const auto& declaration : scan.m_declarations)
        {
            if(declaration.m_isBlock || isOpaqueType(declaration.m_typeName)) continue;
            if(firstLooseUniformAt == npos) firstLooseUniformAt = declaration.m_begin;
            lastLooseUniformAt = declaration.m_begin;
            if(legacyBlockAt == npos && declaration.m_begin >= typesDeclaredAt) legacyBlockAt = declaration.m_begin;
            if(declaration.m_conditions.size() == commonConditions.size() && declaration.m_begin >= typesDeclaredAt)
            {
                legacyBlockAt = declaration.m_begin;
                placedInRegion = true;
                break;
            }
        }
        if(firstLooseUniformAt != npos && legacyBlockAt == npos)
        {
            // every loose uniform precedes some struct it (or a sibling member) needs
            legacyBlockAt = lastLooseUniformAt;
            report.m_warnings.push_back("stage " + std::to_string(std::to_underlying(stage.m_type)) +
                                        ": a struct used by the legacy block is declared after all loose uniforms; block placed at the last one, expect compile errors");
        }
        else if(legacyBlockAt != npos && !placedInRegion)
        {
            report.m_warnings.push_back("stage " + std::to_string(std::to_underlying(stage.m_type)) +
                                        ": every loose uniform is under an extra #if; legacy block placed under the first one's condition");
        }

        for(const auto& declaration : scan.m_declarations)
        {
            const bool isOpaque = !declaration.m_isBlock && isOpaqueType(declaration.m_typeName);

            if(declaration.m_isBlock || isOpaque)
            {
                const auto& entry = resources[declaration.m_name];

                if(declaration.m_explicitBinding)
                {
                    if(withSet && !declaration.m_layoutHasSet && config.m_descriptorSet != 0)
                    {
                        edits.push_back({ declaration.m_layoutParenOpen + 1, 0, "set = " + std::to_string(config.m_descriptorSet) + ", ", 0 });
                    }
                }
                else if(declaration.m_layoutParenOpen != npos)
                {
                    edits.push_back({ declaration.m_layoutParenOpen + 1, 0, layoutBindingText(withSet, config.m_descriptorSet, entry.m_binding) + ", ", 0 });
                }
                else
                {
                    edits.push_back({ declaration.m_begin, 0, "layout(" + layoutBindingText(withSet, config.m_descriptorSet, entry.m_binding) + ") ", 0 });
                }
                continue;
            }

            // loose uniform: remove, keep line count; the chosen one is replaced by the block
            const std::string_view removed = std::string_view(stage.m_code).substr(declaration.m_begin, declaration.m_end - declaration.m_begin);
            std::string replacement(countNewlines(removed), '\n');
            if(declaration.m_begin == legacyBlockAt)
            {
                // placeholder: member references are rewritten before the block text (which
                // declares the very same names) is put in
                replacement = std::string(legacyBlockPlaceholder) + replacement;
            }
            edits.push_back({ declaration.m_begin, declaration.m_end - declaration.m_begin, replacement, 0 });
            ++report.m_looseUniformsMoved;
        }

        if(scan.m_usesFragColor && stage.m_type == SGSLESubShaderType::SST_FRAGMENT)
        {
            const std::size_t at = scan.m_firstStatementStart == npos ? stage.m_code.size() : scan.m_firstStatementStart;
            edits.push_back({ at, 0, "layout(location = 0) out vec4 " + config.m_fragColorName + ";\n", 1 });
        }

        applyEdits(stage.m_code, edits);

        if(!legacy.m_blockName.empty())
        {
            for(const auto& member : legacy.m_members)
            {
                replaceMemberReferences(stage.m_code, member.m_name, legacyInstanceName);
            }

            const auto placeholderPos = stage.m_code.find(legacyBlockPlaceholder);
            if(placeholderPos != npos)
            {
                stage.m_code.replace(placeholderPos, legacyBlockPlaceholder.size(), legacyBlockText);
            }
        }

        if(scan.m_usesFragColor)
        {
            report.m_fragColorReplaced += static_cast<std::uint32_t>(replaceIdentifier(stage.m_code, "gl_FragColor", config.m_fragColorName));
        }
        if(config.m_target == Target::VULKAN)
        {
            report.m_builtinsReplaced += static_cast<std::uint32_t>(replaceIdentifier(stage.m_code, "gl_VertexID", "gl_VertexIndex"));
            report.m_builtinsReplaced += static_cast<std::uint32_t>(replaceIdentifier(stage.m_code, "gl_InstanceID", "gl_InstanceIndex"));
        }
    }

    return report;
}
