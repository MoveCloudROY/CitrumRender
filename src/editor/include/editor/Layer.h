#pragma once
#include <string>

namespace EG {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void onAttach() {};
    virtual void onDetach() {};
    virtual void onUpdate() {};

    const std::string& GetName() const { return m_DebugName; }
protected:
    std::string m_DebugName;
};

} // namespace EG