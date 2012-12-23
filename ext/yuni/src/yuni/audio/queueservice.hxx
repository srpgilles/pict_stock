#ifndef __YUNI_AUDIO_QUEUESERVICE_HXX__
# define __YUNI_AUDIO_QUEUESERVICE_HXX__


namespace Yuni
{
namespace Audio
{

	inline sint64 QueueService::Emitters::elapsedTime(const AnyString& name)
	{
		return elapsedTime(get(name));
	}


	inline sint64 QueueService::Emitters::elapsedTime(Emitter::Ptr emitter)
	{
		return !emitter ? 0 : emitter->elapsedTime();
	}


	inline bool QueueService::Emitters::play(const AnyString& name)
	{
		return play(get(name));
	}


	inline bool QueueService::Emitters::pause(const AnyString& name)
	{
		return pause(get(name));
	}


	inline bool QueueService::Emitters::stop(const AnyString& name)
	{
		return stop(get(name));
	}




} // namespace Audio
} // namespace Yuni

#endif // __YUNI_AUDIO_QUEUESERVICE_HXX__
