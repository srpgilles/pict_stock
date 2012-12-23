#ifndef __YUNI_NET_MESSAGE_TRANSPORT_TRANSPORT_H__
# define __YUNI_NET_MESSAGE_TRANSPORT_TRANSPORT_H__

# include "../../../yuni.h"
# include "../../../core/string.h"
# include "../../../core/noncopyable.h"
# include "../../../thread/thread.h"
# include "../fwd.h"
# include "../../errors.h"
# include "../../hostaddressport.h"
# include "../../port.h"
# include <set>


namespace Yuni
{
namespace Net
{
namespace Message
{
namespace Transport
{

	enum Mode
	{
		tmNone = 0,
		//! Server mode
		tmServer,
		//! Client mode
		tmClient
	};


	/*!
	** \brief Transport layer for messages (abstract)
	**
	** A transport layer is not thread-safe.
	*/
	class ITransport : private NonCopyable<ITransport>
	{
	public:
		//! The most suitable smart pointer for the class
		typedef SmartPtr<ITransport>  Ptr;
		//! Set
		typedef std::set<Ptr> Set;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		explicit ITransport(Mode m);
		//! Destructor
		virtual ~ITransport();
		//@}

		//! \name Attached thread
		//@{
		//! Get the attached thread
		Thread::IThread*  attachedThread();
		//! Get the attached thread (const)
		const Thread::IThread*  attachedThread() const;
		//! Set the attached thread
		void attachedThread(Thread::IThread* thread);
		//@}


		//! Service
		//@{
		//! Start the service
		virtual Yuni::Net::Error  start() = 0;
		//! Run the transport layer (must block until finished)
		virtual Yuni::Net::Error  run() = 0;
		//! Ask to stop the transport layer (if not already done)
		virtual void stop() = 0;
		//@}

	public:
		//! Address to listen
		HostAddress  address;
		//! Port
		Port port;
		//! Mode (server/client)
		const Mode mode;

	protected:
		//! The attached thread
		Thread::IThread* pAttachedThread;

	}; // class Transport






} // namespace Transport
} // namespace Server
} // namespace Net
} // namespace Yuni

# include "transport.hxx"

#endif // __YUNI_NET_MESSAGE_TRANSPORT_TRANSPORT_H__
