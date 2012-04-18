
#include <yuni/yuni.h>
#include <yuni/core/system/process.h>
#include <yuni/core/getopt.h>
#include <yuni/io/directory.h>
#include <yuni/io/file.h>

#include <iostream>
#include "ast/fwd.h"
#include "ast/cppwritervisitor.h"
#include "ast/treeprintervisitor.h"
#include "typing/typevisitor.h"
#include "parser/parser.h"

using namespace Yuni;


int main(int argc, char *argv[])
{
	GetOpt::Parser getopt;

	String::Vector inputFiles;
	String::Vector outputFiles;

	getopt.addParagraph(String() << "Nany to cpp (bootstrap)\n");

	// Simulation mode
	getopt.addParagraph("Input");
	// --economy
	getopt.addFlag(inputFiles, 'i', "input", "Input file (nany source file)");
	getopt.remainingArguments(inputFiles);
	getopt.addFlag(inputFiles, 'o', "output", "Output file (C++ source file)");


	getopt.addParagraph("\nFlags");


	getopt.addParagraph("\nMisc.");
	// --pid
	String optPID;
	getopt.add(optPID, 'p', "pid", "Specify the file where to write the process ID");

	// --version
	bool optVersion = false;
	getopt.addFlag(optVersion, 'v', "version", "Print the version of the solver and exit");


	// Ask to parse the command line
	if (!getopt(argc, argv))
		exit(getopt.errors() ? 1 : 0);

	// Version
	if (optVersion)
	{
		std::cout << "1.0" << std::endl;
		return false;
	}

	// PID
	if (!optPID.empty())
	{
		IO::File::Stream pidfile;
		if (pidfile.openRW(optPID))
			pidfile << Yuni::ProcessID();
		else
			std::cerr << "impossible to write pid file " << optPID << "\n";
	}

	if (inputFiles.empty())
	{
		std::cerr << "no input file.\n";
		return EXIT_FAILURE;
	}
	else
	{
		// Normalizing all filenames and checking for their existence
		String path;
		String tmp;
		String output;
		bool hasError = false;
		for (unsigned int i = 0; i != inputFiles.size(); ++i)
		{
			IO::MakeAbsolute(path, inputFiles[i]);
			IO::Normalize(tmp, path);
			inputFiles[i] = tmp;

			if (!IO::File::Exists(tmp))
			{
				std::cerr << tmp << ": file not found\n";
				hasError = true;
			}
		}

		if (hasError)
		{
			// We have encountered an error (merely a file was not found)
			return EXIT_FAILURE;
		}

		// We will guess the target file with the following algorithm for outputFiles :
		// - if empty, all input files with .cpp extension
		// - if not empty, the size must match with inputFiles
		//
		// Possible improvment :
		// - if size == 1 and is a directory, using this directory for all output files
		//
		if (outputFiles.empty())
		{
			for (unsigned int i = 0; i != inputFiles.size(); ++i)
			{
				IO::ExtractFilePath(path, inputFiles[i]);
				IO::ExtractFileNameWithoutExtension(tmp, inputFiles[i]);
				output.clear() << path << IO::Separator << tmp << ".cpp";
				outputFiles.push_back(output);
			}
		}
		else
		{
			if (outputFiles.size() != inputFiles.size())
			{
				std::cerr << "Please specify all target outputs\n";
				return EXIT_FAILURE;
			}
		}
	}


	// At this location, we must have the same amount of input/output files
	assert(inputFiles.size() == outputFiles.size() && "size mismatch");


	bool gotError = false;

	// processing all input files
	for (unsigned int i = 0; i != inputFiles.size(); ++i)
	{
		const String& input  = inputFiles[i];
		const String& output = outputFiles[i];

		std::cout << "processing " << input << '\n';

		// Parse the file and generate the AST
		Nany::Ast::Node* tree = parseFile(input.c_str());
		if (!tree)
		{
			gotError = true;
			continue; // processing the next input file
		}

		// Run the AST printer visitor on the AST
		Nany::Ast::TreePrinterVisitor printerVisitor;
		tree->accept(&printerVisitor);

		// Run the type visitor on the AST
		Nany::Typing::TypeVisitor typeVisitor;
		tree->accept(&typeVisitor);

		// Run the CPP writer visitor on the AST
		Nany::Ast::CppWriterVisitor visitor;
		if (!visitor.out.openRW(output))
		{
			std::cerr << output << ": i/o error, could not be opened for writing." << std::endl;
			gotError = true;
			delete tree;
			continue; // processing the next input file
		}

		tree->accept(&visitor);

		// release
		delete tree;
	}

	return gotError ? EXIT_FAILURE : 0;
}

