# include <yuni/io/directory/iterator.h>
# include <yuni/core/string.h>
# include <yuni/core/noncopyable.h>
# include "../pict_stock.hpp"

using namespace Yuni;


namespace
{
    /*!
     * \brief "Translate" a name of a directory (or of the project for that matter)
     * to include guards name
     *Logger
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
        assert(out.empty());
        auto size = in.size();

        out << '_';

        for (unsigned int i = 0u; i < size; ++i)
        {
            const char& current = in[i];

            if (String::IsAlpha(current))
            {
                char upperCase = String::ToUpper(current);

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


    bool HeaderGuardsIterator::onStart(const String& rootFolder)
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

        assert("If not, something fishy in the code" && filename.size() > pLengthRootFolder);
        YString truncatedFilename(filename.begin() + pLengthRootFolder, filename.end());
        checkHeaderGuards(truncatedFilename);

        return IO::flowContinue;
    }

    void HeaderGuardsIterator::onTerminate()
    { }


    void HeaderGuardsIterator::checkHeaderGuards(const AnyString& filename)
    {
        YString correctHeaderGuard;
        Translate(correctHeaderGuard, filename);
        logs.notice(filename) << "\t -> " << correctHeaderGuard;
    }





} // namespace anonymous



int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    LoggingFacility logs;

    HeaderGuardsIterator iterator(logs, "PictStock", "/home/sebastien/Projets_logiciels/PictStock/src");

    if (!iterator.start())
    {
        logs.fatal() << "Problem encountered while scanning source directory";
        exit(EXIT_FAILURE);
    }

    iterator.wait();

    return 0;
}

