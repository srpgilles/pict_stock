#include <yuni/core/string.h>

using namespace Yuni;

class Foo
{
public:
	typedef Yuni::SmartPtr<Foo> Ptr;

	Foo(int a)
		: bar(a)
	{}

	friend bool operator==(const Foo& a, const Foo& b);

private:
	int bar;
};

bool operator==(const Foo& a, const Foo& b)
{
	return a.bar == b.bar;

}

int main(int argc, char* argv[])
{

	Foo::Ptr foo1 = new Foo(42);
	Foo::Ptr foo2 = new Foo(42);
	Foo::Ptr foo3(foo1);

	std::cout << (foo1 == foo2) << '\n';
	std::cout << (foo1 == foo3) << '\n';
	std::cout << ((*foo1) == (*foo2)) << '\n';
	Foo bar1(42);
	Foo bar2(42);
	std::cout << (bar1 == bar2) << '\n';
	return 0;
}
