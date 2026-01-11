#include "DValueNode.h"

template <typename Out>
SGCore::UI::DValue::DValueNodeOutput<Out>::~DValueNodeOutput() {
    for (auto node : m_nextNodes) {
        node->disconnectPrevious(this);
    }

    m_nextNodes.clear();
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::connectNext(DValueNodeInput<Out>* next) {
    if (next->m_previousNode == this) {return;}

    if (next->m_previousNode != nullptr) {
        next->disconnectPrevious(this);
    }

    m_nextNodes.insert(next);
    next->m_previousNode = this;
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::disconnectNext(DValueNodeInput<Out>* next) {
    if (m_nextNodes.erase(next)) {
        next->m_previousNode = nullptr;
    }
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::setValue(Out value) {
    for (auto node : m_nextNodes) {
        node->onReceived(value);
    }
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::connectPrevious(DValueNodeOutput<In>* previous) {
    previous->connectNext(this);
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::disconnectPrevious() {
    m_previousNode->disconnectPrevious(this);
}
