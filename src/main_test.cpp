#include <yuni/core/string.h>
#include <map>
#include <unordered_map>
# include <yuni/core/logs.h>
#include <yuni/datetime/timestamp.h>
#include <ctime>
#include <tuple>
#include "tools/tools.hpp"
#include <deque>
#include <yuni/core/math/random/range.h>
#include <yuni/core/math/random/default.h>

#include "tools/associative_vector.hpp"

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


//#ifdef USE_BOOST_REGULAR_EXPR
//#include <boost/regex.hpp>
//namespace TestRegex
//{
//	void testRegex(LoggingFacility& logs)
//	{
//
//		std::list<YString> tests = {"2012:01:01 17:21:03",
//				"2012: 01: 01 17 :21: 03",
//				"2012:1:1 17:21:3",
//				"2012: 1: 1 17:21: 3",
//				"2010:13:01 01:01:01",
//				"2010:12:32 01:01:01",
//				"2010:12:42 01:01:01",
//				"2010:12:12 25:01:01",
//				"2010:12:12 23:01:01",
//				"2010:12:12 23:61:01",
//				"2010:12:12 23:51:71",
//
//
//
//		};
//
//		for (auto it = tests.cbegin(), end = tests.cend(); it != end; ++it)
//		{
//			::PictStock::Date mydate;
//			logs.notice() << *it << "|" << dateFromExif(logs, mydate, *it);
//		}
//	}
//};
//#endif // USE_BOOST_REGULAR_EXPR


//namespace TestExtendedPhoto
//{
//	class TestPhotoDir : public ::PictStock::PhotoDirectoryIterator
//	{
//	public:
//		TestPhotoDir(LoggingFacility& logs)
//			: PhotoDirectoryIterator(logs)
//		{ }
//
//		const std::vector<YString>& files() const;
//
//	private:
//
//		virtual Yuni::IO::Flow onFile(const Yuni::String&, const Yuni::String& folder,
//			const Yuni::String& name, Yuni::uint64 size);
//
//		std::vector<YString> pFiles;
//	};
//
//
//	Yuni::IO::Flow TestPhotoDir::onFile(const Yuni::String& a, const Yuni::String& , const Yuni::String& , Yuni::uint64)
//	 {
//		pFiles.push_back(a);
//		return Yuni::IO::flowContinue;
//	 }
//
//	const std::vector<YString>& TestPhotoDir::files() const
//	{
//		return pFiles;
//	}
//
//	void testExtendedPhoto(LoggingFacility& logs)
//	{
//
//		TestPhotoDir photoDir(logs);
//		photoDir.add("/home/sebastien/Multimedia/Photos/2012");
//		if (!photoDir.start())
//		{
//			logs.fatal() << "Problem encountered while scanning photo directory";
//			exit(EXIT_FAILURE);
//		}
//
//		photoDir.wait();
//
//		auto& files = photoDir.files();
//
//		for (auto it = files.cbegin(), end = files.cend(); it != end; ++it)
//		{
//			const YString& file = *it;
//			if (file.endsWith("JPG") || file.endsWith("jpg"))
//			{
//				YString buf;
//				::PictStock::ExtendedPhoto photo(logs, *it);
//			}
//		}
//	}
//}


//namespace TestPathFormat
//{
//	void test(LoggingFacility& logs)
//	{
//		try
//		{
//			::PictStock::PathFormat foo(logs, "%y/ThePhotographerIs%P/Y%y/M%m/J%d/Photo_%H%M_%P.jpg");
//
//			YString bar1("2012/ThePhotographerIsMe/Y2009/M03/J08");
//
//			boost::cmatch m1,m2;
//			logs.notice("1 -> ") << foo.doFolderMatch(bar1, m1);
//
//			YString bar2("2012/ThePhotographerIsMe/AdditionalIrrelevantTextM03/J08");
//			logs.notice("2 -> ") << foo.doFolderMatch(bar2, m2);
//
//			logs.notice() << m1.size() << "|" << m2.size();
//
//			logs.notice() << m1[0].str() << "|" << m2[0].str();
//			logs.notice() << m1[1].str() << "|" << m2[1].str();
//			logs.notice() << m1[2].str() << "|" << m2[2].str();
//			logs.notice() << m1[3].str() << "|" << m2[3].str();
//
//			logs.notice() << (m1 == m2);
//		}
//		//catch(const ::PictStock::PathFormatException& e)
//		catch(const std::exception& e)
//		{
//			logs.error() << e.what();
//			exit(EXIT_FAILURE);
//		}
//	}




//}




int main()
{




	return 0;
}
