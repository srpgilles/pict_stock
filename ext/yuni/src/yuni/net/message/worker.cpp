
#include "worker.h"
#include "queueservice.h"


namespace Yuni
{
namespace Private
{
namespace Net
{
namespace Message
{

	Worker::Worker(QueueService& queueservice, ITransport::Ptr transport) :
		pTransport(transport),
		pQueueService(queueservice)
	{}


	Worker::~Worker()
	{
		stop();
	}


	bool Worker::onExecute()
	{
		if (!(!pTransport)) // valid pointer to transport
		{
			// The current transport layer
			ITransport& transport = *pTransport;

			// Attach the current thread to the transport layer
			transport.attachedThread(this);

			// run the transport layer, and wait for it
			Yuni::Net::Error error = transport.run();

			// Detach the thread, just in case
			transport.attachedThread(nullptr);

			if (error != Yuni::Net::errNone)
				pQueueService.events.error(QueueService::stRunning, error);

			// directly unassign / destroy the transport here (from the thread
			// to avoid long unexplained delay
			pTransport = nullptr;
		}
		return true;
	}





} // namespace Message
} // namespace Net
} // namespace Private
} // namespace Yuni

