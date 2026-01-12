#include "DValueNode.h"

template <typename Out>
SGCore::UI::DValue::DValueNodeOutput<Out>::~DValueNodeOutput() noexcept {
    for (auto node : m_nextNodes) {
        node->disconnectPrevious(this);
    }

    m_nextNodes.clear();
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::connectNext(DValueNodeInput<Out>* next)noexcept {
    if (next->m_previousNode == this) {return;}

    if (next->m_previousNode != nullptr) {
        next->disconnectPrevious(this);
    }

    m_nextNodes.insert(next);
    next->m_previousNode = this;
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::disconnectNext(DValueNodeInput<Out>* next) noexcept {
    if (m_nextNodes.erase(next)) {
        next->m_previousNode = nullptr;
    }
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::setValue(Out value) noexcept {
    for (auto node : m_nextNodes) {
        node->onReceived(value);
    }
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::connectPrevious(DValueNodeOutput<In>* previous) noexcept {
    previous->connectNext(this);
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::disconnectPrevious() noexcept {
    m_previousNode->disconnectPrevious(this);
}
