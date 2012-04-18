#ifndef __YUNI_PRIVATE_AUDIO_OPENAL_H__
# define __YUNI_PRIVATE_AUDIO_OPENAL_H__

# include "../../yuni.h"
# include <list>
# include "../../core/vector3D.h"
# include "../../core/point3D.h"
# ifdef YUNI_OS_MACOS
#	include <OpenAL/al.h>
#	include <OpenAL/alc.h>
# else
#	include <al.h>
#	include <alc.h>
# endif

namespace Yuni
{
namespace Private
{
namespace Audio
{

	/*!
	** \brief OpenAL wrapper
	*/
	class OpenAL
	{
	public:
		//! \name Enums
		//@{
		enum DistanceModel
		{
			None,
			InverseDistance,
			InverseDistanceClamped,
			LinearDistance,
			LinearDistanceClamped,
			ExponentDistance,
			ExponentDistanceClamped
		};
		//@}


	public:
		/*!
		** \brief Initialize OpenAL device and context
		*/
		static bool Init();

		/*!
		** \brief Close OpenAL context and device
		*/
		static bool Close();

		/*!
		** \brief Convert to an OpenAL format
		** \param bits Number of bits per sample
		** \param channels Number of channels
		** \returns An ALenum containing the format, 0 if none found
		*/
		static ALenum GetFormat(unsigned int bits, unsigned int channels);

		static void SetDistanceModel(DistanceModel model);

		/*!
		** \brief Create OpenAL buffers
		** \param[in] nbBuffers Number of buffers to create
		** \param[out] An array of IDs of the created buffers
		*/
		static bool CreateBuffers(int nbBuffers, unsigned int* buffers);

		static void DestroyBuffers(int nbBuffers, unsigned int* buffers);

		static void SetListener(float position[3], float velocity[3], float orientation[6]);

		/*!
		** \brief Create an OpenAL source
		** \returns The source's ID, 0 if an error is encountered.
		*/
		static unsigned int CreateSource(Point3D<> position, Vector3D<> velocity,
			Vector3D<> direction, float pitch, float gain, bool attenuate, bool loop);

		//! Destroy an OpenAL source
		static void DestroySource(unsigned int source);

		//! Play an OpenAL source
		static bool PlaySource(unsigned int source);

		//! Stop an OpenAL source
		static bool StopSource(unsigned int source);

		//! Pause an OpenAL source
		static bool PauseSource(unsigned int source);

		//! Is the source currently playing ?
		static bool IsSourcePlaying(unsigned int source);

		//! Is the source currently playing ?
		static bool IsSourcePaused(unsigned int source);

		//! Modify characteristics of an existing source
		static bool ModifySource(unsigned int source, float pitch, float gain,
			bool attenuate, bool loop);
		//! Move an existing source
		static bool MoveSource(unsigned int source, const Point3D<>& position,
			const Vector3D<>& velocity, const Vector3D<>& direction);

		//! Bind a sound buffer to an OpenAL source
		static bool BindBufferToSource(unsigned int buffer, unsigned int source);

		//! Unind an OpenAL source from any buffer
		static void UnbindBufferFromSource(unsigned int source);

		//! Queue a sound buffer for playing on a source
		static bool QueueBufferToSource(unsigned int buffer, unsigned int source);

		//! Unqueue a sound buffer from a source
		static unsigned int UnqueueBufferFromSource(unsigned int source);

		//! Get current playback position on a source
		static float SourcePlaybackPosition(unsigned int source);

		//! Set current playback position on a source
		static void SetSourcePlaybackPosition(unsigned int source, float position);

		//! Set data on a sound buffer
		static bool SetBufferData(unsigned int buffer, int format, void* data,
			size_t count, int rate);

	}; // OpenAL

} // namespace Audio
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_AUDIO_OPENAL_H__
