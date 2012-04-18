
#include "queueservice.h"
#include <cassert>
#include "../core/point3D.h"
#include "../core/vector3D.h"
#include "../thread/signal.h"
#include "../private/audio/av.h"
#include "../private/audio/openal.h"

namespace Yuni
{
namespace Audio
{

	Atomic::Int<32> QueueService::sHasRunningInstance = 0;


	QueueService::QueueService() :
		pReady(false),
		pAudioLoop(this)
	{
		bank.pQueueService = this;
		emitter.pQueueService = this;
		emitter.pBank = &bank;
	}


	QueueService::~QueueService()
	{
		if (pReady)
			stop();
	}


	bool QueueService::start()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		// Do not initialize the manager twice
		if (pReady || sHasRunningInstance)
			return false;

		pAudioLoop.start();

		// Init audio from the audio loop
		{
			Thread::Signal signal;
			InitData initData(signal, pReady);
			Bind<bool()> callback;
			callback.bind(this, &QueueService::initDispatched, (InitData&)initData);
			pAudioLoop.dispatch(callback);
			// Wait for the initDispatched to finish
			signal.wait();
		}

		if (pReady)
		{
			sHasRunningInstance = 1;
			return true;
		}
		return false;
	}


	void QueueService::stop()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		// Do not stop the manager if it was not properly started
		if (!pReady)
			return;

		// Close OpenAL buffers properly
		bank.clear();

