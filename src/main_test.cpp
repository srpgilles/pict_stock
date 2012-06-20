#include <yuni/core/string.h>
#include <map>
# include <yuni/core/logs.h>
#include <yuni/datetime/timestamp.h>
#include <ctime>
#include <tuple>

typedef Yuni::Logs::Logger<> LoggingFacility;
using namespace Yuni;


namespace Element // use enum class when MSVC will support it!
{
	enum Date
	{
		year = 0,
		month,
		day,
		hour,
		minute,
		second,
		size
	};
}


template<Element::Date T>
int toCTimeInformations(int value);

template<>
inline int toCTimeInformations<Element::year>(int value)
{
	return value - 1900;
}


template<>
inline int toCTimeInformations<Element::month>(int value)
{
	assert(value > 0 && value <= 12);
	return --value;
}

template<>
inline int toCTimeInformations<Element::day>(int value)
{
	assert(value >= 1 && value <= 31);
	return value;
}

template<>
inline int toCTimeInformations<Element::hour>(int value)
{
	assert(value >= 0 && value <= 23);
	return value;
}

template<>
inline int toCTimeInformations<Element::minute>(int value)
{
	assert(value >= 0 && value <= 59);
	return value;

}

template<>
inline int toCTimeInformations<Element::second>(int value)
{
	assert(value >= 0 && value < 60);
	return value;// + static_cast<int>(timezone);
}


struct Year
{
	int a;
};


struct Day
{
	int b;
};

struct Month
{
	int c;
};


struct Day2 : public Day
{

};


typedef std::tuple<Year, Day> DateTuple;






// T is the class for which we seek the index
// I is the position in DateTuple we are investigating



template<class T, class U>
struct IsSame
{
	enum { same = 0 };
};

template<class T>
struct IsSame<T, T>
{
	enum { same = 1 };
};



template<class T, std::size_t I>
struct IndexOfHelper
{

public:

	typedef typename std::tuple_element<I, DateTuple>::type type;
	enum { found = IsSame<T, type>::same };
	enum { value = (found == 1 ? I : IndexOfHelper<T, I+1>::value) };
};


template<class T>
struct IndexOfHelper<T, std::tuple_size<DateTuple>::value>
{
	enum { value = -1 };
};


template<class T>
struct IndexOf
{
	enum { value = IndexOfHelper<T, 0>::value };
};





int main(int argc, char* argv[])
{
	LoggingFacility logs;

	YString date;
	AnyString format("%Y-%m-%d %H:%M:%S");

	time_t foo(1299997609);

	//if (Yuni::DateTime::TimestampToString(date, format, foo, false))
	//	logs.notice(date);

	struct tm * timeinfo;
	char buffer [80];
	timeinfo = localtime (&foo);

	strftime (buffer, 80, format.c_str(), timeinfo);

	// logs.notice(buffer);

//	logs.notice("Check ") << foo << " == " << mktime(timeinfo);
//
//	{
//		tm myTimeInfo;
//
//		myTimeInfo.tm_year = 70;
//		myTimeInfo.tm_mon = 0;
//		myTimeInfo.tm_mday = 1;
//		myTimeInfo.tm_hour = 0;
//		myTimeInfo.tm_min = 0;
//		myTimeInfo.tm_sec = -timezone;
//		myTimeInfo.tm_isdst = 0;
//
//
//		Yuni::DateTime::TimestampToString(date, format, mktime(&myTimeInfo), true);
//		logs.notice("Date ") << mktime(&myTimeInfo) << "\t" << date;
//	}


	{
		int year = 2012;
		int month = 4;
		int day = 26;
		int hour = 13;
		int minute = 49;
		int second = 32;

		tm myTimeInfo;

		myTimeInfo.tm_year = toCTimeInformations<Element::year>(year);
		myTimeInfo.tm_mon = toCTimeInformations<Element::month>(month);
		myTimeInfo.tm_mday = toCTimeInformations<Element::day>(day);
		myTimeInfo.tm_hour = toCTimeInformations<Element::hour>(hour);
		myTimeInfo.tm_min = toCTimeInformations<Element::minute>(minute);
		myTimeInfo.tm_sec = toCTimeInformations<Element::second>(second);
		myTimeInfo.tm_isdst = 0;

		logs.notice("Original date = ") << year << ' ' << month
			<< ' ' << day << ' ' << hour << ':' << minute << ':' << second;

		Yuni::DateTime::TimestampToString(date, format, mktime(&myTimeInfo), true);

		logs.notice("Date ") << date;
	}



	{
		typedef std::tuple_element<1, DateTuple>::type MyType;


		MyType foo;
		foo.b = 5;

		logs.checkpoint() << static_cast<int>(IndexOf<Day2>::value);


	}




	return 0;
}
