#include "tools/sqlite_wrapper.hpp"

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	GenericTools::SqliteWrapper db("test.db3", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

	db.createTable("Photographers",
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

	{
		std::vector<YString> fields { "FirstName", "LastName", "Abbreviation" };

		typedef std::tuple<AnyString, AnyString, AnyString> TupleType;

		std::vector<TupleType> listValues
		{
			{ TupleType("Claire et Sébastien", "Gilles", "CSG") },
			{ TupleType("Aurélien", "Gilles", "AG") },
			{ TupleType("Annie et Christian", "Gilles", "ACG") },
			{ TupleType("Papy Mamie", "Sénépart", "PMS") },
			{ TupleType("René", "Wagon", "RW") },
			{ TupleType("Laurent et Céline", "Saint Cyr", "LCSC") },
			{ TupleType("Parents de Thuy", "", "THUY")}
		};

		std::for_each(listValues.cbegin(), listValues.cend(),
			[&](const TupleType& values)
			{
				db.insertData("Photographers", fields, values);
			}
			);


	}

	{
		std::vector<YString> fields {"Keyword", "Value", "Owner"};

		typedef std::tuple<AnyString, AnyString, AnyString> TupleType;

		std::vector<TupleType> listValues
		{
			{ TupleType("Exif.Canon.SerialNumber", "2280522782", "CSG") },
			{ TupleType("Exif.Image.Model", "FinePix A350", "CSG") },
			{ TupleType("Exif.Image.Model", "FinePix E500", "CSG") },
			{ TupleType("Exif.Canon.SerialNumber", "430125393", "AG") },
			{ TupleType("Exif.Image.Model", "FinePix S5000", "AG") },
			{ TupleType("Exif.Image.Model", "DSC-W70", "ACG") },
			{ TupleType("Exif.Canon.ModelID", "50593792", "ACG") },
			{ TupleType("Exif.Image.Model", "DMC-FS4", "PMS") },
			{ TupleType("ExifPhoto.__NikonSerialNumbers", "4045196", "RW") },
			{ TupleType("Exif.Canon.SerialNumber", "1531001946", "LCSC") },
			{ TupleType("Exif.Image.Model", "FinePix S6500fd", "THUY") },
			{ TupleType("Exif.Image.Model", "DMC-TZ1", "THUY") },
			{ TupleType("Exif.Image.Model", "SAMSUNG ST550 / SAMSUNG ST560 / VLUU ST550 / SAMSUNG TL225", "THUY") },
			{ TupleType("Exif.Nikon3.SerialNumber", "6100268", "THUY") },
			{ TupleType("Exif.Image.ImageDescription", "SONY DSC", "THUY") }
		};

		std::for_each(listValues.cbegin(), listValues.cend(),
			[&](const TupleType& values)
			{
				db.insertData("Cameras", fields, values);
			}
			);

	}

	return 0;
}
