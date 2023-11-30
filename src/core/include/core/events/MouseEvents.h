#pragma once

#include "Events.h"

namespace EG {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y)
        : mouseX_(x), mouseY_(y) {}

    inline float GetX() const { return mouseX_; }
    inline float GetY() const { return mouseY_; }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float mouseX_, mouseY_;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : xOffset_(xOffset), yOffset_(yOffset) {}

    inline float GetXOffset() const { return xOffset_; }
    inline float GetYOffset() const { return yOffset_; }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float xOffset_, yOffset_;
};

class MouseButtonEvent : public Event {
public:
    inline int GetMouseButton() const { return button_; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
protected:
    MouseButtonEvent(int button)
        : button_(button) {}

    int button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace EG