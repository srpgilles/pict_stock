#ifndef POPULATE_DAY_FOLDER_HPP_
# define POPULATE_DAY_FOLDER_HPP_

# include "../extended_photo.hpp"

namespace SgPhoto
{
namespace Private
{

	/*!
	** \brief A proxy method of #SortPhotos in charge of inserting new photos inside a given target folder
	**
	** The point is to make sure no photo is lost and that a same photo can't be duplicated
	**
	** The names of the pictures must be relevant: if there is 2 photos in the same minute they should
	** be indexed "1" and "2", not the first without index and the second with "1" or "2"
	**
	** For instance:
	**  [good]
	**  	Photo_1313_CSG_1.jpg
	**  	Photo_1313_CSG_2.jpg
	**  [bad]
	**  	Photo_1313_CSG.jpg
	**  	Photo_1313_CSG_2.jpg
	**  [bad]
	**		Photo_1313_CSG_1.jpg
	**		Photo_1313_CSG_4.jpg
	**  [worse]
	**  	Photo_1313_CSG.jpg [one of the two has been lost]
	*/
	class PopulateDayFolder
	{

	public:
		//! Constructor & destructor
		//@{
		/*!
		** \brief Add new photos to a given target folder
		**
		** Make sure no photo is lost and that a same photo can't be duplicated
		**
		** \param[in, out] logs Logging facility
		** \param[in] targetFolder Folder in which the new photo will be inserted. Must exist
		** \param[in] targetDate
		** \param[in] newPhotos
		*/
		PopulateDayFolder(LoggingFacility& logs, const YString& targetFolder,
			const DateString& targetDate, ExtendedPhoto::Vector& newPhotos);

		//@}

	public:

		//! Logs
		mutable LoggingFacility& logs;

	private:

		/*!
		** \brief Scan new photos and check their date matches #pTargetDate
		**
		** This should be the case in most cases; but in case you are sorting pictures
		** with unreliable exif data it might not be
		** If disagreement, modify the exif by giving a fake date associated with a proper comment
		*/
		bool enforceDateInNewPhotos();



	private:

		//! Folder in which new photos will be stored
		const YString& pTargetFolder;

		//! Date of the photos to put into target folder
		const DateString& pTargetDate;

		//! New photos to be added in target folder
		ExtendedPhoto::Vector pNewPhotos;

		/*!
		** \brief Work variable to properly name all photos in target folder
		**
		** Key is the name given by #ExtendedPhoto and values are all photos that would
		** share this name if we do not introduce something else (an index)
		 */
		std::map<YString, ExtendedPhoto::Vector> pPhotosPerName;


	};




} // namespace Private
} // namespace SgPhoto


#endif /* POPULATEDAYFOLDER_HPP_ */
