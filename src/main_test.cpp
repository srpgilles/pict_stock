#include <yuni/core/string.h>
#include <map>
# include <yuni/core/logs.h>

typedef Yuni::Logs::Logger<> LoggingFacility;
using namespace Yuni;


class Foo
{
public:
    Foo(LoggingFacility& logs) :
        logs(logs)
    {}

    bool getValue() const
    {
        logs.debug() << __PRETTY_FUNCTION__ << " has been called\n";
        return true;
    }

public:
    LoggingFacility& logs;
};


int main(int argc, char* argv[])
{
	LoggingFacility logs;
	Foo bar(logs);

	bar.getValue();


	return 0;
}
