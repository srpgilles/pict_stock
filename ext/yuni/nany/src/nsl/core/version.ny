
namespace Core;


class Version
{
published
	property hi:  uint := 0;
	property lo:  uint := 0;
	property rev: uint := 0;

public
	operator < (rhs: Version)
	{
		if hi < rhs.hi then
			return true;

		if hi = rhs.hi then
		{
			if lo < rhs.lo then
				return true;

			if lo = rhs.lo then
			{
				if rev < rhs.rev then
					return true;
			}
		}
		return false;
	}

	//! operator = : Determine whether two versions are strictly equals or not
	operator = (rhs: Version)
	{
		hi = rhs.hi and lo = rhs.lo and rev = rhs.rev;
	}

} // class Version


