# include <yuni/io/directory/iterator.h>
# include <yuni/io/file/stream.h>
# include <yuni/core/string.h>
# include <yuni/core/noncopyable.h>
# include "../pict_stock.hpp"

using namespace Yuni;


namespace
{
    /*!
     * \brief "Translate" a name of a directory (or of the project for that matter)
     * to include guards name
     *
     * For instance, PictStock -> PICT_STOCK
     */
    void Translate(YString& out, const AnyString& in);


    /*!
     * \brief A derived class from HeaderGuardsIteratorIO::Directory::IIterator in charge
     * of scanning the source directories and scan all header files
     *
     */
    class YUNI_DECL HeaderGuardsIterator : public IO::Directory::IIterator<false>,
                                        private NonCopyable<HeaderGuardsIterator>
    {

    public:

        //! Constructor & destructor
        //@{
        //! Constructor
        HeaderGuardsIterator(LoggingFacility& logs, const AnyString& projectName,
            const AnyString& rootFolder);

        //! Destructor
        virtual ~HeaderGuardsIterator();

        //@}

        //! Flow
        typedef IO::Flow Flow;

    public:

        //! Logging facility
        LoggingFacility& logs;

    private:

        //! Check the header guards for given header file and correct them if necessary
        void checkHeaderGuards(const AnyString& filename);



        //! Overload IIterator methods
        //@{
        virtual bool onStart(const YString& rootFolder);

        virtual Flow onBeginFolder(const YString& completePath,
            const YString&, const YString& name);

        virtual void onEndFolder(const YString&, const YString&, const YString&);

        virtual Flow onFile(const YString&, const YString& folder,
            const YString& name, uint64 size);

        virtual void onTerminate();
        //@}

    private:

        //! Name of the project
        YString pProjectName;

        //! Index of first character to consider when translating into header guard
        AnyString::size_type pLengthRootFolder;

    }; // Class HeaderGuardsIterator


    void Translate(YString& out, const AnyString& in)
    {
        auto size = in.size();

        out << '_';

        for (unsigned int i = 0u; i < size; ++i)
        {
            const char& current = in[i];

            if (String::IsAlpha(current))
            {
                char upperCase = static_cast<char>(String::ToUpper(current));

                if (upperCase == current)
                    out << '_' << current;
                else
                    out << upperCase;
            }
            else if (current == IO::Separator)
            {
                out << "__";
            }
            else if (String::IsDigit(current))
            {
                out << current;
            }
            else
            {
                // Virtually anything else should be converted to a '_'
                out << '_';
            }
        }

        if (out.last() != '_')
            out << '_';

    }

   HeaderGuardsIterator::HeaderGuardsIterator(LoggingFacility& logs, const AnyString& projectName,
        const AnyString& rootFolder)
        : logs(logs),
          pProjectName(projectName),
          pLengthRootFolder(rootFolder.size() + 1u)
   {
       add(rootFolder);
   }



    HeaderGuardsIterator::~HeaderGuardsIterator()
    {
        // For code robustness and to avoid corrupt vtable
        stop();
    }


    bool HeaderGuardsIterator::onStart(const String& /*rootFolder*/)
    {
        return true;
    }


    HeaderGuardsIterator::Flow HeaderGuardsIterator::onBeginFolder(const String&,
        const String&, const String& /*name*/)
    {
        return IO::flowContinue;
    }

    void HeaderGuardsIterator::onEndFolder(const String&, const String&, const String&)
    { }


    HeaderGuardsIterator::Flow HeaderGuardsIterator::onFile(const String& filename,
        const String& /*folder*/, const String& name, uint64 /*size*/)
    {
        CString<6, false> ext;

        if (!IO::ExtractExtension(ext, name, false, false))
            return IO::flowContinue;

        // Criterion is extension that begins with h
        if (!ext.startsWith('h'))
            return IO::flowContinue;

        checkHeaderGuards(filename);

        return IO::flowContinue;
    }

    void HeaderGuardsIterator::onTerminate()
    { }


    /*!
    ** \brief RAII for temporary file written with correct include guards
    **
    ** Header file content is copied into NewFile, with header guards eventually corrected.
    **
    ** If successful, calling function will yield temporary filename and copy it
    ** to replace previous version of header file.
    **
    */
    class NewFile
    {
    public:

        //! Constructor and destructor
        //@{
        explicit NewFile(const AnyString& filename);

        ~NewFile();
        //@}

        //! Operator <<
        template<class U>
        inline NewFile& operator << (const U& u);

        //! Name of the temporary filename
        inline const YString& name() const { return pFileName; }

        //! Close the stream
        void close();


    private:

        //! Name of the temporary file
        YString pFileName;

        //! Stream
        IO::File::Stream pStream;

    };

