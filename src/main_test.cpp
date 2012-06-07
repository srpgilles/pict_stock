#include <tuple>
#include "pict_stock.hpp"
#include <yuni/core/string.h>
#include <utility>
#include <iostream>
#include <vector>

using namespace Yuni;

typedef Yuni::CString<4, false> SymbolString;
typedef Yuni::CString<30, true> RegexString;

struct Year
{
	static const SymbolString Symbol() { return "%y"; }
	static const RegexString Regex() { return "\\d{4}"; }
};


struct Month
{
	static const SymbolString Symbol() { return "%m"; }
	static const RegexString Regex() { return " \\d{1}|0\\d{1}|10|11|12"; }
};

struct Day
{
	static const SymbolString Symbol() { return "%d"; }
	static const RegexString Regex() { return  "[ |0|1|2]\\d{1}|3[0|1]"; } // no check for adequacy month day (for instance 30 Elements::b is allowed)
};


//template<std::size_t I, class TupleT>
//typename std::enable_if<I == SizeT, void>
//  print()
//  { }
//
//template<std::size_t I, class TupleT>
//typename std::enable_if<I < SizeT, void>
//  print()
//  {
//	std::cout << "THERE\n";
//	print<I + 1, SizeT, TupleT>();
//  }


template<std::size_t I = 0, class TupleT>
typename std::enable_if<I == std::tuple_size<TupleT>::value, void>::type
  printHelper()
  { }

template<std::size_t I = 0, class TupleT>
typename std::enable_if<I < std::tuple_size<TupleT>::value, void>::type
  printHelper()
  {
    std::cout << std::tuple_element<I, TupleT>::type::Symbol() << std::endl;
    printHelper<I + 1, TupleT>();
  }

template<class TupleT>
void print()
{
	printHelper<0, TupleT>();
}

struct BaseElement
{
	typedef Yuni::SmartPtr<BaseElement> Ptr;
	typedef std::vector<Ptr> Vector;
};

template<class T>
struct MyElement : public BaseElement
{
	typedef T Type;
	unsigned int position;

	MyElement(unsigned int position);
};

template<class T>
MyElement<T>::MyElement(unsigned int position)
	: position(position)
{ }

template<class T, class U>
bool operator < (const MyElement<T>& t, const MyElement<U>& u)
{
	return t.position < u.position;
}

template<std::size_t I = 0, class TupleT>
typename std::enable_if<I == std::tuple_size<TupleT>::value, void>::type
  findInStringHelper(BaseElement::Vector& /*out*/, const YString& /*format*/)
  { }

template<std::size_t I = 0, class TupleT>
typename std::enable_if<I < std::tuple_size<TupleT>::value, void>::type
findInStringHelper(BaseElement::Vector& out, const YString& format)
  {
	typedef typename std::tuple_element<I, TupleT>::type type;

	unsigned int pos = format.find(type::Symbol());

	if (pos != String::npos)
	{
		out.push_back(new MyElement<type>(pos));
	}

    //std::cout << std::tuple_element<I, TupleT>::type::Symbol() << std::endl;
    findInStringHelper<I + 1, TupleT>(out, format);
  }

template<class TupleT>
inline void findInString(BaseElement::Vector& out, const YString& format)
{
	findInStringHelper<0, TupleT>(out, format);
}


int main(int argc, char* argv[])
{
	typedef std::tuple<Year, Month, Day> TupleType;
	TupleType myTuple;

	LoggingFacility logs;


	typedef std::tuple_element<0, TupleType>::type type;

	//print<0, std::tuple_size<TupleType>::value, TupleType>();


	YString buf("%y/jfkldjfl/%d"); // month left aside on purpose

	BaseElement::Vector foo;

	findInString<TupleType>(foo, buf);

	logs.notice() << foo.size();


	//logs.notice() << myTuple.get<O>;

	return 0;
}
