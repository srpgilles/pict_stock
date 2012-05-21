#include "extended_photo.hpp"
#include <boost/regex.hpp>
#include "private/photo_directory_iterator.hpp"
#include "private/path_format.hpp"
#include "private/date.hpp"

#ifdef USE_BOOST_REGULAR_EXPR
namespace TestRegex
{
	void testRegex(LoggingFacility& logs)
	{

		std::list<YString> tests = {"2012:01:01 17:21:03",
				"2012: 01: 01 17 :21: 03",
				"2012:1:1 17:21:3",
				"2012: 1: 1 17:21: 3",
				"2010:13:01 01:01:01",
				"2010:12:32 01:01:01",
				"2010:12:42 01:01:01",
				"2010:12:12 25:01:01",
				"2010:12:12 23:01:01",
				"2010:12:12 23:61:01",
				"2010:12:12 23:51:71",



		};

		for (auto it = tests.cbegin(), end = tests.cend(); it != end; ++it)
		{
			::PictStock::Private::Date mydate;
			logs.notice() << *it << "|" << dateFromExif(logs, mydate, *it);
		}
	}
};
#endif // USE_BOOST_REGULAR_EXPR


namespace TestExtendedPhoto
{
	class TestPhotoDir : public ::PictStock::Private::PhotoDirectoryIterator
	{
	public:
		TestPhotoDir(LoggingFacility& logs)
			: PhotoDirectoryIterator(logs)
		{ }

		const std::vector<YString>& files() const;

	private:

		virtual Yuni::IO::Flow onFile(const Yuni::String&, const Yuni::String& folder,
			const Yuni::String& name, Yuni::uint64 size);

		std::vector<YString> pFiles;
	};


	Yuni::IO::Flow TestPhotoDir::onFile(const Yuni::String& a, const Yuni::String& , const Yuni::String& , Yuni::uint64)
	 {
		pFiles.push_back(a);
		return Yuni::IO::flowContinue;
	 }

	const std::vector<YString>& TestPhotoDir::files() const
	{
		return pFiles;
	}

	void testExtendedPhoto(LoggingFacility& logs)
	{

		TestPhotoDir photoDir(logs);
		photoDir.add("/home/sebastien/Multimedia/Photos/2012");
		if (!photoDir.start())
		{
			logs.fatal() << "Problem encountered while scanning photo directory";
			exit(EXIT_FAILURE);
		}

		photoDir.wait();

		auto& files = photoDir.files();

		for (auto it = files.cbegin(), end = files.cend(); it != end; ++it)
		{
			const YString& file = *it;
			if (file.endsWith("JPG") || file.endsWith("jpg"))
				::PictStock::ExtendedPhoto photo(logs, *it);
		}
	}
}


namespace TestPathFormat
{
	void test(LoggingFacility& logs)
	{
		try
		{
			::PictStock::Private::PathFormat foo(logs, "%y/ThePhot(ographerIs%P/M%m/J%d/Photo_%H%M_%P.jpg");

			YString bar1("2012/ThePhotographerIsMe/M03/J08");

			boost::cmatch m1,m2;
			logs.notice("1 -> ") << foo.isOk(bar1, m1);

			YString bar2("2012/ThePhotographerIsMe/AdditionalIrrelevantTextM03/J08");
			logs.notice("2 -> ") << foo.isOk(bar2, m2);

			logs.notice() << m1.size() << "|" << m2.size();

			logs.notice() << m1[0].str() << "|" << m2[0].str();
			logs.notice() << m1[1].str() << "|" << m2[1].str();
			logs.notice() << m1[2].str() << "|" << m2[2].str();
			logs.notice() << m1[3].str() << "|" << m2[3].str();

			logs.notice() << (m1 == m2);
		}
		//catch(const ::PictStock::Private::PathFormatException& e)
		catch(const std::exception& e)
		{
			logs.error() << e.what();
			exit(EXIT_FAILURE);
		}
	}




}

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	LoggingFacility logs;

	//TestRegex::testRegex(logs);
	//TestExtendedPhoto::testExtendedPhoto(logs);

	TestPathFormat::test(logs);

	return 0;
}
