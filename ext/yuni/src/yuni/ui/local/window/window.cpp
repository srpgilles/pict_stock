
#include "window.h"

#ifdef YUNI_OS_MAC
	// TODO: Write window creation for Cocoa. For the moment it does nothing
#else
#	ifdef YUNI_OS_WINDOWS
#		include "wingdi.h"
#   else
#		ifdef YUNI_OS_UNIX
#			include "x11.h"
#		else
#			error No window creation available for your platform!
#		endif
#	endif
#endif
#include <iostream>



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


	IWindow* Create()
	{
		# ifdef YUNI_WINDOWSYSTEM_MSW
		IWindow* wnd = new WinGDI();
		# endif
		# ifdef YUNI_WINDOWSYSTEM_X11
		IWindow* wnd = new X11();
		# endif
		# ifdef YUNI_OS_MAC
		IWindow* wnd = nullptr; // new Cocoa::CairoWindow(source, 32, false);
		# endif

		// Try to initialize
		if (!wnd || !wnd->initialize())
		{
			wnd->close();
			delete wnd;
			return nullptr;
		}
		return wnd;
	}




	IWindow::IWindow():
		pStyleSet(defaultStyleSet),
		pStayOnTop(false),
		pFullScreen(false),
		pBackgroundColor(0.0f, 0.0f, 1.0f),
		pRefreshRefCount(0)
	{
	}


	IWindow::~IWindow()
	{
	}


	void IWindow::onMinimize()
	{
		std::cout << "Caught Minimize event !" << std::endl;
	}


	void IWindow::onMaximize()
	{
		std::cout << "Caught Maximize event !" << std::endl;
	}


	void IWindow::onRestore()
	{
		std::cout << "Caught Restore event !" << std::endl;
	}


	void IWindow::onShow()
	{
		std::cout << "Caught Show event !" << std::endl;
	}


	void IWindow::onHide()
	{
		std::cout << "Caught Hide event !" << std::endl;
	}


	void IWindow::onResize(float width, float height)
	{
		std::cout << "Caught Resize event to (" << width << "," << height << ") !" << std::endl;
	}


	void onMove(float left, float top)
	{
		std::cout << "Caught Move event to (" << left << "," << top << ") !" << std::endl;
	}


	void IWindow::close()
	{
		onClose();
	}


	void IWindow::onCloseQuery(bool& /*canClose*/)
	{
		std::cout << "Caught Close event !" << std::endl;
	}


	void IWindow::onClose()
	{
		std::cout << "Caught Close event !" << std::endl;
	}


	void IWindow::style(unsigned int flags)
	{
		pStyleSet = flags;
		doUpdateStyle();
	}


	void IWindow::stayOnTop(bool alwaysOnTop)
	{
		pStayOnTop = alwaysOnTop;
		doUpdateStayOnTop();
	}


	void IWindow::backgroundColor(const Color& color)
	{
		pBackgroundColor = color;
		refresh();
	}


	void IWindow::backgroundColor(float r, float g, float b)
	{
		pBackgroundColor.red = r;
		pBackgroundColor.green = g;
		pBackgroundColor.blue = b;
		refresh();
	}


	void IWindow::refresh()
	{
		if (!pRefreshRefCount)
			doRefresh();
	}


	void IWindow::forceRefresh()
	{
		doRefresh();
	}


	void IWindow::beginUpdate()
	{
		++pRefreshRefCount;
	}


	void IWindow::endUpdate()
	{
		assert(pRefreshRefCount > 0);
		if (!--pRefreshRefCount)
          doRefresh();
	}


	const String& IWindow::caption() const
	{
		return pCaption;
	}





} // namespace Window
} // namespace Local
} // namespace UI
} // namespace Private
} // namespace Yuni
