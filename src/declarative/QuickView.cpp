//=================================================================================================
/*
    Copyright (C) 2019 Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    - GNU General Public License Usage:
    This file may be used under the terms of the GNU General Public License version 3 as published
    by the Free Software Foundation and appearing in the LICENSE.md file included in the packaging
    of this file. Please review the following information to ensure the GNU General Public License
    requirements will be met: https://www.gnu.org/licenses/gpl.html.
*/
//=================================================================================================

#include "QuickView.h"

// Qt includes
#ifdef QT_4
#include <QApplication>
#include <QDesktopWidget>
#include <QFocusEvent>
#ifdef Q_OPENGL
#include <QGLWidget>
#endif
#else
#include <QGuiApplication>
#include <QScreen>
#endif

// Gui includes
#ifdef QT_4
#include <GraphicsScene.h>
#endif
#include <QuickWindow.h>

// Windows includes
#ifdef Q_WIN_BORDERLESS
#ifdef QT_4
#include <qt_windows.h>
#else
#include <QtWinExtras>
#endif
#endif

/*!
    \class QuickView

    \brief Base QML window.
*/

//-------------------------------------------------------------------------------------------------
// Static variables

#ifdef Q_WIN_BORDERLESS
static const DWORD QUICKVIEW_FLAGS = WS_OVERLAPPED  | WS_THICKFRAME | WS_MINIMIZEBOX |
                                     WS_MAXIMIZEBOX | WS_CLIPCHILDREN;

#ifdef QT_5
static const int QUICKVIEW_INTERVAL = 400;

static const int QUICKVIEW_DELAY = 5000;
#endif
#endif

#ifdef QT_5
//-------------------------------------------------------------------------------------------------
// Global variables

QWindow * view = NULL;

int count = 0;
#endif

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* explicit */ QuickView::QuickView(QuickWindow * window) : QDeclarativeView()
#else
/* explicit */ QuickView::QuickView(QuickWindow * window) : QQuickWindow()
#endif
{
    _window = window;

    _maximized  = false;
    _fullScreen = false;
    _locked     = false;

#ifdef QT_4
#ifdef Q_OPENGL
    QGLFormat format = QGLFormat::defaultFormat();

    format.setSampleBuffers(true);

    setViewport(new QGLWidget(format));
#endif

    GraphicsScene * scene = new GraphicsScene(this);

    setScene(scene);

    scene->addItem(window);
#else
    QSurfaceFormat format;

#ifdef Q_OPENGL
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setVersion       (3, 3);
#endif
    format.setSwapBehavior   (QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);

    setFormat(format);

    window->setParentItem(contentItem());
#endif

#ifdef Q_WIN_BORDERLESS
#ifdef QT_4
    setWindowFlags(Qt::FramelessWindowHint);

    _id = QDeclarativeView::winId();
#else
    setFlags(Qt::Window | Qt::FramelessWindowHint);

    WId id = QQuickWindow::winId();

    _id = (HWND) id;

    _viewport = fromWinId(id);
#endif

    _x = 0;
    _y = 0;

    _width  = 0;
    _height = 0;

    _minimumWidth  = 0;
    _minimumHeight = 0;

    _maximumWidth  = QWIDGETSIZE_MAX;
    _maximumHeight = QWIDGETSIZE_MAX;

    _visible = false;
    _opacity = 1.0;

    const QMetaObject * meta = metaObject();

    _method = meta->method(meta->indexOfMethod("onFocus()"));

#ifdef QT_5
    _timer.setInterval(QUICKVIEW_INTERVAL);

    _timer.setSingleShot(true);

    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onMove()));

    _touchId   = -1;
    _touchItem = NULL;

    _touchTimer.setSingleShot(true);
#endif

    //---------------------------------------------------------------------------------------------

    WNDCLASSEX wcx;

    wcx.cbSize = sizeof(WNDCLASSEX);

    wcx.style = CS_HREDRAW | CS_VREDRAW;

    wcx.lpfnWndProc = events;

    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;

    wcx.hInstance = 0;

    wcx.hCursor = 0;

    wcx.hbrBackground = 0;

    wcx.lpszMenuName  = 0;
    wcx.lpszClassName = L"Window";

    HINSTANCE application = GetModuleHandle(0);

    wcx.hIcon = (HICON) LoadImage(application, L"IDI_ICON1", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);

    if (wcx.hIcon)
    {
        int width  = GetSystemMetrics(SM_CXSMICON);
        int height = GetSystemMetrics(SM_CYSMICON);

        wcx.hIconSm = (HICON) LoadImage(application, L"IDI_ICON1", IMAGE_ICON, width, height, 0);
    }
    else wcx.hIconSm = 0;

    RegisterClassEx(&wcx);

    _handle = CreateWindow(L"Window", 0, QUICKVIEW_FLAGS, 0, 0, 0, 0, 0, 0, 0, NULL);

    SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR> (this));

