#include <yuni/core/getopt.h>

using namespace Yuni;

int main(int argc, char* argv[])
{
    GetOpt::Parser parser;
    String parameterFile;
    parser.addParagraph("\nUsual option(s):\n");
    parser.add(parameterFile, ' ', "parameter_file", "Parameters file. If not precised "
        "parameters_pict_frame.ini in current folder is attempted.");

    parser.addParagraph("\nHelp:\n");

    if (!parser(argc, argv))
        exit(EXIT_FAILURE);

/*	(void) argc;
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
        std::cout << *it << '\n';*/


	return 0;
}
