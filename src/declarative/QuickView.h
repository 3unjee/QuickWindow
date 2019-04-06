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

#ifndef QUICKVIEW_H
#define QUICKVIEW_H

// Qt includes
#ifdef QT_4
#include <QDeclarativeView>
#else
#include <QQuickWindow>
#include <QTimer>
#endif
#ifdef Q_OS_WIN
#include <QMetaMethod>
#endif

// Windows includes
#if defined(QT_4) && defined(Q_OS_WIN)
#include <windows.h>
#endif

// Forward declarations
class QuickWindow;
#ifdef QT_5
class QuickMouseArea;
#endif

#ifdef QT_4
class QuickView : public QDeclarativeView
#else
class QuickView : public QQuickWindow
#endif
{
    Q_OBJECT

public:
    explicit QuickView(QuickWindow * window);

#ifdef Q_WIN_BORDERLESS
    /* virtual */ ~QuickView();
#endif

public: // Interface
    Q_INVOKABLE void move(int x, int y);
    Q_INVOKABLE void move(const QPoint & position);

    Q_INVOKABLE QRect screenGeometry   () const;
    Q_INVOKABLE QRect availableGeometry() const;

#ifdef Q_WIN_BORDERLESS
    Q_INVOKABLE void show();
    Q_INVOKABLE void hide();

    Q_INVOKABLE void showNormal    ();
    Q_INVOKABLE void showMaximized ();
    Q_INVOKABLE void showFullScreen();

    Q_INVOKABLE void showMinimized();

    Q_INVOKABLE void raise();
    Q_INVOKABLE void lower();

    Q_INVOKABLE bool close();

#ifdef QT_5
    Q_INVOKABLE void setPosition(int x, int y);
    Q_INVOKABLE void setPosition(const QPoint & position);
#endif

    Q_INVOKABLE void resize(int width, int height);
    Q_INVOKABLE void resize(const QSize & size);

    Q_INVOKABLE void setGeometry(int x, int y, int width, int height);
    Q_INVOKABLE void setGeometry(const QRect & rect);

    Q_INVOKABLE void setMinimumSize(int width, int height);
    Q_INVOKABLE void setMaximumSize(int width, int height);

    Q_INVOKABLE void setMinimumSize(const QSize & size);
    Q_INVOKABLE void setMaximumSize(const QSize & size);

    Q_INVOKABLE void setMinimumWidth (int width);
    Q_INVOKABLE void setMinimumHeight(int height);

    Q_INVOKABLE void setMaximumWidth (int width);
    Q_INVOKABLE void setMaximumHeight(int height);

    Q_INVOKABLE void setVisible(bool visible);

    Q_INVOKABLE void setFocus();

#ifdef QT_4
    Q_INVOKABLE void setWindowIcon (const QIcon   & icon);
    Q_INVOKABLE void setWindowTitle(const QString & title);
#else
    Q_INVOKABLE void setIcon (const QIcon   & icon);
    Q_INVOKABLE void setTitle(const QString & title);
#endif
#endif

protected: // Events
    /* virtual */ void resizeEvent(QResizeEvent * event);

    /* virtual */ void mouseMoveEvent(QMouseEvent * event);

#ifdef Q_WIN_BORDERLESS
#ifdef QT_4
    /* virtual */ bool winEvent(MSG * msg, long * result);
#else
    /* virtual */ bool nativeEvent(const QByteArray & event, void * msg, long * result);
#endif

private: // Functions
    void updateState(Qt::WindowState state);

private: // Static functions
    static HICON getIcon(const QIcon & icon, int width, int height);

private: // Windows events
    static LRESULT CALLBACK events(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private slots:
#ifdef QT_5
    void onCreate();

    void onMove();
#endif

    void onFocus();
#endif

public: // Properties
    QuickWindow * window() const;

    bool isMaximized() const;
    void setMaximized(bool maximized);

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    bool isLocked() const;
    void setLocked(bool locked);

#ifdef Q_WIN_BORDERLESS
    WId winId() const;

#ifdef QT_5
    QScreen * screen() const;
#endif

    QRect geometry() const;

    int x() const;
    int y() const;

    int width () const;
    int height() const;

    int minimumWidth () const;
    int minimumHeight() const;

    int maximumWidth () const;
    int maximumHeight() const;

#ifdef QT_4
    qreal windowOpacity() const;
    void  setWindowOpacity(qreal level);
#else
    qreal opacity() const;
    void  setOpacity(qreal level);
#endif
#endif

private: // Variables
    QuickWindow * _window;

    bool _maximized;
    bool _fullScreen;
    bool _locked;

#ifdef Q_WIN_BORDERLESS
    HWND _id;
    HWND _handle;

#ifdef QT_5
    QWindow * _viewport;
#endif

    int _x;
    int _y;

    int _width;
    int _height;

    int _minimumWidth;
    int _minimumHeight;

    int _maximumWidth;
    int _maximumHeight;

    QRect _rect;

    bool  _visible;
    qreal _opacity;

    QMetaMethod _method;

#ifdef QT_5
    QTimer _timer;

    int _touchId;

    QuickMouseArea * _touchItem;
    QTimer           _touchTimer;
#endif
#endif

private:
    friend class QuickItem;
    friend class QuickMouseArea;
};

#endif // QUICKVIEW_H
