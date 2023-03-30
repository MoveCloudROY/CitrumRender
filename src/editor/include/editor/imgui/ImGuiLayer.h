#include "../Layer.h"
#include <imgui.h>

namespace EG {

class ImGuiLayer: public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate() override;

    void Begin();
    void End();
};
}