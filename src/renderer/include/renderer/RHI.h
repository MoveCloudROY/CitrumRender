#pragma once

#include "VertexArray.h"
#include <memory>
#include <glm/glm.hpp>

namespace EG {

class RHI {
public:
    enum class API {
        None   = 0,
        OpenGL = 1
    };

public:
    virtual ~RHI() = default;

    virtual void Init()                                                               = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color)                                = 0;
    virtual void Clear()                                                              = 0;

    // virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;

    // virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

    // virtual void SetLineWidth(float width) = 0;

    static API                  GetAPI() { return api_; }
    static std::unique_ptr<RHI> Create();

private:
    static API api_;
};

} // namespace EG
