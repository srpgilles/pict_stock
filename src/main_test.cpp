#include <yuni/core/string.h>
#include <map>

using namespace Yuni;

typedef Yuni::CString<4, false> SymbolString;




int main(int argc, char* argv[])
{
	std::map<SymbolString, int> foo;

	foo["Bar"] = 3;
	foo["Baz"] = 5;

	return 0;
}
