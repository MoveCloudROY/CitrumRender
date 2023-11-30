#include "core/LayerStack.h"

#include <algorithm>

namespace EG {

LayerStack::~LayerStack() {
    for (auto it : layers_) {
        it->OnDetach();
        delete it;
    }
}
void LayerStack::PushLayer(Layer* layer) {
    layers_.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = find(layers_.begin(), layers_.end(), layer);
    if (it != layers_.end()) {
        layer->OnDetach();
        layers_.erase(it);
    }
}

void LayerStack::PushOverlay(Layer* overlay) {
    layers_.emplace_back(overlay);
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find(layers_.begin() + layerInsertIndex_, layers_.end(), overlay);
    if (it != layers_.end()) {
        overlay->OnDetach();
        layers_.erase(it);
    }
}
} // namespace EG