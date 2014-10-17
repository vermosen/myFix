#include <functional>
#include <vector>
#include <iostream>

namespace detail
{
	template <typename Event>
	class EventBroadcaster
	{
	public:
		typedef std::function<void(const Event&)> Connection;

		void connect(Connection&& connection)
		{
			connections.push_back(std::move(connection));
		}

		void signal(const Event& event)
		{
			for (const auto& connection : connections)
			{
				connection(event);
			}
		}
	private:
		std::vector<Connection> connections;
	};

	//template <typename T> struct traits
	//	: public traits<decltype(&T::operator())> {};

	template <typename C, typename R, typename A>
	struct traits<R(C::*)(const A&) const>
	{
		typedef A type;
	};
}

template <typename... Events>
class EventBroadcaster
	: detail::EventBroadcaster<Events>...
{
public:
	template <typename Connection>
	void connect(Connection&& connection)
	{
		typedef typename detail::traits<Connection>::type Event;
		detail::EventBroadcaster<Event>& impl = *this;
		impl.connect(std::move(connection));
	}

	template <typename Event>
	void signal(const Event& event)
	{
		detail::EventBroadcaster<Event>& impl = *this;
		impl.signal(event);
	}

	virtual void processEvents() = 0;
};

struct MouseMotion
{
	int x = 0;
	int y = 0;
};

struct KeyPress
{
	char c;
};

class Input
	: public EventBroadcaster<MouseMotion, KeyPress>
{
public:
	void processEvents()
	{
		MouseMotion mouseMotion;
		mouseMotion.x = 10;
		mouseMotion.y = 20;
		signal(mouseMotion);

		KeyPress keyPress;
		keyPress.c = 'a';
		signal(keyPress);
	}
};

int main()
{
	int x = 0;
	int y = 0;
	char c = '~';

	Input input;

	input.connect([&](const MouseMotion& e){
		x += e.x;
		y += e.y;
	});

	input.connect([&](const KeyPress& e){
		c = e.c;
	});

	input.processEvents();

	std::cout << c << " " << x << "," << y << std::endl; // Output: a 10,20

	return 0;
}