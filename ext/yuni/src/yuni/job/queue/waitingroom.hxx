#ifndef __YUNI_JOB_WAITING_ROOM_HXX__
# define __YUNI_JOB_WAITING_ROOM_HXX__


namespace Yuni
{
namespace Private
{
namespace QueueService
{

	inline bool WaitingRoom::empty() const
	{
		return (!pJobCount);
	}


	inline bool WaitingRoom::notEmpty() const
	{
		return (!(!pJobCount));
	}


	inline unsigned int WaitingRoom::size() const
	{
		return (unsigned int) pJobCount;
	}




} // namespace QueueService
} // namespace Private
} // namespace Yuni

#endif // __YUNI_JOB_WAITING_ROOM_HXX__


