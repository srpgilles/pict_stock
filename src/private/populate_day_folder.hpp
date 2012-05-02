#ifndef POPULATE_DAY_FOLDER_HPP_
# define POPULATE_DAY_FOLDER_HPP_

# include "../extended_photo.hpp"

namespace PictStock
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
	class YUNI_DECL PopulateDayFolder
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
		** \param[in] targetDate Date of the pĥotos inside that folder. Format YYYYMMDD
		** \param[in] newPhotos List of new photos to be inserted in the target folder
		** \param[in] summaryFile File in which all operations will be saved
		*/
		PopulateDayFolder(LoggingFacility& logs, const YString& targetFolder,
			const DateString& targetDate, ExtendedPhoto::Vector& newPhotos,
			const YString& summaryFile);

		//! Destructor
		~PopulateDayFolder();
		//@}

		//! Method that does the actual work
		bool proceed();

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

		/*!
		** \brief Insert existing photos in #pPhotosPerName
		**
		*/
		bool insertExistingPhotos();

		/*!
		** \brief Insert new photos in #pPhotosPerName
		**
		*/
		void insertNewPhotos();

		/*!
		** \brief Put all photos with appropriate names in #pTargetFolder
		**
		*/
		bool putPhotosInTarget();

		/*!
		** \brief Yuni doesn't have any move function for directory; mimic it crudely
		**
		** This method also register the operations done in a file
		**
		** \param[in] logs Logging facility
		** \param[in] origin Original file
		** \param[in] target Target file
		*/
		bool moveFile(LoggingFacility& logs, const YString& origin, const YString& target) const;


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

		//! Path to the file in which all operations are recorded
		const YString& pSummaryFile;
	};




} // namespace Private
} // namespace PictStock


#endif /* POPULATEDAYFOLDER_HPP_ */
