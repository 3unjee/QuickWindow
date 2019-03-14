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

import QtQuick 1.0
import gui     1.0

Window
{
    //---------------------------------------------------------------------------------------------
    // Properties
    //---------------------------------------------------------------------------------------------

    property int dp16: dp(16)
    property int dp24: dp(24)
    property int dp64: dp(64)
    property int dp96: dp(96)

    property int dp320: dp(320)
    property int dp200: dp(200)

    property int dp640: dp(640)
    property int dp480: dp(480)

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    width : dp640
    height: dp480

    minimumWidth : dp320
    minimumHeight: dp200

    //---------------------------------------------------------------------------------------------
    // Events
    //---------------------------------------------------------------------------------------------

    Component.onCompleted: forceActiveFocus()

    //---------------------------------------------------------------------------------------------
    // Keys
    //---------------------------------------------------------------------------------------------

    Keys.onPressed:
    {
        if (event.key == Qt.Key_Escape)
        {
            event.accepted = true;

            close();
        }
        else if (event.key == Qt.Key_F12)
        {
            event.accepted = true;

            takeShot();
        }
    }

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------

    function dp(value)
    {
        return value * ratio;
    }

    function takeShot()
    {
        console.debug("Saving shot: QuickWindow.png");

        saveShot("../QuickWindow.png");

        compressShots(".");
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Rectangle
    {
        anchors.fill: parent

        color: "#323232"
    }

    Text
    {
        id: itemText

        anchors.fill: parent

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment  : Text.AlignVCenter

        text: "Hello World"

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: dp96
    }

    Text
    {
        anchors.right : parent.right
        anchors.bottom: parent.bottom

        anchors.margins: dp16

        text: "Qt " + core.version

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: dp24
    }

    MouseArea
    {
        anchors.fill: parent

        onClicked:
        {
            if (itemText.text == "Hello World")
            {
                 itemText.text = "Click !";
            }
            else itemText.text = "Hello World";
        }
    }

    Rectangle
    {
        anchors.fill: parent

        color: "transparent"

        border.width: dp16
        border.color: "#161616"
    }

    ItemDrag
    {
        anchors.left : parent.left
        anchors.right: parent.right

        height: dp64
    }

    ItemResizer
    {
        anchors.fill: parent

        visible: (maximized == false && fullScreen == false)
    }
}
