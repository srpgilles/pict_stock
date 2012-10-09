#ifndef __YUNI_UI_LOCAL_WINDOW_WINGDI_H__
# define __YUNI_UI_LOCAL_WINDOW_WINGDI_H__

# include "../../../yuni.h"

# ifdef YUNI_WINDOWSYSTEM_MSW

#	include "../../../core/system/windows.hdr.h"
#	include "window.h"



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


	/*!
	** \brief Implementation of a window for Microsoft Windows GDI+
	*/
	class WinGDI: public IWindow
	{
	public:
		//! List of local GDI windows, by HWND
		typedef std::map<HWND, WinGDI*> WindowList;


	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		WinGDI()
		{}
		//! Destructor
		virtual ~WinGDI() {}
		//@}

		/*!
		** \brief Initialize the window
		**
		** \returns False if the initialization failed, true if it worked
		*/
		virtual bool initialize();

		/*!
		** \brief Move the window to a new position
		**
		** \param left New coordinate of the left of the window
		** \param top New coordinate of the top of the window
		*/
		virtual void move(float left, float top);

		/*!
		** \brief Move the window relatively to its old position
		**
		** \param left How much to add to the left of the window
		** \param top How much to add to the top of the window
		*/
		virtual void moveRelative(float left, float top);

		/*!
		** \brief Resize the window to new dimensions
		**
		** \param width New width of the window
		** \param height New height of the window
		*/
		virtual void resize(float width, float height);

		//! Show the window
		virtual void show();

		//! Hide the window
		virtual void hide();

		//! Minimize the window
		virtual void minimize();

		//! Maximize the window
		virtual void maximize();

		//! Restore the window (from minimization or maximization)
		virtual void restore();

		//! Bring the window to front
		virtual void bringToFront();

		//! Send the window to back
		virtual void sendToBack();

		/*!
		** \brief Poll events for this window
		**
		** \returns True to continue, false if a quit event was caught
		*/
		virtual bool pollEvents();


	protected:
		//! Do the actual modification of the caption, virtual, called from base
		virtual void doUpdateCaption();

		//! Do the actual modification of the style, virtual, called from base
		virtual void doUpdateStyle();

		//! Do the actual modification of the stay on top option, virtual, called from base
		virtual void doUpdateStayOnTop();

		//! Do the actual modification of the full screen option, virtual, called from base
		virtual void doUpdateFullScreen();

		//! Do the actual refresh of the window
		virtual void doRefresh();

		//! Do the actual refresh of a rectangle in the window
		virtual void doRefreshRect(float left, float top, float width, float height);


	protected:
		/*!
		** \brief Callback for window message handling
		**
		** \param handle Handle for this window
		** \param uMsg Message
		** \param wParam Additional Message Information
		** \param lParam Additional Message Information
		*/
		static LRESULT CALLBACK MessageCallback(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//! Find a window given its handle
		static WinGDI* FindWindow(HWND handle);

		//! Register a window with its handle as a key
		static void RegisterWindow(HWND handle, WinGDI* window);

		//! Unregister the window, happens when closing it
		static void UnregisterWindow(HWND handle);


	protected:
		//! Static list of registered GDI windows
		static WindowList sWindowList;

	protected:
		//! Instance handle
		HINSTANCE pHInstance;
		//! Window handle
		HWND pHWnd;

	}; // class WinGDI





} // namespace Window
} // namespace Local
} // namespace UI
} // namespace Private
} // namespace Yuni

# endif // YUNI_WINDOWSYSTEM_MSW

#endif // __YUNI_UI_LOCAL_WINDOW_WINGDI_H__
