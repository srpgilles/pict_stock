#ifndef __YUNI_NET_MESSAGE_QUEUESERVICE_H__
# define __YUNI_NET_MESSAGE_QUEUESERVICE_H__

# include "../../yuni.h"
# include "../net.h"
# include "../../thread/policy.h"
# include "../../core/event.h"
# include "fwd.h"
# include "transport.h"
# include "../../core/noncopyable.h"


namespace Yuni
{
namespace Net
{
namespace Message
{


	class QueueService final : private Yuni::NonCopyable<QueueService>, public Policy::ObjectLevelLockable<QueueService>
	{
	public:
		//! The threading policy
		typedef Policy::ObjectLevelLockable<QueueService>  ThreadingPolicy;

		/*!
		** \brief Different states of a queue service
		*/
		enum State
		{
			//! The queue service is currently stopped
			stStopped,
			//! The queue service is currently starting
			stStarting,
			//! The queue service is working
			stRunning,
			//! The queue service is current stopping its work
			stStopping
		};


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		QueueService();
		//! Destructor
		~QueueService();
		//@}


		//! \name Service management
		//@{
		/*!
		** \brief Try to start the queue service
		*/
		Error start();

		/*!
		** \brief Wait for the service to stop
		*/
		void wait();

		/*!
		** \brief Stop the server
		*/
		Error stop();

		//! Get if the queueservice is started
		bool started() const;
		//@}


	public:
		//! Listeners management
		class Listeners
		{
		public:
			/*!
			** \brief Add a new address where the server should listen for incoming connections
			**
			** This action will be effective the next time the server starts
			** \code
			** Net::Message::QueueService  server;
			** server.addListener("82.125.10.31", 4242, new Net::Transport::REST());
			** server.start();
			** server.wait();
			** server.stop();
			** \endcode
			*/
			Error add(const AnyString& address, const Port& port, Transport::ITransport::Ptr transport);

			/*!
			** \brief Clear all addresses where the server should listen for incoming connections
			**
			** This action will be effective the next time the server starts
			*/
			void clear();

		private:
			QueueService* pService;
			friend class QueueService;
		}
		listeners;

		//! Events
		class Events
		{
		public:
			//! Prototype event: The queue service is starting
			typedef Event<void (Error&)> Starting;
			//! Prototype event: The queue service has started
			typedef Event<void ()> Started;
			//! Prototype event: The queue service is stopping
			typedef Event<void ()> Stopping;
			//! Prototype event: The queue service has been stopped
			typedef Event<void ()> Stopped;
			//! Prototype event: An error has occured
			typedef Event<void (State, Error)> ErrorTriggered;
			//! Prototype event: accepting a client
			typedef Event<void (bool&, const String&, Port, const String&, Port)> ClientAccept;

		public:
			//! Event: The queue service is starting
			Starting starting;
			//! Event: The queue service has started and is ready for incoming connections
			Started  started;
			//! Event: The queue service is shutting down
			Stopping stopping;
			//! Event: The queue service is stopped
			Stopped  stopped;
			//! Event: The queue service has encountered an error
			ErrorTriggered  error;
			//! Event: A client try to connect to the server
			ClientAccept   accept;
			//! Event: A client has been successfully connected to the server
		}
		events;


	protected:
		//! Flag to know the state of the server
		State pState;
		//! Internal data
		Yuni::Private::Net::Message::QueueServiceData* pData;

		// Friends
		friend class Yuni::Private::Net::Message::Worker;
		friend class Listeners;

	}; // class QueueService






} // namespace Message
} // namespace Net
} // namespace Yuni

# include "queueservice.hxx"

#endif // __YUNI_NET_MESSAGE_QUEUESERVICE_H__
