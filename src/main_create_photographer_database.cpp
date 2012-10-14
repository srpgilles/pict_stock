#include "tools/sqlite_wrapper.hpp"

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

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

	{
		std::vector<AnyString> fields { "FirstName", "LastName", "Abbr" };

		std::list<std::vector<AnyString> > listValues
		{
			{ "Claire et Sébastien", "Gilles", "CSG" },
			{ "Aurélien", "Gilles", "AG" },
			{ "Annie et Christian", "Gilles", "ACG" },
			{ "Papy Mamie", "Sénépart", "PMS" },
			{ "René", "Wagon", "RW" },
			{ "Laurent et Céline", "Saint Cyr", "LCSC" },
			{ "Parents de Thuy", "", "THUY"}
		};

		std::for_each(listValues.cbegin(), listValues.cend(),
			[&](const std::vector<AnyString>& values)
			{
				db.insertData("Photographers", fields, values);
			}
			);


	}

	{
		std::vector<AnyString> fields {"Keyword", "Value", "Owner"};

		std::list<std::vector<AnyString> > listValues
		{
			{ "Exif.Canon.SerialNumber", "2280522782", "CSG" },
			{ "Exif.Image.Model", "FinePix A350", "CSG" },
			{ "Exif.Image.Model", "FinePix E500", "CSG" },
			{ "Exif.Canon.SerialNumber", "430125393", "AG" },
			{ "Exif.Image.Model", "FinePix S5000", "AG" },
			{ "Exif.Image.Model", "DSC-W70", "ACG" },
			{ "Exif.Canon.ModelID", "50593792", "ACG" },
			{ "Exif.Image.Model", "DMC-FS4", "PMS" },
			{ "ExifPhoto.__NikonSerialNumbers", "4045196", "RW" },
			{ "Exif.Canon.SerialNumber", "1531001946", "LCSC" },
			{ "Exif.Image.Model", "FinePix S6500fd", "THUY" },
			{ "Exif.Image.Model", "DMC-TZ1", "THUY" },
			{ "Exif.Image.Model", "SAMSUNG ST550 / SAMSUNG ST560 / VLUU ST550 / SAMSUNG TL225", "THUY" },
			{ "Exif.Nikon3.SerialNumber", "6100268", "THUY" },
			{ "Exif.Image.ImageDescription", "SONY DSC", "THUY" }
		};

		std::for_each(listValues.cbegin(), listValues.cend(),
			[&](const std::vector<AnyString>& values)
			{
				db.insertData("Cameras", fields, values);
			}
			);

	}

	return 0;
}
