#ifndef __YUNI_AUDIO_LOOP_H__
# define __YUNI_AUDIO_LOOP_H__

# include "../yuni.h"
# include "../core/event/loop.h"


namespace Yuni
{
namespace Audio
{
	//! Forward Declaration
	class QueueService;


	/*!
	** \brief The audio loop is where every audio event is run
	*/
	class Loop: public Core::EventLoop::IEventLoop<Loop, Core::EventLoop::Flow::Timer,
		Core::EventLoop::Statistics::None, true>
	{
	public:
		//! The loop itself
		typedef Core::EventLoop::IEventLoop<Loop,
			Core::EventLoop::Flow::Timer,       // The flow policy
			Core::EventLoop::Statistics::None,  // The statistics policy
			true>                               // Use a separate thread
			LoopType;

		typedef LoopType::RequestType RequestType;

	public:
		/*!
		** \brief Constructor
		**
		** \param audioService Audio queue service corresponding to this loop
		*/
		Loop(QueueService* audioService);

		//! Virtual destructor
		virtual ~Loop() { stop(); }

		/*!
		** \brief Redefinition of the onLoop() callback
		*/
		bool onLoop();

		void beginClose();

		void endClose();

	public:
		//! Audio queue service corresponding to this loop
		QueueService* pAudioService;

		//! When closing we must not execute onLoop()
		bool pClosing;

	}; // class Loop



} // namespace Audio
} // namespace Yuni

#endif // __YUNI_AUDIO_LOOP_H__
