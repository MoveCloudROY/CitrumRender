#ifndef EG_CORE_APPLICATION_EVENTS_H__
#define EG_CORE_APPLICATION_EVENTS_H__


#include "Events.h"

namespace EG {

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : width_(width), height_(height) {}

    unsigned int GetWidth() const { return width_; }
    unsigned int GetHeight() const { return height_; }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int width_, height_;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event {
public:
    AppTickEvent() = default;

    EVENT_CLASS_TYPE(AppTick);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppUpdateEvent : public Event {
public:
    AppUpdateEvent() = default;

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event {
public:
    AppRenderEvent() = default;

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


} // namespace EG

#endif