#ifndef PICT_FRAME_HPP_
# define PICT_FRAME_HPP_

# include "private/scan_photo_directory.hpp"


namespace PictStock
{

	/*!
	** \brief Scan the entire photo directory, and choose randomly some photos
	** to be displayed on a digital frame
	**
	*/

	class YUNI_DECL PictFrame
	{
	public:


		//! Constructor & destructor
		//@{
		/*!
		** \briefConstructor
		**
		** \param[in] logs Logging facility
		** \param[in] pathFormat Path format object to use to identify date from path
		** \param[in] nbPhotos Number of photos requested on the digital frame
		** \param[in] beginDate Oldest date considered
		** \param[in] endDate Newest date considered
 		**
		*/
		PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
			unsigned int nbPhotos, const time_t beginDate, const time_t endDate,
			ReadDate::Mode mode);

		//@}








	};

} // namespace PictStock




#endif /* PICT_FRAME_HPP_ */
