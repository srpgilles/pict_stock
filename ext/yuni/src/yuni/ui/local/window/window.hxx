#ifndef __YUNI_UI_LOCAL_WINDOW_WINDOW_HXX__
# define __YUNI_UI_LOCAL_WINDOW_WINDOW_HXX__

# include "../../../core/string.h"
# include <cassert>


namespace Yuni
{
namespace Private
{
namespace UI
{
namespace Local
{
namespace Window
{

	template<class StringT>
	inline void IWindow::caption(const StringT& newString)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, IWindowCaption_InvalidTypeForBuffer);
		pCaption = newString;
		doUpdateCaption();
	}



	inline uint IWindow::style() const
	{
		return pStyleSet;
	}


	inline bool IWindow::stayOnTop() const
	{
		return pStayOnTop;
	}


	inline const Color& IWindow::backgroundColor() const
	{
		return pBackgroundColor;
	}





} // namespace Window
} // namespace Local
} // namespace UI
} // namespace Private
} // namespace Yuni

#endif // __YUNI_UI_LOCAL_WINDOW_WINDOW_HXX__
