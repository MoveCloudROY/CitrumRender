#pragma once

#include "core/events/Events.h"
#include "core/Timestep.h"
#include <string>

namespace EG {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach(){};
    virtual void OnDetach(){};
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnEvent(Event& e){};

    virtual void OnImGuiRender(){};

    const std::string& GetName() const { return debug_name_; }

protected:
    std::string debug_name_;
};

} // namespace EG