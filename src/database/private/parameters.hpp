#ifndef PARAMETERS_HPP_
# define PARAMETERS_HPP_

# include <yuni/core/string.h>
# include "table.hpp"
# include <yuni/core/noncopyable.h>


namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace Database
{
namespace Private
{


    namespace TableParameters
    {
        struct Key
        {
            typedef Yuni::CString<30, false> WrappedType;
            static YString FieldName() { return "Key"; }
        };

        struct Value
        {
            typedef YString WrappedType;
            static YString FieldName() { return "Value"; }
        };

    } // namespace TableParameters

    namespace Schema
    {
        struct Parameters
        {
            static Yuni::CString<11, false> Name() { return "Parameters"; }
            static YString Schema()
            {
                return
                    "Key varchar(20),"
                    "Value varchar,"
                    "UNIQUE (Key) ON CONFLICT ABORT";
            }
        };
    } // namespace Schema




	/*!
    ** \brief This class handles generic parameters, such as the directory
    ** in which all photos are stored
	*/
    class YUNI_DECL Parameters : private Yuni::NonCopyable<Parameters>,
                                 public Table<Schema::Parameters>
	{


    public:

        //! Tuple type for rows of the table cameras in sqlite database
        typedef std::tuple<TableParameters::Key, TableParameters::Value> Tuple;

        /*!
        ** \brief Yields the WrappedType hold by each element of the input tuple
        **
        ** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
        ** 	[Keyword::WrappedType, Value::WrappedType, Owner::WrappedType]
        */
        typedef GenericTools::Tuple::WrappedType<Tuple>::type TupleWrappedType;

        //! Name of the table in sqlite database
        static YString TableName() { return Schema::Parameters::Name(); }

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
        explicit Parameters(GenericTools::SqliteWrapper& database,
            nsTable::Values mode = nsTable::load);
		//@}



    private:

        /*!
        ** \brief Load existing data
        **
        ** Expected to be called in constructor if load mode
        */
        void load();


	private:

		//! Reference to database object
		GenericTools::SqliteWrapper& pDatabase;

        /*!
        **\brief Rows in sqlite database table related to generic parameters
        **
        */
        std::vector<TupleWrappedType> pRows;

	};

} // namespace Private
} // namespace Database
} // namespace PictStock

# include "parameters.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
