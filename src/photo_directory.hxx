#ifndef PHOTO_DIRECTORY_HXX_
#define PHOTO_DIRECTORY_HXX_

namespace PictStock
{


	inline bool PhotoDirectory::findDate(const DateString& date, std::list<YString>& folders) const
	{
		assert(folders.size() == 0 && "Only output parameter");
		auto iterator = pTree.find(date);

		if (iterator == pTree.end())
			return false;

		folders = iterator->second;
		return true;
	}



} // namespace PictStock


#endif /* PHOTO_DIRECTORY_HXX_ */
