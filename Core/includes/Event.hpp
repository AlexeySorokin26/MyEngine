#pragma once

#include "Keys.hpp"

#include <array>
#include <functional>

enum class EventType {
	WindowResize = 0,
	WindowClose,

	KeyPressed,
	KeyReleased,

	MouseBUttonPressed,
	MouseButtonReleased,
	MouseMoved,

	EventCount
};

struct BaseEvent {
	virtual ~BaseEvent() = default;
	virtual EventType get_type() const = 0;
};

struct Event {
	unsigned int width;
	unsigned int height;
};

class EventDispatcher {
public:
	// problem here was that we cannot say
	// callbacks[1] = callback because their std::function has different types
	// so we create here event with BaseEvent but also pass into it func which is template EventType
	template<typename EventType>
	void add_event_listener(std::function<void(EventType&)> callback)
	{
		auto baseCallback = [func = std::move(callback)](BaseEvent& e)
			{
				if (e.get_type() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
		callbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
	}

	void dispatch(BaseEvent& e) {
		auto& callback = callbacks[static_cast<size_t>(e.get_type())];
		if (callback) {
			callback(e);
		}
	}
private:
	std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventCount)> callbacks;
};

struct EventMouseMoved : public BaseEvent {
	EventMouseMoved(const double x, const double y) :
		x(x), y(y)
	{
	}
	virtual EventType get_type() const override {
		return type;
	}
	double x;
	double y;

	static const EventType type = EventType::MouseMoved;
};

struct EventWindowResize : public BaseEvent {
	EventWindowResize(const unsigned int w, const unsigned int h) :
		w(w), h(h)
	{
	}
	virtual EventType get_type() const override {
		return type;
	}
	unsigned int w;
	unsigned int h;

	static const EventType type = EventType::WindowResize;
};

struct EventWindowClose : public BaseEvent {
	EventWindowClose()
	{
	}
	virtual EventType get_type() const override {
		return type;
	}
	static const EventType type = EventType::WindowClose;
};

struct EventKeyPressed : public BaseEvent {
	EventKeyPressed(const KeyCode keyCode, const bool repeated) :
		keyCode(keyCode), repeated(repeated)
	{
	}
	virtual EventType get_type() const override {
		return type;
	}
	KeyCode keyCode;
	bool repeated;
	static const EventType type = EventType::KeyPressed;
};

struct EventKeyReleased : public BaseEvent {
	EventKeyReleased(const KeyCode keyCode) :
		keyCode(keyCode)
	{
	}
	virtual EventType get_type() const override {
		return type;
	}
	KeyCode keyCode;
	static const EventType type = EventType::KeyReleased;
};