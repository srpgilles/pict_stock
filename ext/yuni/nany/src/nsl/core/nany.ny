
import core.version;



class Nany
{
public
	method create
	{
		version.hi  := __asm.version.hi;
		version.lo  := __asm.version.lo;
		version.rev := __asm.version.rev;
	}

public
	var version: Core.Version;

} // class Nany

