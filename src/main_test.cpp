#include <tuple>
#include "pict_stock.hpp"
#include <yuni/core/string.h>
#include <utility>
#include <iostream>
#include <array>
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


template<class T>
struct MyElement
{
	typedef T Type;
};

//template<class T>
//MyElement<T>::MyElement(unsigned int position)
//	: BaseElement(position)
//{ }


template<std::size_t I = 0, class TupleT>
typename std::enable_if<I == std::tuple_size<TupleT>::value, void>::type
  findInStringHelper(std::array<unsigned int, std::tuple_size<TupleT>::value>& /*out*/, const YString& /*format*/)
  { }

template<std::size_t I = 0, class TupleT>
typename std::enable_if<I < std::tuple_size<TupleT>::value, void>::type
findInStringHelper(std::array<unsigned int, std::tuple_size<TupleT>::value>& out, const YString& format)
  {
	typedef typename std::tuple_element<I, TupleT>::type type;

	unsigned int pos = format.find(type::Symbol());

	//if (pos != String::npos)
	out[I] = pos; // npos possibly stored

	//std::cout << std::tuple_element<I, TupleT>::type::Symbol() << std::endl;
    findInStringHelper<I + 1, TupleT>(out, format);
  }


/*!
** \brief Look in the user-defined format to find the positions of relevant informations
**
** All types in the tuple list will be tried out to check whether their symbol is present
** in the user defined format. If so, a vector will be returned with the index of the
** relevant symbols in the appropriate order.
**
** For instance, if format = "%d/%y", only year and day informations are considered
** (and month ones for instance are ignored - I know it's a pretty stupid choice, but
** I'll use this one for the sake of example)
**
** The output vector will feature the positions of year and day values in the tuple list.
**
** So for instance if list is { Year, Month, Day, Hour, Minute, Second }, the
** output vector will return (2, 0)
** meaning the first informations found is index 2 in the tuple list (namely day)
** and second one is year
*/

template<class TupleT>
void findInString(std::vector<size_t>& out, const YString& format)
{
	assert(out.empty());

	// First, find whether symbols are present and store the position in an array.
	// If not found, store npos instead of position value
	std::array<unsigned int, std::tuple_size<TupleT>::value> buf;
	findInStringHelper<0, TupleT>(buf, format);

	// Prepare the output vector
	std::map<unsigned int, size_t> helper;

	for (size_t i = 0u, size = buf.size(); i < size; ++i)
	{
		if (buf[i] == String::npos)
			continue;

		helper[buf[i]] = i;
	}

	for (auto it = helper.cbegin(), end = helper.cend(); it != end; ++it)
		out.push_back(it->second);
}


int main(int argc, char* argv[])
{
	typedef std::tuple<Year, Month, Day> TupleType;
	TupleType myTuple;

	LoggingFacility logs;


	typedef std::tuple_element<0, TupleType>::type type;


	YString buf("%d/%y/jfkldjfl"); // month left aside on purpose

	auto size = std::tuple_size<TupleType>::value;

	std::vector<size_t> foo;

	findInString<TupleType>(foo, buf);

	for (unsigned int i = 0u, size = foo.size(); i < size; ++i)
		logs.notice() << "Position " << i << '\t' << foo.at(i);

	//for (auto it = foo.begin(), end = foo.end(); it != end; ++it)
	//	logs.notice() << it->first << '\t' << it->second.Symbol();



	//logs.notice() << myTuple.get<O>;

	return 0;
}
