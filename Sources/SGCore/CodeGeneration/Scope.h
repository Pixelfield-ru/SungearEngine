#ifndef SCOPE_H
#define SCOPE_H

namespace SGCore::CodeGen::Lang {
    struct Scope {
        [[nodiscard]] std::shared_ptr<Lang::Any> getMember(std::string name) noexcept;
        template<typename T> [[nodiscard]] std::shared_ptr<T> getMember(std::string name) noexcept;
        void setMember(std::string name, std::shared_ptr<Lang::Any> value) noexcept;
        bool deleteMember(std::string name) noexcept;
        [[nodiscard]] std::unordered_map<std::string, Lang::Any>& getAllMembers();

        [[nodiscard]] std::shared_ptr<Lang::Table> getTable(std::string name) noexcept;

        std::shared_ptr<Lang::Any> operator[](const std::string& memberName) noexcept;

        Scope createSubScope(); // returns new scope
        Scope destroySubScope(); // returns parent scope
    private:
        std::unordered_map<std::string, Lang::Any> m_variables;
        std::shared_ptr<Scope> parentScope = nullptr;
    };

    struct FunctionArgument
    {
        std::string m_name;
        bool m_isNecessary = true;
        Any m_data; // default value?
    };

    struct Any {
        virtual std::string getTypeName() const noexcept { return "Any"; }

        bool typeEquals(Any& other) { return getTypeName() == other.getTypeName(); }



        bool referenceEquals(Any& other) { return &other == this; }
        virtual bool equals(Any& other) { return referenceEquals(other); }

        bool isLessOrEqual(Any& other);
        bool isGreaterOrEqual(Any& other);
        bool isLess(Any& other);
        bool isGreater(Any& other);
    };

    template<typename Other>
    struct Comparable {
        // this <= other
        [[nodiscard]] virtual bool lessOrEqual(Other& other) noexcept;
        // this >= other
        [[nodiscard]] virtual bool greaterOrEqual(Other& other) noexcept;
        // this < other
        [[nodiscard]] virtual bool less(Other& other) noexcept;
        // this > other
        [[nodiscard]] virtual bool greater(Other& other) noexcept;
    };

    //template<typename Other>
    //struct IComparable {
    //    // other <= this
    //    [[nodiscard]] virtual bool iLessOrEqual(Other& other) noexcept { return false; }
    //    // other >= this
    //    [[nodiscard]] virtual bool iGreaterOrEqual(Other& other) noexcept { return false; }
    //    // other < this
    //    [[nodiscard]] virtual bool iLess(Other& other) noexcept { return false; }
    //    // other > this
    //    [[nodiscard]] virtual bool iGreater(Other& other) noexcept { return false; }
    //};

    template<typename Type>
    struct ObjectOfType : Any {
        Type m_value;

        bool equals(Any& other) {
            auto casted = dynamic_cast<ObjectOfType<Type>*>(&other);
            return casted != nullptr && casted->m_value == m_value;
        }
    };

    struct Number : ObjectOfType<double>, Comparable<Number> {
        std::string getTypeName() const noexcept { return "Number"; }

        [[nodiscard]] bool lessOrEqual(Number& other) noexcept;
        [[nodiscard]] bool greaterOrEqual(Number& other) noexcept;
        [[nodiscard]] bool less(Number& other) noexcept;
        [[nodiscard]] bool greater(Number& other) noexcept;
    };
    struct String : ObjectOfType<std::string> {
        std::string getTypeName() const noexcept { return "String"; }
    };

    struct Function : Any
    {
        std::string getTypeName() const noexcept { return "Function"; }

        std::vector<FunctionArgument> m_arguments;
        std::function<std::any(Any* operableVariable, // this
            const size_t& curLine,
            std::string& outputText,
            const std::vector<FunctionArgument>& args)> m_functor;
    };

    struct Table : Any
    {
        bool equals(Any& other) {
            Table* otherTable = dynamic_cast<Table*>(&other);
            if (otherTable == nullptr) { return false; }

            for (auto& member : m_members) {
                if (!otherTable->hasMember(member.first) ||
                    !otherTable->getMember(member.first)->equals(*member.second)) {
                    return false;
                }
            }

            return true;
        }

        std::string getTypeName() const noexcept { return "Table"; }

        Table() = default;
        Table(const Table&) = default;
        Table(Table&&) noexcept = default;

        [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Any>>& getMembers() const noexcept;
        [[nodiscard]] std::shared_ptr<Table> getMember(const std::string& name) noexcept;
        [[nodiscard]] bool hasMember(const std::string& memberName) const noexcept;
        void setMember(const std::string& memberName, std::shared_ptr<Any> value) noexcept;
        void removeMember(const std::string& memberName) noexcept;
        [[nodiscard]] size_t membersCount() const noexcept;

        Table& operator=(const Table&) noexcept = default;
        Table& operator=(Table&&) noexcept = default;

        struct Proxy {
            operator std::shared_ptr<Any>() {
                if (m_takedObject == nullptr)
                    m_takedObject = (*m_parentTable)[m_name];
                return m_takedObject;
            }
            Proxy& operator=(std::shared_ptr<Any> otherAny) {
                (*m_parentTable)[m_name] = otherAny;
            }

            Proxy operator[](const std::string& memberName) {
                auto newTable = std::make_shared<Table>(Table());
                (*m_parentTable)[m_name] = newTable;
                return (*newTable)[memberName];
            }

            template<typename T>
            std::shared_ptr<T> scast() {
                return std::static_pointer_cast<T>((std::shared_ptr<Any>)this);
            }

        private:
            Proxy(std::shared_ptr<Table> parentTable, std::string name) :
                m_parentTable(parentTable), m_name(name) {}

            std::shared_ptr<Any> m_takedObject;

            std::shared_ptr<Table> m_parentTable;
            std::string m_name;
        };

        Proxy operator[](const std::string& memberName) noexcept;

    private:
        std::unordered_map<std::string, std::shared_ptr<Any>> m_members;
    };

    struct ASTToken
    {
        Tokens m_type = Tokens::K_EOF;
        // optional
        std::string m_name;
        // optional
        std::string m_cppCode;
        bool m_isExprToken = false;
        std::weak_ptr<Lang::ASTToken> m_parent;
        std::vector<std::shared_ptr<Lang::ASTToken>> m_children;

        [[nodiscard]] std::shared_ptr<Table> getScopeVariable(const std::string& variableName) const noexcept;
    };
}

#endif // !SCOPE_H
