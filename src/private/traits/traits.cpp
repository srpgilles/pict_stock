#include "traits.hpp"

namespace PictStock
{
namespace Private
{
namespace Traits
{

	Element::Element(const unsigned int nature)
	: nature(nature)
	{ }

	Year::Year()
		: Element(Year::index)
	{ }

	Month::Month()
		: Element(Month::index)
	{ }

	Day::Day()
		: Element(Day::index)
	{ }

	Hour::Hour()
		: Element(Hour::index)
	{ }

	Minute::Minute()
		: Element(Minute::index)
	{ }

	Second::Second()
		: Element(Second::index)
	{ }

	Photographer::Photographer()
		: Element(Photographer::index)
	{ }


} // namespace Traits
} // namespace Private
} // namespace PictStock
