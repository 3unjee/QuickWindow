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

#include "QuickWindow.h"

// Qt includes
#ifdef QT_4
#include <QCoreApplication>
#include <QMouseEvent>
#else
#include <QScreen>
#endif
#include <QTimer>
#include <QTime>
#include <QDir>

// Gui includes
#include <QuickView.h>

/*!
    \class QuickWindow

    \brief QML window.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

QuickWindow::QuickWindow(QuickItem * parent) : QuickItem(parent)
{
    _view = new QuickView(this);

#ifdef QT_4
    _ratio = 1.0;
#else
    QScreen * screen = q->screen();

    _ratio = screen->logicalDotsPerInch() / 96;
#endif

    _mouseX = -1;
    _mouseY = -1;

#ifdef QT_5
    connect(_view, SIGNAL(screenChanged(QScreen *)), this, SLOT(onGeometryChanged()));

    connect(screen, SIGNAL(availableGeometryChanged(QRect)), this, SLOT(onGeometryChanged()));
#endif
}

/* virtual */ QuickWindow::~QuickWindow()
{
    _view->deleteLater();
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

/*!
    Closes the window.
*/
/* Q_INVOKABLE */ void QuickWindow::close()
{
    QTimer::singleShot(0, _view, SLOT(close()));
}

/*!
    Centers the window.
*/
/* Q_INVOKABLE */ void QuickWindow::centerWindow()
{
    QRect geometry = _view->availableGeometry();

    _view->move(geometry.x() + (geometry.width () - _view->width ()) / 2,
                geometry.y() + (geometry.height() - _view->height()) / 2);
}

//-------------------------------------------------------------------------------------------------

/*!
    Saves a screenshot.
*/
/* Q_INVOKABLE */ bool QuickWindow::saveShot(const QString & fileName, int x,     int y,
                                                                       int width, int height) const
{
#ifdef QT_4
    QImage image = QPixmap::grabWidget(_view->viewport(), x, y, width, height).toImage();
#else
    if (width  == -1) width  = _view->width ();
    if (height == -1) height = _view->height();

    QImage image = _view->grabWindow().copy(x, y, width, height);
#endif

    return image.save(fileName, "png");
}

//-------------------------------------------------------------------------------------------------

/*!
    Compresses the screenshots.
*/
/* Q_INVOKABLE */ bool QuickWindow::compressShots(const QString & path, int quality) const
{
    QDir dir(path);

    if (dir.exists() == false) return false;

    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

    foreach (QFileInfo info, list)
    {
        if (info.suffix().toLower() == "png")
        {
            const QString & path = info.filePath();

            qDebug("Compressing: %s", qPrintable(path));

            QImage image(path);

            if (image.save(path, "png", quality) == false)
            {
                qWarning("QuickWindow::compressShots: Failed to save image.");
            }
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
// Protected QuickItem reimplementation
//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickWindow::componentComplete()
{
    QuickItem::componentComplete();

    centerWindow();

    _view->show();
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void QuickWindow::setRatio(qreal ratio)
{
    if (_ratio == ratio) return;

    _ratio = ratio;

    emit ratioChanged();
}

//-------------------------------------------------------------------------------------------------

void QuickWindow::setMouseX(int x)
{
    if (_mouseX == x) return;

    _mouseX = x;

    emit mouseXChanged();
}

void QuickWindow::setMouseY(int y)
{
    if (_mouseY == y) return;

    _mouseY = y;

    emit mouseXChanged();
}

//-------------------------------------------------------------------------------------------------
// Private slots
//-------------------------------------------------------------------------------------------------

void QuickWindow::onGeometryChanged()
{
#ifdef QT_5
    setRatio(_view->screen()->logicalDotsPerInch() / 96);
#endif
}

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

QuickView * QuickWindow::view() const
{
    return _view;
}

//-------------------------------------------------------------------------------------------------

void QuickWindow::setX(int x)
{
    _view->move(x, _view->y());

    emit xChanged();
}

void QuickWindow::setY(int y)
{
    _view->move(_view->x(), y);

    emit yChanged();
}

//-------------------------------------------------------------------------------------------------

void QuickWindow::setWidth(int width)
{
    _view->resize(width, _view->height());

    emit widthChanged();
}

void QuickWindow::setHeight(int height)
{
    _view->resize(_view->width(), height);

    emit heightChanged();
}

//-------------------------------------------------------------------------------------------------

qreal QuickWindow::ratio() const
{
    return _ratio;
}

//-------------------------------------------------------------------------------------------------

int QuickWindow::minimumWidth() const
{
    return _view->minimumWidth();
}

void QuickWindow::setMinimumWidth(int width)
{
#ifdef QT_4
    _view->setMinimumSize(width, _view->minimumHeight());
#else
    _view->setMinimumWidth(width);
#endif

    emit minimumWidthChanged();
}

int QuickWindow::minimumHeight() const
{
    return _view->minimumHeight();
}

void QuickWindow::setMinimumHeight(int height)
{
#ifdef QT_4
    _view->setMinimumSize(_view->minimumWidth(), height);
#else
    _view->setMinimumHeight(height);
#endif

    emit minimumHeightChanged();
}

//-------------------------------------------------------------------------------------------------

int QuickWindow::maximumWidth() const
{
    return _view->maximumWidth();
}

void QuickWindow::setMaximumWidth(int width)
{
#ifdef QT_4
    _view->setMaximumSize(width, _view->maximumHeight());
#else
    _view->setMaximumWidth(width);
#endif

    emit maximumWidthChanged();
}

int QuickWindow::maximumHeight() const
{
    return _view->maximumHeight();
}

void QuickWindow::setMaximumHeight(int height)
{
#ifdef QT_4
    _view->setMaximumSize(_view->maximumWidth(), height);
#else
    _view->setMaximumHeight(height);
#endif

    emit maximumHeightChanged();
}

//-------------------------------------------------------------------------------------------------

int QuickWindow::mouseX() const
{
    return _mouseX;
}

int QuickWindow::mouseY() const
{
    return _mouseY;
}

//-------------------------------------------------------------------------------------------------

bool QuickWindow::isMaximized() const
{
    return _view->isMaximized();
}

void QuickWindow::setMaximized(bool maximized)
{
    _view->setMaximized(maximized);

    emit maximizedChanged();
}

//-------------------------------------------------------------------------------------------------

bool QuickWindow::isFullScreen() const
{
    return _view->isFullScreen();
}

void QuickWindow::setFullScreen(bool fullScreen)
{
    _view->setFullScreen(fullScreen);

    emit fullScreenChanged();
}

//-------------------------------------------------------------------------------------------------

bool QuickWindow::isLocked() const
{
    return _view->isLocked();
}

void QuickWindow::setLocked(bool locked)
{
    _view->setLocked(locked);

    emit lockedChanged();
}
