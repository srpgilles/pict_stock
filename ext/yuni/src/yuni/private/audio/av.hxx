#ifndef __YUNI_PRIVATE_AUDIO_AV_HXX__
# define __YUNI_PRIVATE_AUDIO_AV_HXX__

#include "../../core/string.h"

namespace Yuni
{
namespace Private
{
namespace Audio
{

	template<typename StringT>
	AudioFile* AV::OpenFile(const StringT& path)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, AV_OpenFile_InvalidFileNameType);

		AudioFile* file = (AudioFile*)calloc(1, sizeof(AudioFile));
# if LIBAVFORMAT_VERSION_MAJOR < 53
		if (file && !av_open_input_file(&file->FormatContext,
			Traits::CString<StringT>::Perform(path), NULL, 0, NULL))
# else
		if (file && !avformat_open_input(&file->FormatContext,
			Traits::CString<StringT>::Perform(path), NULL, NULL))
# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		{
			// After opening, we must search for the stream information because not
			// all formats will have it in stream headers (eg. system MPEG streams)
# if LIBAVFORMAT_VERSION_MAJOR < 53
			if (av_find_stream_info(file->FormatContext) >= 0)
# else
			if (avformat_find_stream_info(file->FormatContext, NULL) >= 0)
# endif // LIBAVFORMAT_VERSION_MAJOR < 53
				return file;
# if LIBAVFORMAT_VERSION_MAJOR < 53
			av_close_input_file(file->FormatContext);
# else
			avformat_close_input(&file->FormatContext);
# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		}
		free(file);
		return NULL;
	}



} // namespace Audio
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_AUDIO_AV_HXX__