		// Close OpenAL emitters properly
		{
			Emitter::Map::iterator sEnd = emitter.pEmitters.end();
			for (Emitter::Map::iterator it = emitter.pEmitters.begin(); it != sEnd; ++it)
			{
				Private::Audio::OpenAL::DestroySource(it->second->id());
			}
		}
		emitter.pEmitters.clear();
		// Close OpenAL
		Yuni::Bind<bool()> callback;
		callback.bind(&Private::Audio::OpenAL::Close);
		pAudioLoop.dispatch(callback);
		pAudioLoop.stop();
		pReady = false;
		sHasRunningInstance = 0;
	}


	bool QueueService::initDispatched(InitData& data)
	{
		data.ready = Private::Audio::AV::Init() && Private::Audio::OpenAL::Init();
		// The variable must be considered as destroyed as soon as the method
		// 'notify()' is called
		bool ready = data.ready;
		data.signal.notify();
		return ready;
	}


	bool QueueService::loadSoundDispatched(const String& filePath)
	{
		// std::cout << "Loading file \"" << filePath << "\"..." << std::endl;

		// Try to open the file
		Private::Audio::AudioFile* file = Private::Audio::AV::OpenFile(filePath);
		if (!file)
			return false;

		// Try to get an audio stream from it
		Private::Audio::AudioStream* stream = Private::Audio::AV::GetAudioStream(file, 0);
		if (!stream)
		{
			Private::Audio::AV::CloseFile(file);
			return false;
		}

		// Get info on the audio stream
		int rate;
		int channels;
		int bits;
		if (0 != Private::Audio::AV::GetAudioInfo(stream, rate, channels, bits))
		{
			Private::Audio::AV::CloseFile(file);
			return false;
		}

		// Check the format
		stream->Format = Private::Audio::OpenAL::GetFormat(bits, channels);
		if (0 == stream->Format)
		{
			Private::Audio::AV::CloseFile(file);
			return false;
		}
		//std::cout << "Sound is " << bits << " bits " << (channels > 1 ? "stereo " : "mono ")
		//	  << rate << "Hz" << std::endl;

		// Associate the buffer with the stream
		{
			ThreadingPolicy::MutexLocker locker(*this);
			Sound::Ptr buffer = bank.get(filePath);
			assert(NULL != buffer);
			buffer->stream(stream);
		}

		return true;
	}


	bool QueueService::updateDispatched()
	{
		const Emitter::Map::iterator end = emitter.pEmitters.end();
		for (Emitter::Map::iterator it = emitter.pEmitters.begin(); it != end; ++it)
			(it->second)->updateDispatched();
		return true;
	}


	bool QueueService::playing() const
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::const_iterator end = emitter.pEmitters.end();
		for (Emitter::Map::const_iterator it = emitter.pEmitters.begin(); it != end; ++it)
		{
			if (it->second->playing())
				return true;
		}
		return false;
	}



	///////////////////////////////// Emitters


	Emitter::Ptr QueueService::Emitters::get(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(name);
		if (it == pEmitters.end())
			return nullptr;
		return it->second;
	}


	bool QueueService::Emitters::add(const AnyString& emitterName)
	{
		Emitter::Ptr newEmitter(new Emitter());

		ThreadingPolicy::MutexLocker locker(*this);

		if (!pQueueService->pReady)
			return false;

		// Create the emitter and add it
		pEmitters[emitterName] = newEmitter;

		Audio::Loop::RequestType callback;
 		callback.bind(newEmitter, &Emitter::prepareDispatched);
		// Dispatching...
 		pQueueService->pAudioLoop.dispatch(callback);

		return true;
	}


	bool QueueService::Emitters::attach(const AnyString& emitterName,
		const AnyString& bufferName)
	{
		Sound::Ptr buffer = pBank->get(bufferName);
		if (!buffer)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (!pQueueService->pReady)
				return false;

			Emitter::Map::iterator it = pEmitters.find(emitterName);
			if (it == pEmitters.end())
				return false;
			Emitter::Ptr emitter = it->second;

			Audio::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachBufferDispatched, buffer);
			// Dispatching...
			pQueueService->pAudioLoop.dispatch(callback);
		}
		return true;
	}


	bool QueueService::Emitters::attach(Emitter::Ptr emitter, const AnyString& bufferName)
	{
		if (!emitter || !bufferName)
			return false;

		Sound::Ptr buffer = pBank->get(bufferName);
		if (!buffer)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (!pQueueService->pReady)
				return false;

			Audio::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachBufferDispatched, buffer);
			// Dispatching...
			pQueueService->pAudioLoop.dispatch(callback);
		}
		return true;
	}


	bool QueueService::Emitters::attach(Emitter::Ptr emitter, Sound::Ptr buffer)
	{
		if (!emitter || !buffer)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (!pQueueService->pReady)
				return false;

			Audio::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachBufferDispatched, buffer);
			// Dispatching...
	 		pQueueService->pAudioLoop.dispatch(callback);
		}
		return true;
	}


	void QueueService::Emitters::detach(const AnyString& name)
	{
		return detach(get(name));
	}


	void QueueService::Emitters::detach(Emitter::Ptr emitter)
	{
		if (!emitter)
			return;

		Audio::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::detachBufferDispatched);
		// Dispatching...
 		pQueueService->pAudioLoop.dispatch(callback);
	}



	bool QueueService::Emitters::move(const AnyString& emitterName,
		const Point3D<>& position)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(emitterName);
		if (it == pEmitters.end())
			return false;
		it->second->position(position);
		return true;
	}


	bool QueueService::Emitters::move(const AnyString& emitterStr, const Point3D<>& position,
		const Vector3D<>& velocity, const Vector3D<>& direction)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(emitterStr);
		if (it == pEmitters.end())
			return false;

 		it->second->position(position);
 		it->second->velocity(velocity);
 		it->second->direction(direction);
		return true;
	}


	bool QueueService::Emitters::play(Emitter::Ptr emitter)
	{
		if (!emitter)
			return false;
		Audio::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::playSoundDispatched);
		// Dispatching...
 		pQueueService->pAudioLoop.dispatch(callback);
		return true;
	}


	bool QueueService::Emitters::stop(const Emitter::Ptr& emitter)
	{
		if (!emitter)
			return false;
		// TODO
		return false;
	}



	////////////////////////// Bank

	Sound::Ptr QueueService::Bank::get(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Sound::Map::iterator it = pBuffers.find(name);
		if (it == pBuffers.end())
			return nullptr;
		return it->second;
	}


	void QueueService::Bank::clear()
	{
		ThreadingPolicy::MutexLocker locker(*this);

		{
			Thread::Signal signal;
			Yuni::Bind<bool()> callback;
			Sound::Map::iterator bEnd = pBuffers.end();
			for (Sound::Map::iterator it = pBuffers.begin(); it != bEnd; ++it)
			{
				callback.bind(it->second, &Sound::destroyDispatched, signal);
				pQueueService->pAudioLoop.dispatch(callback);
				signal.wait();
				signal.reset();
			}
		}
		pBuffers.clear();
	}


	bool QueueService::Bank::load(const AnyString& filePath)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (!pQueueService->pReady)
			return false;

		// Create the buffer, store it in the map
		pBuffers[filePath] = new Sound(NULL);

		Yuni::Bind<bool()> callback;
		callback.bind(pQueueService, &QueueService::loadSoundDispatched, filePath);
		pQueueService->pAudioLoop.dispatch(callback);
		return true;
	}


	bool QueueService::Bank::unload(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (!pQueueService->pReady)
			return false;

		Sound::Map::iterator it = pBuffers.find(name);
		if (pBuffers.end() == it)
			return false;

		{
			Thread::Signal signal;
			Yuni::Bind<bool()> callback;

			callback.bind(it->second, &Sound::destroyDispatched, signal);
			pQueueService->pAudioLoop.dispatch(callback);
			signal.wait();
		}
		// Wait for data to be properly unloaded before removing from the list
		pBuffers.erase(it);

		return true;
	}


	unsigned int QueueService::Bank::duration(const AnyString& name)
	{
		Sound::Ptr buffer = get(name);
		if (!buffer)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (!pQueueService->pReady)
			return 0;
		return buffer->duration();
	}


	bool QueueService::running() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pReady && pAudioLoop.running();
	}






} // namespace Audio
} // namespace Yuni

