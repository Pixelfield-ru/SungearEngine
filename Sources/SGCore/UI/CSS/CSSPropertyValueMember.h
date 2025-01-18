//
// Created by stuka on 19.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYVALUEMEMBER_H
#define SUNGEARENGINE_CSSPROPERTYVALUEMEMBER_H

#include <utility>

namespace SGCore::UI
{
    template<typename MemberT>
    struct CSSPropertyValueMember
    {
        template<typename MemberT0>
        void set(MemberT0&& value) noexcept
        {
            m_member = std::forward<MemberT0>(value);

            m_isUnspecified = false;
        }
        
        [[nodiscard]] const MemberT& get() const noexcept
        {
            return m_member;
        }

        [[nodiscard]] bool isUnspecified() const noexcept
        {
            return m_isUnspecified;
        }

    private:
        MemberT m_member { };
        bool m_isUnspecified = true;
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUEMEMBER_H