    NewFile::NewFile(const AnyString& filename)
        : pFileName(filename)
    {
        pFileName << ".tmp";
        pStream.openRW(pFileName);
    }


    NewFile::~NewFile()
    {
        if (pStream.opened())
            pStream.close();

        IO::File::Delete(pFileName);
    }

    template<class U>
    NewFile& NewFile::operator << (const U& u)
    {
        pStream.operator << (u);
        return *this;
    }


    void NewFile::close()
    {
        pStream.close();
    }



    void HeaderGuardsIterator::checkHeaderGuards(const AnyString& filename)
    {
        // Prepare message in case of failure
        YString failureMsg("Couldn't modify automatically file ");
        failureMsg << filename;

        // Determine correct header guards
        assert("If not, something fishy in the code" && filename.size() > pLengthRootFolder);
        YString truncatedFilename(filename, pLengthRootFolder);

        YString correctHeaderGuard;
        {
            // Determine the header guard to apply
            Translate(correctHeaderGuard, pProjectName);
            Translate(correctHeaderGuard, truncatedFilename);
        }        

        // Open the file to study
        IO::File::Stream fileContent(filename, IO::OpenMode::read);

        // Open a copy in which the modified content will be written
        NewFile newFile(filename);
        bool isFileUnchanged(true);

        // Read line by line, and modify the three lines with header guards informations
        {
            YString line;

            // The first relevant line should be line with the first header guard
            // So line sbefore should be either empty or comments.
            bool isComment(false);

            while (fileContent.readline(line))
            {
                if (line.empty() || line.startsWith("//"))
                {
                    newFile << line << '\n';
                    continue;
                }

                if (line.startsWith("/*"))
                {
                    newFile << line << '\n';
                    isComment = true;
                    continue;
                }

                // If not in comment it means the line should be the header guard; which
                // is handled just after the while loop
                if (!isComment)
                    break;

                if (line.endsWith("*/"))
                {
                    isComment = false;
                    newFile << line << '\n';
                }
            }





//        while (fileContent.readline(line) && line.empty());

//        // It is acceptable that there is comment blocks before header guard

//        if (line.startsWith("/*"))
//        {
//            newFile << line << '\n';

//            while (fileContent.readline(line))
//            {
//                newFile << line << '\n';
//                if (line.contains("*/"))
//                    break;
//            }
//        }

//        // Empty lines might occur after comment block
//        while (fileContent.readline(line) && line.empty());

        if (!line.contains("ifndef"))
        {
            logs.error(failureMsg);
            return;
        }

        {
            YString expected("#ifndef ");
            expected << correctHeaderGuard;
            isFileUnchanged &= (line == expected);
            newFile << expected << '\n';
        }

        // Second relevant line is line #define...
            while (fileContent.readline(line) && line.empty());

            if (!line.contains("define"))
            {
                logs.error(failureMsg);
                return;
            }

            {
                YString expected("# define ");
                expected << correctHeaderGuard;
                isFileUnchanged &= (line == expected);
                newFile << expected << '\n';
            }

            // Copy the rest of the file except the last relevant line,
            // which should be endif one
            YString lastRelevantLine;
            while (fileContent.readline(lastRelevantLine) && lastRelevantLine.empty())
                newFile << '\n';

            bool previousLineEmpty(false);

            while (fileContent.readline(line))
            {
                bool isLineEmpty = line.empty();

                if (!previousLineEmpty && !isLineEmpty)
                    newFile << lastRelevantLine;

                newFile << '\n';

                // Prepare next iteration
                if (isLineEmpty)
                    previousLineEmpty = true;
                else
                {
                    previousLineEmpty = false;
                    lastRelevantLine = line;
                }
            }

            // LastRelevantLine should contain the last relevant line, with an endif in it
            if (lastRelevantLine.empty() || !lastRelevantLine.contains("endif"))
            {
                logs.error(failureMsg);
                return;
            }

            {
                YString expected("#endif /* ");
                expected << correctHeaderGuard << " */";
                isFileUnchanged &= (lastRelevantLine == expected);
                newFile << expected << '\n';
            }
        }

        if (!isFileUnchanged)
        {
            newFile.close();
            IO::File::Copy(newFile.name(), filename);
            IO::File::Delete(newFile.name());
        }
    }

} // namespace anonymous



int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    LoggingFacility logs;

   // HeaderGuardsIterator iterator(logs, "PictStock", "/home/sebastien/Projets_logiciels/PictStock/src");
 HeaderGuardsIterator iterator(logs, "Felisce", "/tmp");

    if (!iterator.start())
    {
        logs.fatal() << "Problem encountered while scanning source directory";
        exit(EXIT_FAILURE);
    }

    iterator.wait();

    return 0;
}

