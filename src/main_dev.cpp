//#include "extended_photo/extended_photo.hpp"
//#include <yuni/datetime/timestamp.h>
#include <ctime>
//# include "tools/numeration.hpp"

#include "database/cameras.hpp"
#include "tools/sqlite_wrapper.hpp"
#include "tools/tools.hpp"
#include <ostream>

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

using namespace PictStock;

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	GenericTools::SqliteWrapper db("test.db3", SQLITE_OPEN_READWRITE);

	typedef PictStock::Database::Cameras::Tuple Tuple;

	std::cout << std::tuple_size<Tuple>::value << '\n';
    typedef GenericTools::Tuple::WrappedType<Tuple>::type Strings;


	std::cout << std::tuple_size<Strings>::value << '\n';

	std::vector<Strings> pRows;
	db.select(pRows, "Keyword,Value,Owner FROM Cameras ORDER BY Keyword");

	std::vector<YString> fieldNames;
    GenericTools::Tuple::Fields<Tuple>::FieldNames(fieldNames);

	for (auto it = fieldNames.cbegin(), end = fieldNames.cend(); it != end; ++it)
		std::cout << *it << '\n';

	Database::Cameras cameras(db);

	cameras.addNewPhotographer("FOO", "BAR", "Baz");
	cameras.addNewCamera("Foo", "Bar", "Baz");

	return 0;
}
