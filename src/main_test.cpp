#include <yuni/core/string.h>
#include <map>
# include <yuni/core/logs.h>
#include <yuni/datetime/timestamp.h>
#include <ctime>
#include <tuple>
#include "tools/tools.hpp"
#include <deque>
#include <yuni/core/math/random/range.h>
#include <yuni/core/math/random/default.h>
typedef Yuni::Logs::Logger<> LoggingFacility;

using namespace Yuni;

class Foo
{
public:
	/*!
	 *
	 * @param min Minimal value
	 * @param size Size of the container (value immediately above max)
	 * @param nbRequested Number of values requested
	 */
	Foo(unsigned int min, unsigned int size, unsigned int nbRequested);


	unsigned int nbElementsInSequence() const;

	unsigned int distance() const;

	std::vector<unsigned int> sequence() const;

private:

	unsigned int pMin;
	unsigned int pSize;
	unsigned int pNbRequested;

};


Foo::Foo(unsigned int min, unsigned int size, unsigned int nbRequested)
	: pMin(min),
	  pSize(size),
	  pNbRequested(nbRequested)
{
	assert(nbRequested > 0u);
	LoggingFacility logs;
	auto bar = sequence();
	YString buf("Sequence = ");
	for (auto it = bar.cbegin(), end = bar.cend(); it != end; ++it)
		buf << ' ' << *it;

	logs.notice(buf);
}


unsigned int Foo::nbElementsInSequence() const
{
	if (pMin >= pSize)
		return 0;

	return 1u + (pSize - 1u - pMin) / distance();
}

unsigned int Foo::distance() const
{
	return pSize / pNbRequested;
}

std::vector<unsigned int> Foo::sequence() const
{
	LoggingFacility logs;
	auto distance = this->distance();
	std::vector<unsigned int> ret;
	ret.reserve(nbElementsInSequence());
	for (unsigned int i = pMin; i < pSize; i += distance)
		ret.push_back(i);

	assert(ret.size() == nbElementsInSequence());

	return ret;
}


/*
 *  Algorithm to implement:
 *
 *  . Calculate the distance
 *  . Generate a random number in [0, distance[ to determine the starting point of the sequence
 *  . If the sequence is longer than the requested number of elements, eliminate randomly
 *  as many elements as necessary
 *  	- Really randomly? Efficiency cost will be quite high; better to remove last entries...
 *  	Not such a problem with shuffle though; we get best of both worlds
 *  . And if user want chronological order, it is not a big deal to sort again the vector with
 *  a proper predicate
 *
 *
 */




int main()
{
	LoggingFacility logs;

	std::deque<unsigned int> input;
	for (unsigned int i = 0u; i < 15u; ++i)
		input.push_back(i);

	std::vector<unsigned int> choice;
	unsigned int nbElements = 4u;

	//select(input, choice, nbElements);
	for (unsigned int i = 0u; i < 100u; ++i)
	{
		for (unsigned int k = 0u; k < 100u; ++k)
		{
			for (unsigned int j = 1u; j < i; ++j)
				Foo foo(k, i, j);
		}
	}

	return 0;
}
