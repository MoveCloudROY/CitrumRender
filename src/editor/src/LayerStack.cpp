#include "editor/LayerStack.h"

#include <algorithm>

namespace EG {

LayerStack::~LayerStack() {
    for (auto it : m_Layers) {
        it->onDetach();
        delete it;
    }
}
void LayerStack::PushLayer(Layer* layer) {
    m_Layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        layer->onDetach();
        m_Layers.erase(it);
    }
}

}