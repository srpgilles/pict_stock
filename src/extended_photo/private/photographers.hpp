#ifndef PHOTOGRAPHERS_HPP_
# define PHOTOGRAPHERS_HPP_

# include "../../pict_stock.hpp"
# include "../photographer.hpp"

namespace PictStock
{

namespace GenericTools
{
	class SqliteWrapper;
}


namespace Private
{



	/*!
	** \brief This class handles all the known photographers and cameras, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Photographers
	{
	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Photographers(GenericTools::SqliteWrapper& database);


		//@}

	private:

		//! List of all photographers (along with their cameras)
		Photographer::Vector pList;


	};



} // namespace Private
} // namespace PictStock




#endif /* PHOTOGRAPHERS_HPP_ */
