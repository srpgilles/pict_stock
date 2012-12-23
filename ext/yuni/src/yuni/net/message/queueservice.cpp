
#include "queueservice.h"
#include "worker.h"
#include "../../thread/array.h"


typedef Yuni::Thread::Array<Yuni::Private::Net::Message::Worker>  Workers;

typedef Yuni::Net::Message::Transport::ITransport::Set  ListenerList;



namespace Yuni
{
namespace Private
{
namespace Net
{
namespace Message
{

	class QueueServiceData
	{
	public:
		//! All workers
		Workers workers;
		//! All addresses to listen
		ListenerList listeners;

	}; // class QueueServiceData



} // namespace Message
} // namespace Net
} // namespace Private
} // namespace Yuni




namespace Yuni
{
namespace Net
{
namespace Message
{

	static inline void InitializeInternalData(Yuni::Private::Net::Message::QueueServiceData* data)
	{
		if (not data)
			data = new Yuni::Private::Net::Message::QueueServiceData();
	}



	QueueService::QueueService() :
		pState(stStopped),
		pData(nullptr)
	{
		listeners.pService = this;
	}


	QueueService::~QueueService()
	{
		// Stopping all workers
		stop();
		// release internal data
		delete pData;
	}


	Error  QueueService::Listeners::add(const AnyString& address, const Port& port, Transport::ITransport::Ptr transport)
	{
		if (not port.valid())
			return errInvalidPort;
		if (not transport || transport->mode != Transport::tmServer)
			return errInvalidTransport;

		// The address
		if (not address)
			return errInvalidHostAddress;
		transport->address = address;
		transport->port    = port;

		// Adding the new address
		{
			ThreadingPolicy::MutexLocker locker(*pService);
			InitializeInternalData(pService->pData);
			if (not pService->pData->listeners.insert(transport).second)
				return errDupplicatedAddress;
		}
		return errNone;
	}


	void QueueService::Listeners::clear()
	{
		ThreadingPolicy::MutexLocker locker(*pService);
		if (pService->pData)
			pService->pData->listeners.clear();
	}


	Error QueueService::start()
	{
		// Checking if the service is not already running
		{
			ThreadingPolicy::MutexLocker locker(*this);

			// Directly stop if there is no transport available
			if (not pData || pData->listeners.empty())
			{
				pState = stStopped;
				return errNoTransport;
			}

			// checking the current state
			if (pState != stStopped)
				return (pState == stRunning) ? errNone : errUnknown;

			// ok, let's start
			pState = stStarting;
		}

		// note: from now on, we know that pData is initialized due to the previous check

		// no error by default
		Error err = errNone;

		// The internal mutex must be unlocked whenever an event is called to prevent
		// any deadlocks.
		events.starting(err);
		if (err != errNone)
		{
			ThreadingPolicy::MutexLocker locker(*this);
			pState = stStopped;
			return err;
		}

		// Trying to start all workers
		{
			ThreadingPolicy::MutexLocker locker(*this);

			// destroy all workers, just to be sure
			Workers& workers = pData->workers;
			workers.stop();
			workers.clear();
			// Ok, attempt to start the server from the real implementation
			// recreating all workers
			ListenerList& listeners = pData->listeners;
			ListenerList::iterator end = listeners.end();
			for (ListenerList::iterator i = listeners.begin(); i != end; ++i)
			{
				// start the transport
				if (errNone != (err = (*i)->start()))
					break;

				// creating a new worker
				workers += new Yuni::Private::Net::Message::Worker(*this, *i);
			}

			// The new state
			pState = (err == errNone) ? stRunning : stStopped;
		}

		if (err == errNone)
		{
			// Great ! The server is working !
			events.started();
			return errNone;
		}
		else
		{
			ThreadingPolicy::MutexLocker locker(*this);
			// destroy all workers
			Workers& workers = pData->workers;
			workers.stop();
			workers.clear();
		}

		// An error has occured
		events.error(stStarting, err);
		return err;
	}


	void QueueService::wait()
	{
	}


	Error QueueService::stop()
	{
		// Checking if the service is not already running
		{
			ThreadingPolicy::MutexLocker locker(*this);
			if (pData == NULL)
				return errNone;
			if (pState != stRunning)
				return (pState == stStopped) ? errNone : errUnknown;
			pState = stStopping;
		}

		// The internal mutex must be unlocked whenever an event is called to prevent
		// any deadlocks.
		events.stopping();

		Error err = errNone;

		// Trying to start all workers
		{
			ThreadingPolicy::MutexLocker locker(*this);

			// destroy all workers
			Workers& workers = pData->workers;
			workers.stop();
			workers.clear();
			// The current state
			pState = (err == errNone) ? stStopped : stRunning;
		}

		if (err == errNone)
		{
			// Great ! The server is working !
			events.stopped();
			return errNone;
		}

		// An error has occured
		events.error(stStopping, err);
		return err;
	}


	bool QueueService::started() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return (pState == stStarting) or (pState == stRunning);
	}





} // namespace Message
} // namespace Net
} // namespace Yuni

