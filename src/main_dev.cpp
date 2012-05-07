#include "extended_photo.hpp"
#include <boost/regex.hpp>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	LoggingFacility logs;

//	PictStock::ExtendedPhoto myPhoto(logs, "/home/sebastien/Photo_C0050.JPG");
//	YString name;
//	myPhoto.newNameWithoutExtension(name);
//	logs.notice(myPhoto.date());
//	logs.notice(myPhoto.time());
//	logs.notice(name);

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

	return 0;
}
