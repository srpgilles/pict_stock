#ifndef POPULATE_DAY_FOLDER_HPP_
# define POPULATE_DAY_FOLDER_HPP_

# include "../../extended_photo/extended_photo.hpp"

namespace PictStock
{
	//! Forward declaration
	namespace PhotoDirectory
	{
		class PathFormat;
	}

	namespace ExtendedPhoto
	{
		class Database;
	}

namespace SortNewPhotos
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
	class YUNI_DECL PopulateDayFolder : private Yuni::NonCopyable<PopulateDayFolder>
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
		** \param[in] targetInfos Informations relevant for the target
		** \param[in] newPhotos List of new photos to be inserted in the target folder
		** \param[in] summaryFile File in which all operations will be saved
		*/
		PopulateDayFolder(LoggingFacility& logs,
			const Database::Database& database,
			const PhotoDirectory::PathFormat& pathFormat,
			const YString& targetFolder,
			const ExtendedPhoto::PathInformations& targetInfos,
			ExtendedPhoto::ExtendedPhoto::Vector& newPhotos,
			const YString& summaryFile);

		//! Destructor
		~PopulateDayFolder();
		//@}

		//! Method that does the actual work
		bool proceed();

	public:

		//! Logs
		LoggingFacility& logs;

	private:


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
		const ExtendedPhoto::PathInformations& pTargetInformations;

		//!
		const PhotoDirectory::PathFormat& pPathFormat;

		//! New photos to be added in target folder
		ExtendedPhoto::ExtendedPhoto::Vector pNewPhotos;

		/*!
		** \brief Work variable to properly name all photos in target folder
		**
		** Key is the name given by #ExtendedPhoto and values are all photos that would
		** share this name if we do not introduce something else (an index)
		 */
		std::map<YString, ExtendedPhoto::ExtendedPhoto::Vector> pPhotosPerName;

		//! Path to the file in which all operations are recorded
		const YString& pSummaryFile;

		//! Object that keeps all known cameras under check
		const Database::Database& pDatabase;
	};



} // namespace Private
} // namespace SortNewPhotos
} // namespace PictStock


#endif /* POPULATEDAYFOLDER_HPP_ */
