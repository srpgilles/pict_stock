
#include "array.h"
#include <iostream>


namespace Yuni
{
namespace Bit
{


	template<bool ValueT>
	static uint Find(const Bit::Array::BufferType& pBuffer, uint pCount, uint offset)
	{
		// bitmask
		static const unsigned char mask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
		// alias to npos
		enum { npos = Yuni::Bit::Array::npos };

		for (uint i = (offset >> 3); i < pBuffer.size(); ++i)
		{
			if ((unsigned char)(pBuffer[i]) != (ValueT ? (unsigned char)0 : (unsigned char)0xFF))
			{
				const unsigned char c = static_cast<unsigned char>(pBuffer[i]);

				// trivial cases
				if (ValueT && c == 0xFF)
				{
					uint p = i * 8;
					p = (p < offset) ? offset : p;
					return (p < pCount) ? p : npos;
				}
				if (!ValueT && c == 0)
				{
					uint p = i * 8;
					p = (p < offset) ? offset : p;
					return (p < pCount) ? p : npos;
				}

				// the current absolute position
				const uint absOffset = i * 8;

				// bit scan
				if (absOffset < offset)
				{
					for (uint p = 0; p != 8; ++p)
					{
						if (ValueT == (0 != (c & mask[p])))
						{
							p += absOffset;
							if (p >= offset)
								return (p < pCount) ? p : npos;
							// restoring previous value
							p -= absOffset;
						}
					}
				}
				else
				{
					// TODO : we can use ffs here
					for (uint p = 0; p != 8; ++p)
					{
						if (ValueT == (0 != (c & mask[p])))
						{
							p += absOffset;
							return (p < pCount) ? p : npos;
						}
					}

				}
			}
		}
		return npos;
	}





	uint Array::findFirstSet(uint offset) const
	{
		return Find<true>(pBuffer, pCount, offset);
	}


	uint Array::findFirstUnset(uint offset) const
	{
		return Find<false>(pBuffer, pCount, offset);
	}


	bool Array::any() const
	{
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if (pBuffer[i] != 0)
				return true;
		}
		return false;
	}


	bool Array::none() const
	{
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if (pBuffer[i] != 0)
				return false;
		}
		return true;
	}


	bool Array::all() const
	{
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if ((unsigned char) pBuffer[i] != 0xFF)
				return false;
		}
		return true;
	}




} // namespace Bit
} // namespace Yuni