#ifdef QT_4
    SetWindowLong(_id, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN);
#else
    SetWindowLong(_id, GWL_STYLE, WS_CHILD);

    setProperty("_q_embedded_native_parent_handle", (WId) _handle);
#endif

    SetParent(_id, _handle);

#ifdef QT_5
    // FIXME Qt5 Windows: We need to create a QWindow to receive QScreen events.
    if (count == 0)
    {
        QTimer::singleShot(QUICKVIEW_DELAY, this, SLOT(onCreate()));
    }

    count++;
#endif
#else
#ifdef QT_4
    setWindowFlags(Qt::FramelessWindowHint);
#else
    setFlags(Qt::Window | Qt::FramelessWindowHint);
#endif
#endif
}

#ifdef Q_WIN_BORDERLESS
/* virtual */ QuickView::~QuickView()
{
    DestroyWindow(_handle);

#ifdef QT_5
    count--;

    if (count == 0 && view)
    {
        delete view;

        view = NULL;
    }
#endif
}
#endif

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

/*!
    Moves the view.
*/
/* Q_INVOKABLE */ void QuickView::move(int x, int y)
{
#ifdef Q_WIN_BORDERLESS
    SetWindowPos(_handle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
#elif defined(QT_4)
    QDeclarativeView::move(x, y);
#else
    QQuickWindow::setPosition(x, y);
#endif
}

/*!
    This is an overloaded function.

    Moves the view.
*/
/* Q_INVOKABLE */ void QuickView::move(const QPoint & position)
{
    move(position.x(), position.y());
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the screen full geometry.
*/
/* Q_INVOKABLE */ QRect QuickView::screenGeometry() const
{
#ifdef QT_4
    return qApp->desktop()->screenGeometry(this);
#else
    return screen()->geometry();
#endif
}

/*!
    Returns the screen available geometry.
*/
/* Q_INVOKABLE */ QRect QuickView::availableGeometry() const
{
#ifdef QT_4
    return qApp->desktop()->availableGeometry(this);
#else
    return screen()->availableGeometry();
#endif
}

//-------------------------------------------------------------------------------------------------

#ifdef Q_WIN_BORDERLESS

/*!
    Shows the view
*/
/* Q_INVOKABLE */ void QuickView::show()
{
    setVisible(true);
}

/*!
    Hides the view
*/
/* Q_INVOKABLE */ void QuickView::hide()
{
    setVisible(false);
}

//-------------------------------------------------------------------------------------------------

/*!
    Restores the view to a normal state
*/
/* Q_INVOKABLE */ void QuickView::showNormal()
{
    if (_maximized)
    {
        setMaximized(false);
    }
    else if (_fullScreen)
    {
        setFullScreen(false);
    }
}

/*!
    Shows the view maximized.
*/
/* Q_INVOKABLE */ void QuickView::showMaximized()
{
    setMaximized(true);
}

/*!
    Shows the view full-screen.
*/
/* Q_INVOKABLE */ void QuickView::showFullScreen()
{
    setFullScreen(true);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void QuickView::showMinimized()
{
    ShowWindow(_handle, SW_SHOWMINIMIZED);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void QuickView::raise()
{
    SetWindowPos(_handle, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
}

/* Q_INVOKABLE */ void QuickView::lower()
{
    SetWindowPos(_handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
}

//-------------------------------------------------------------------------------------------------

/*!
    Closes the view.
*/
/* Q_INVOKABLE */ bool QuickView::close()
{
    ShowWindow(_handle, SW_HIDE);

#ifdef QT_4
    return QDeclarativeView::close();
#else
    return QQuickWindow::close();
#endif
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_5

/*!
    Moves the view.
*/
/* Q_INVOKABLE */ void QuickView::setPosition(int x, int y)
{
    SetWindowPos(_handle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

/*!
    This is an overloaded function.

    Moves the view.
*/
/* Q_INVOKABLE */ void QuickView::setPosition(const QPoint & position)
{
    setPosition(position.x(), position.y());
}

#endif

//-------------------------------------------------------------------------------------------------

/*!
    Resizes the view.
*/
/* Q_INVOKABLE */ void QuickView::resize(int width, int height)
{
    SetWindowPos(_handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
}

/*!
    This is an overloaded function.

    Resizes the view.
*/
/* Q_INVOKABLE */ void QuickView::resize(const QSize & size)
{
    resize(size.width(), size.height());
}

//-------------------------------------------------------------------------------------------------

/*!
    Changes the view geometry.
*/
/* Q_INVOKABLE */ void QuickView::setGeometry(int x, int y, int width, int height)
{
    SetWindowPos(_handle, HWND_TOP, x, y, width, height, 0);
}

/*!
    This is an overloaded function.

    Changes the view geometry.
*/
/* Q_INVOKABLE */ void QuickView::setGeometry(const QRect & rect)
{
    setGeometry(rect.x(), rect.y(), rect.width(), rect.height());
}

//-------------------------------------------------------------------------------------------------

/*!
    Changes the minimum size.
*/
/* Q_INVOKABLE */ void QuickView::setMinimumSize(int width, int height)
{
    _minimumWidth  = width;
    _minimumHeight = height;

    if (_width < width)
    {
        if (_height < height)
        {
             SetWindowPos(_handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
        }
        else SetWindowPos(_handle, HWND_TOP, 0, 0, width, _height, SWP_NOMOVE);
    }
    else if (_height < height)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, _width, height, SWP_NOMOVE);
    }
}

/*!
    Changes the maximum size.
*/
/* Q_INVOKABLE */ void QuickView::setMaximumSize(int width, int height)
{
    _maximumWidth  = width;
    _maximumHeight = height;

    if (_width > width)
    {
        if (_height > height)
        {
             SetWindowPos(_handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
        }
        else SetWindowPos(_handle, HWND_TOP, 0, 0, width, _height, SWP_NOMOVE);
    }
    else if (_height > height)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, _width, height, SWP_NOMOVE);
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    This is an overloaded function.

    Changes the minimum size.
*/
/* Q_INVOKABLE */ void QuickView::setMinimumSize(const QSize & size)
{
    setMinimumSize(size.width(), size.height());
}

/*!
    This is an overloaded function.

    Changes the maximum size.
*/
/* Q_INVOKABLE */ void QuickView::setMaximumSize(const QSize & size)
{
    setMaximumSize(size.width(), size.height());
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void QuickView::setMinimumWidth(int width)
{
    _minimumWidth = width;

    if (_width < width)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, width, _height, SWP_NOMOVE);
    }
}

/* Q_INVOKABLE */ void QuickView::setMinimumHeight(int height)
{
    _minimumHeight = height;

    if (_height < height)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, _width, height, SWP_NOMOVE);
    }
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void QuickView::setMaximumWidth(int width)
{
    _maximumWidth = width;

    if (_width > width)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, width, _height, SWP_NOMOVE);
    }
}

/* Q_INVOKABLE */ void QuickView::setMaximumHeight(int height)
{
    _maximumHeight = height;

    if (_height > height)
    {
        SetWindowPos(_handle, HWND_TOP, 0, 0, _width, height, SWP_NOMOVE);
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Changes the view visibility.
*/
/* Q_INVOKABLE */ void QuickView::setVisible(bool visible)
{
    if (_visible == visible) return;

    _visible = visible;

#ifdef QT_4
    QDeclarativeView::setVisible(visible);
#else
    QQuickWindow::setVisible(visible);
#endif

    if (visible)
    {
         ShowWindow(_handle, SW_SHOW);
    }
    else ShowWindow(_handle, SW_HIDE);
}

//-------------------------------------------------------------------------------------------------

/*!
    Focuses the view.
*/
/* Q_INVOKABLE */ void QuickView::setFocus()
{
    SetFocus(_id);
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* Q_INVOKABLE */ void QuickView::setWindowIcon(const QIcon & icon)
#else
/* Q_INVOKABLE */ void QuickView::setIcon(const QIcon & icon)
#endif
{
    HICON iconSmall = getIcon(icon, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
    HICON iconBig   = getIcon(icon, GetSystemMetrics(SM_CXICON),   GetSystemMetrics(SM_CXICON));

    if (iconBig)
    {
        PostMessage(_handle, WM_SETICON, ICON_SMALL, (LPARAM) iconSmall);
        PostMessage(_handle, WM_SETICON, ICON_BIG,   (LPARAM) iconBig);
    }
    else
    {
        PostMessage(_handle, WM_SETICON, ICON_SMALL, (LPARAM) iconSmall);
        PostMessage(_handle, WM_SETICON, ICON_BIG,   (LPARAM) iconSmall);
    }
}

#ifdef QT_4
/* Q_INVOKABLE */ void QuickView::setWindowTitle(const QString & title)
#else
/* Q_INVOKABLE */ void QuickView::setTitle(const QString & title)
#endif
{
    SetWindowText(_handle, (wchar_t *) title.utf16());
}

#endif // Q_WIN_BORDERLESS

//-------------------------------------------------------------------------------------------------
// Events
//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickView::resizeEvent(QResizeEvent * event)
{
#ifdef QT_4
    QDeclarativeView::resizeEvent(event);
#else
    QQuickWindow::resizeEvent(event);
#endif

    _window->setSize(event->size());
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickView::mouseMoveEvent(QMouseEvent * event)
{
#ifdef QT_4
    QDeclarativeView::mouseMoveEvent(event);
#else
    QQuickWindow::mouseMoveEvent(event);
#endif

    QPoint position = event->pos();

    _window->setMouseX(position.x());
    _window->setMouseY(position.y());
}

//-------------------------------------------------------------------------------------------------

#ifdef Q_WIN_BORDERLESS

#ifdef QT_4
/* virtual */ bool QuickView::winEvent(MSG * msg, long * result)
#else
/* virtual */ bool QuickView::nativeEvent(const QByteArray & event, void * msg, long * result)
#endif
{
#ifdef QT_4
    UINT message = msg->message;
#else
    UINT message = static_cast<MSG *> (msg)->message;
#endif

    if (message == WM_SETFOCUS)
    {
        QFocusEvent event(QEvent::FocusIn);

        QCoreApplication::sendEvent(this, &event);
    }
    else if (message == WM_KILLFOCUS)
    {
        QFocusEvent event(QEvent::FocusOut);

        QCoreApplication::sendEvent(this, &event);
    }

#ifdef QT_4
    return QDeclarativeView::winEvent(msg, result);
#else
    return QQuickWindow::nativeEvent(event, msg, result);
#endif
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void QuickView::updateState(Qt::WindowState state)
{
    if (state == Qt::WindowMaximized)
    {
        _maximized = true;

        emit _window->maximizedChanged();
    }
    else if (state == Qt::WindowNoState)
    {
        _maximized = false;

        emit _window->maximizedChanged();
    }
}

#ifdef Q_WIN_BORDERLESS

//-------------------------------------------------------------------------------------------------
// Private static functions
//-------------------------------------------------------------------------------------------------

/* static */ HICON QuickView::getIcon(const QIcon & icon, int width, int height)
{
    QSize size = icon.actualSize(QSize(width, height));

    QPixmap pixmap = icon.pixmap(size);

    if (pixmap.isNull())
    {
        return NULL;
    }
#ifdef QT_4
    else return pixmap.toWinHICON();
#else
    else return QtWin::toHICON(pixmap);
#endif
}

//-------------------------------------------------------------------------------------------------
// Private static events
//-------------------------------------------------------------------------------------------------

/* static */ LRESULT CALLBACK QuickView::events(HWND handle, UINT message, WPARAM wParam,
                                                                           LPARAM lParam)
{
    if (message == WM_ACTIVATE)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        if (view->_fullScreen)
        {
            view->setGeometry(view->screenGeometry());
        }

        return 0;
    }
    else if (message == WM_SETFOCUS)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        // FIXME: Sometimes the focus fails.
        if (SetFocus(view->_id) == NULL)
        {
            view->_method.invoke(view, Qt::QueuedConnection);
        }

        return 0;
    }
    else if (message == WM_NCCALCSIZE)
    {
        return 0;
    }
    else if (message == WM_GETMINMAXINFO)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        if (view == NULL) return 0;

        MINMAXINFO * info = (MINMAXINFO *) lParam;

        info->ptMinTrackSize.x = view->_minimumWidth;
        info->ptMinTrackSize.y = view->_minimumHeight;

        info->ptMaxTrackSize.x = view->_maximumWidth;
        info->ptMaxTrackSize.y = view->_maximumHeight;

        return 0;
    }
    else if (message == WM_MOVE)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        if (view == NULL) return 0;

        RECT rect;

        GetWindowRect(handle, &rect);

        view->_x = rect.left;
        view->_y = rect.top;

#ifdef QT_4
        view->QDeclarativeView::move(0, 0);
#else
        view->_timer.start();
#endif

        return 0;
    }
    else if (message == WM_SIZE)
    {
        WINDOWPLACEMENT placement;

        placement.length = sizeof(WINDOWPLACEMENT);

        GetWindowPlacement(handle, &placement);

        if (placement.showCmd == SW_SHOWMINIMIZED) return 0;

        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        if (view == NULL) return 0;

        RECT rect;

        GetClientRect(handle, &rect);

        if (placement.showCmd == SW_MAXIMIZE)
        {
            RECT adjust = { 0, 0, 0, 0 };

            UINT style = GetWindowLong(handle, GWL_STYLE);

            AdjustWindowRectEx(&adjust, style, FALSE, 0);

            int border   = adjust.right;
            int border2x = border * 2;

            int width  = rect.right  - border2x;
            int height = rect.bottom - border2x;

            view->_width  = width;
            view->_height = height;

#ifdef QT_4
            view->QDeclarativeView::setGeometry(border, border, width, height);
#elif defined(Q_OS_WIN)
            // FIXME Qt5 Windows: Workaround for opengl full screen flicker.
            view->_viewport->setGeometry(border, border, width + 1, height);
#else
            view->_viewport->setGeometry(border, border, width, height);
#endif

            view->updateState(Qt::WindowMaximized);
        }
        else
        {
            int width  = rect.right;
            int height = rect.bottom;

            view->_width  = width;
            view->_height = height;

#ifdef QT_4
            view->QDeclarativeView::setGeometry(0, 0, width, height);
#elif defined(Q_OS_WIN)
            // FIXME Qt5 Windows: Workaround for opengl full screen flicker.
            if (view->_fullScreen)
            {
                 view->_viewport->setGeometry(0, 0, width + 1, height);
            }
            else view->_viewport->setGeometry(0, 0, width, height);
#else
            view->_viewport->setGeometry(0, 0, width, height);
#endif

            view->updateState(Qt::WindowNoState);
        }

        return 0;
    }
#ifdef QT_5
    // FIXME Qt 5.12: We have to send mouse release manually when dragging and resizing.
    else if (message == WM_CAPTURECHANGED)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        QPoint pos = QCursor::pos();

        QMouseEvent event(QEvent::MouseButtonRelease, view->mapFromGlobal(pos), pos,
                          Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

        QCoreApplication::sendEvent(view, &event);

        return 0;
    }
#endif
    else if (message == WM_CLOSE)
    {
        QuickView * view = reinterpret_cast<QuickView *> (GetWindowLongPtr(handle, GWLP_USERDATA));

        view->close();

        return 0;
    }
    else return DefWindowProc(handle, message, wParam, lParam);
}

#endif

//-------------------------------------------------------------------------------------------------
// Private slots
//-------------------------------------------------------------------------------------------------

#ifdef QT_5

// FIXME Qt5 Windows: We need to create a QWindow to receive QScreen events.
void QuickView::onCreate()
{
    view = new QWindow;

    view->setFlags(Qt::Tool | Qt::FramelessWindowHint);

    view->resize(0, 0);

    view->show();
    view->hide();
}

void QuickView::onMove()
{
    if (_maximized || _fullScreen) return;

    QSize size = _viewport->size();

    _viewport->resize(size.width() + 1, size.height());
    _viewport->resize(size);
}

#endif

void QuickView::onFocus()
{
    SetFocus(_id);
}

#endif // Q_WIN_BORDERLESS

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

QuickWindow * QuickView::window() const
{
    return _window;
}

//-------------------------------------------------------------------------------------------------

bool QuickView::isMaximized() const
{
    return _maximized;
}

void QuickView::setMaximized(bool maximized)
{
    if (_maximized == maximized) return;

    _maximized = maximized;

    if (maximized)
    {
#if defined(Q_WIN_BORDERLESS)
        ShowWindow(_handle, SW_MAXIMIZE);
#elif defined(QT_4)
        QDeclarativeView::showMaximized();
#else
        QQuickWindow::showMaximized();
#endif
    }
#if defined(Q_WIN_BORDERLESS)
    else ShowWindow(_handle, SW_RESTORE);
#elif defined(QT_4)
    else QDeclarativeView::showNormal();
#else
    else QQuickWindow::showNormal();
#endif
}

//-------------------------------------------------------------------------------------------------

bool QuickView::isFullScreen() const
{
    return _fullScreen;
}

void QuickView::setFullScreen(bool fullScreen)
{
    if (_fullScreen == fullScreen) return;

    _fullScreen = fullScreen;

#ifdef Q_WIN_BORDERLESS
    if (fullScreen)
    {
        setMaximized(false);

        _rect = QRect(x(), y(), width(), height());

        setGeometry(screenGeometry());
    }
    else setGeometry(_rect);
#else
    if (fullScreen)
    {
#ifdef QT_4
        QDeclarativeView::showFullScreen();
#else
        QQuickWindow::showFullScreen();
#endif
    }
#ifdef QT_4
    else QDeclarativeView::showNormal();
#else
    else QQuickWindow::showNormal();
#endif
#endif
}

//-------------------------------------------------------------------------------------------------

bool QuickView::isLocked() const
{
    return _locked;
}

void QuickView::setLocked(bool locked)
{
    if (_locked == locked) return;

    _locked = locked;

#ifdef Q_OS_WIN
    if (locked)
    {
         SetWindowPos((HWND) winId(),
                      HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else SetWindowPos((HWND) winId(),
                      HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#elif defined(QT_4)
    if (locked)
    {
         setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint
                                                | Qt::WindowStaysOnTopHint);
    }
    else setWindowFlags(Qt::FramelessWindowHint);
#else
    if (locked)
    {
         setFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    }
    else setFlags(Qt::FramelessWindowHint);
#endif
}

//-------------------------------------------------------------------------------------------------

#ifdef Q_WIN_BORDERLESS

WId QuickView::winId() const
{
    return (WId) _handle;
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_5
QScreen * QuickView::screen() const
{
    return fromWinId((WId) _id)->screen();
}
#endif // QT_5

//-------------------------------------------------------------------------------------------------

QRect QuickView::geometry() const
{
    return QRect(_x, _y, _width, _height);
}

//-------------------------------------------------------------------------------------------------

int QuickView::x() const
{
    return _x;
}

int QuickView::y() const
{
    return _y;
}

//-------------------------------------------------------------------------------------------------

int QuickView::width() const
{
    return _width;
}

int QuickView::height() const
{
    return _height;
}

//-------------------------------------------------------------------------------------------------

int QuickView::minimumWidth() const
{
    return _minimumWidth;
}

int QuickView::minimumHeight() const
{
    return _minimumHeight;
}

//-------------------------------------------------------------------------------------------------

int QuickView::maximumWidth() const
{
    return _maximumWidth;
}

int QuickView::maximumHeight() const
{
    return _maximumHeight;
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_4
qreal QuickView::windowOpacity() const
#else
qreal QuickView::opacity() const
#endif
{
    return _opacity;
}

#ifdef QT_4
void QuickView::setWindowOpacity(qreal level)
#else
void QuickView::setOpacity(qreal level)
#endif
{
    qreal oldOpacity = _opacity;

    _opacity = level;

    if (level == 1.0)
    {
        SetLayeredWindowAttributes(_handle, 0, 255, LWA_ALPHA);

        SetWindowLong(_handle,
                      GWL_EXSTYLE, GetWindowLong(_handle, GWL_EXSTYLE) & ~WS_EX_LAYERED);
    }
    else
    {
        if (oldOpacity == 1.0)
        {
            SetWindowLong(_handle,
                          GWL_EXSTYLE, GetWindowLong(_handle, GWL_EXSTYLE) | WS_EX_LAYERED);
        }

        SetLayeredWindowAttributes(_handle, 0, level * 255, LWA_ALPHA);
    }
}

#endif // Q_WIN_BORDERLESS
