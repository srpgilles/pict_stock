#include "extended_photo.hpp"
#include <boost/regex.hpp>
#include "private/photo_directory_iterator.hpp"

#ifdef USE_BOOST_REGULAR_EXPR
namespace TestRegex
{
	void testRegex(LoggingFacility& logs)
	{
		boost::regex e(
			"\\A(\\d{2,4})" // Year with 2 or 4 digits
			":" // separator
			"(\\d{2}| \\d{1})" // Month with either 2 digits or one space and one digit
			":" // separator
			"(\\d{2}| \\d{1})" // Day with either 2 digits or one space and one digit
			" " // separator
			"(\\d{2}| \\d{1})" // Hour with either 2 digits or one space and one digit
			":" // separator
			"(\\d{2}| \\d{1})" // Minute with either 2 digits or one space and one digit
			":" // separator
			"(\\d{2}| \\d{1})" // Seconds with either 2 digits or one space and one digit
			"\\z"
		);

		std::list<YString> tests = {"2012:01:01 17:21:03", "2012: 01: 01 17 :21: 03", "2012:1:1 17:21:3", "2012: 1: 1 17:21: 3"};

		for (auto it = tests.cbegin(), end = tests.cend(); it != end; ++it)
		{
			boost::cmatch foo;
			logs.notice() << *it << '\t' << regex_match(it->c_str(), foo, e);
			logs.notice() << foo.size();
			for (auto it = foo.begin(), end = foo.end(); it != end; ++it)
				std::cout << "\t|" << *it << "|\n";
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

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	LoggingFacility logs;

	//TestRegex::testRegex(logs);
	TestExtendedPhoto::testExtendedPhoto(logs);

	return 0;
}
