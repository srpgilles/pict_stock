
#include "resolution.h"
#include "../../core/hash/checksum/md5.h"
#include "../../core/math.h"
#include <cassert>


namespace Yuni
{
namespace Device
{
namespace Display
{


	Resolution::Resolution(unsigned int width, unsigned int height, uint8 bits)
	{
		assert(width  <= (unsigned) maximumWidth && "display resolution: width greater than Device::Display::Resolution::maximumWidth");
		assert(height <= (unsigned) maximumHeight && "display resolution: height greater than Device::Display::Resolution::maximumHeight");

		pBitsPerPixel = (32 == bits ||  24 == bits || 16 == bits || 8 == bits)
			? (uint8) bits : (uint8)32;
		pWidth  = Math::MinMax<unsigned int>(width,  minimumWidth,  maximumWidth);
		pHeight = Math::MinMax<unsigned int>(height, minimumHeight, maximumHeight);
	}


	String Resolution::toString() const
	{
		String ret;
		ret << pWidth << 'x' << pHeight;
		if (pBitsPerPixel)
			ret << " (" << (int)pBitsPerPixel << "Bits)";
		return ret;
	}


	bool Resolution::operator < (const Resolution& rhs) const
	{
		if (pWidth < rhs.pWidth)
			return true;
		if (pWidth == rhs.pWidth)
		{
			if (pHeight < rhs.pHeight)
				return true;
			if (pHeight == rhs.pHeight)
				return (pBitsPerPixel < rhs.pBitsPerPixel);
		}
		return false;
	}


	bool Resolution::operator > (const Resolution& rhs) const
	{
		if (pWidth > rhs.pWidth)
			return true;
		if (pWidth == rhs.pWidth)
		{
			if (pHeight > rhs.pHeight)
				return true;
			if (pHeight == rhs.pHeight)
				return (pBitsPerPixel > rhs.pBitsPerPixel);
		}
		return false;
	}


	std::ostream& Resolution::print(std::ostream& out) const
	{
		out << pWidth << "x" << pHeight;
		if (pBitsPerPixel)
			out << " (" << (int) pBitsPerPixel << "Bits)";
		return out;
	}


	Resolution& Resolution::operator = (const Resolution& p)
	{
		pWidth = p.pWidth;
		pHeight = p.pHeight;
		pBitsPerPixel = p.pBitsPerPixel;
		return (*this);
	}




} // namespace Display
} // namespace Device
} // namespace Yuni

