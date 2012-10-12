#include "extended_photo/extended_photo.hpp"
#include <yuni/datetime/timestamp.h>
#include <ctime>
# include "tools/numeration.hpp"

#include "tools/sqlite_wrapper.hpp"

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

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	LoggingFacility logs;

	GenericTools::SqliteWrapper db("test.db3", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

	db.createTable("Photographers",
		//"id INTEGER PRIMARY KEY NOT NULL,"
		"FirstName varchar(80),"
		"LastName varchar(80),"
		"Abbr varchar(8) PRIMARY KEY NOT NULL"
		);

	db.createTable("Cameras",
		"Keyword varchar(80),"
		"Value varchar(80),"
		"Owner varchar(8),"
		"FOREIGN KEY(Owner) REFERENCES Photographers(Abbr)"
			);

	db.insertData("Photographers", "FirstName,LastName,Abbr", "\"First\", \"Last\", \"Abbr\"");
	db.insertData("Cameras", "Keyword,Value,Owner", "\"Foo\",\"Bar\",\"Abbr\"");



	  //trackartist INTEGER,
	  //FOREIGN KEY(trackartist) REFERENCES artist(artistid)
//	command = "CREATE TABLE Photographers3("
//		"FirstName varchar(80),"
//		"LastName varchar(80),"
//		"Abbr varchar(8));";
//
//	db.execDataManipulationLanguage(command);


	//sqlite3* db = nullptr;

	// Open the database connection, creating the db3 file if necessary
	/*int errCode = sqlite3_open("test.db3", &db);
	logs.notice(errCode == 0);

	// Prepare the statement to create the table
	sqlite3_stmt* statement;
	YString cmd("CREATE TABLE Photographers2("
		"FirstName varchar(80),"
		"LastName varchar(80),"
		"Abbr varchar(8));");

	errCode = sqlite3_prepare_v2(db, cmd.c_str(), cmd.size(), &statement, NULL);
	logs.notice(errCode == 0);

	if (errCode == 0)
	{
		// Create effectively the table if not existing
		errCode = sqlite3_step(statement);
		logs.notice(errCode == 101);
	}

	// Finalize the statement
	errCode = sqlite3_finalize(statement);
	logs.notice(errCode == 0);

	// Close the database connection
	errCode = sqlite3_close(db);
	logs.notice(errCode == 0);*/


	return 0;
}
