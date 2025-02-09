//
// Created by stuka on 20.01.2025.
//
#include "CSSMathNode.h"

#include "SGCore/Logger/Logger.h"

float SGCore::UI::CSSMathNode::calculate(const float* parentSelectorValue) const noexcept
{
    float result = 0.0f;

    CSSMathSign lastSign = CSSMathSign::MS_PLUS;

    for(const auto& operand : m_operands)
    {
        const float calculatedValue = operand->calculate(parentSelectorValue);

        switch(lastSign)
        {
            case CSSMathSign::MS_PLUS:
            {
                result += calculatedValue;
                break;
            }
            case CSSMathSign::MS_MINUS:
            {
                result -= calculatedValue;
                break;
            }
            case CSSMathSign::MS_MULTIPLY:
            {
                result *= calculatedValue;
                break;
            }
            case CSSMathSign::MS_DIVIDE:
            {
                result /= calculatedValue;
                break;
            }
            case CSSMathSign::MS_NO_SIGN:
            {
                LOG_E(SGCORE_TAG, "CSSMathNode: unexpected error!: math node has sign equals to MS_NO_SIGN!");

                continue;
            }
        }

        lastSign = operand->m_sign;
    }

    switch(m_unarySign)
    {
        case CSSMathSign::MS_MINUS: return -result;
        default: return result;
    }
}

void SGCore::UI::CSSMathNode::resolvePriorities(bool recursed) noexcept
{
    for(size_t i = 0; i < m_operands.size(); ++i)
    {
        const auto currentOperand = m_operands[i];

        if(currentOperand->m_sign == CSSMathSign::MS_MULTIPLY ||
           currentOperand->m_sign == CSSMathSign::MS_DIVIDE)
        {
            if(recursed)
            {
                currentOperand->resolvePriorities(recursed);
            }

            const auto newNode = MakeRef<CSSMathNode>();

            newNode->m_operands.push_back(currentOperand);

            auto lastOperandSign = CSSMathSign::MS_NO_SIGN;

            for(size_t j = i + 1; j < m_operands.size();)
            {
                Ref<CSSMathNode> nextOperand = m_operands[j];

                if(recursed)
                {
                    nextOperand->resolvePriorities(recursed);
                }

                newNode->m_operands.push_back(nextOperand);

                lastOperandSign = nextOperand->m_sign;

                m_operands.erase(m_operands.begin() + j);

                if(nextOperand->m_sign != CSSMathSign::MS_MULTIPLY &&
                   nextOperand->m_sign != CSSMathSign::MS_DIVIDE)
                {
                    nextOperand->m_sign = CSSMathSign::MS_NO_SIGN;

                    break;
                }
            }

            newNode->m_sign = lastOperandSign;

            m_operands[i] = newNode;
        }
    }
}

