#pragma once

#include "Events.h"
namespace EG {


class KeyEvent : public Event {
public:
    inline int GetKeyCode() const { return keycode_; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

protected:
    KeyEvent(int keycode)
        : keycode_(keycode) {}

    int keycode_;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keycode, bool isRepeat = false)
        : KeyEvent(keycode), isRepeat_(isRepeat) {}

    bool IsRepeat() const { return isRepeat_; }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    bool isRepeat_;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(int keycode)
        : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyTyped)
};


} // namespace EG