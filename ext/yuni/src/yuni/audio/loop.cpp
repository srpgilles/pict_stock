#include "loop.h"
#include "queueservice.h"

namespace Yuni
{
namespace Audio
{

	Loop::Loop(QueueService* audioService):
		pAudioService(audioService),
		pClosing(false)
	{}

	void Loop::beginClose()
	{
		pClosing = true;
	}

	void Loop::endClose()
	{
		stop();
	}


	bool Loop::onLoop()
	{
		if (!pAudioService || pClosing)
			return false;
		pAudioService->updateDispatched();
		return true;
	}



} // namespace Audio
} // namespace Yuni
