#pragma once

#include "../tools.h"
#include <functional>
#include <ostream>

namespace EG {

enum class EventType {
    None,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory {
    None                     = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EventCategoryMouse       = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                    \
    static EventType    GetStaticType() { return EventType::type; }               \
    virtual EventType   GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }


class Event {
public:
    virtual EventType   GetEventType() const     = 0;
    virtual const char* GetName() const          = 0;
    virtual int         GetCategoryFlags() const = 0;

    bool IsInCategory(EventCategory category) {
        return GetCategoryFlags() & category;
    }

public:
    bool handled = false;
};

class EventDispatcher {
    template <typename T>
    using EventHandler = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event)
        : m_Event(event) {
    }

    template <typename T>
    bool Dispatch(EventHandler<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType) {
            m_Event.handled = func(*reinterpret_cast<T*>(&m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};


} // namespace EG