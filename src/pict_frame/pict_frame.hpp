#ifndef PICT_FRAME_HPP_
# define PICT_FRAME_HPP_

# include "private/scan_photo_directory.hpp"
# include "../extended_photo/extended_photo.hpp"


namespace PictStock
{

	/*!
	** \brief Scan the entire photo directory, and choose randomly some photos
	** to be displayed on a digital frame
	**
	*/

	class YUNI_DECL PictFrame : private Yuni::NonCopyable<PictFrame>
	{
	public:


		//! Constructor & destructor
		//@{
		/*!
		** \briefConstructor
		**
		** \param[in] logs Logging facility
		** \param[in] photoDirectory Folder in which the entire pool of photos considered
		** is stored
		** \param[in] outputDirectory Output directory (probably on a photo card such as a SD)
		** \param[in] pathFormat Path format object to use to identify date from path
		** \param[in] nbPhotos Number of photos requested on the digital frame
		** \param[in] beginDate Oldest date considered
		** \param[in] endDate Newest date considered
		** \param[in] isChronological If true, pictures will be sort chronologically
 		**
		*/
		PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
			const YString& photoDirectory, const YString& outputDirectory,
			unsigned int nbPhotos, const time_t beginDate, const time_t endDate,
			ReadDate::Mode mode, bool isChronological);

		//@}

	public:

		//! Logging facility
		LoggingFacility& logs;


	private:

		/*!
		** \brief Prepare output directory for new photos
		**
		** Delete any JPG files in it if it already exists, or create it if it does not
		 */
		void prepareOutputDirectory(const YString& outputDirectory) const;

		/*!
		** \brief Select the requested number of photos
		*/
		void selectPhotos(const std::deque<YString>& pool);

		/*!
		** \brief Copy selected photos to output directory
		*/
		void copyToOutputDirectory(const YString& outputDirectory) const;

	private:

		//! Number of photos for the digital frame
		unsigned int pNbPhotos;

		//! Photos that have been selected
		std::list<ExtendedPhoto::Ptr> pPhotosChosen;

	};

} // namespace PictStock




#endif /* PICT_FRAME_HPP_ */
