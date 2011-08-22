/* Copyright (C) 2011 Instituto Nokia de Tecnologia

   Author: Sheldon Almeida Demario <sheldon.demario@openbossa.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

import Qt 4.7

Rectangle {
	id: monitorView
	width: 480; height: 800;
	signal foo(string bla, int ble)

	color: "#3F303A"

	Image {
		id: header
		source: "images/header.png"

		Text {
			id: labelDevices
			text: "Devices:"
			color: "#768EA5"
			font.pixelSize: 32

			anchors.fill: parent.fill
			anchors.verticalCenter: parent.verticalCenter
		}
	}


	Image {
		id: list_bg
		source: "images/list_bg.png"

		anchors.top: header.bottom
	}

	ListView {
		id: listView
		objectName: "listView"
		currentIndex: -1

		anchors.fill: list_bg

		model: monitor.devicesName()
		delegate: DeviceListItem {}
		clip: true
		highlight: Rectangle { color: "#02118A"; radius: 5 }
	}

	Rectangle {
		id: contentView
		width: parent.width; height: 400
		anchors.top: list_bg.bottom

		enabled: false

		Image {
			id: signal_bg
			source: "images/content_bg_signal.png"

			anchors.top: parent.top
		}

		Text {
			id: labelSignalLevel
			text: "Signal Level:"

			color: "#768EA5"
			font.pixelSize: 32

			anchors.top: signal_bg.top
			anchors.right: signal_bg.horizontalCenter
			anchors.topMargin: 50
			anchors.leftMargin: 10
			anchors.rightMargin: 5
		}

		Text {
			id: signalLevel
			objectName: "signalLevel"

			text: "unknown"

			color: "#60748E"
			font.pixelSize: 32
			font.weight: Font.Bold

			anchors.top: labelSignalLevel.top
			anchors.left: labelSignalLevel.right
			anchors.leftMargin: 5
		}

		Image {
			id: immediate_bg
			source: "images/content_bg_alerts.png"

			anchors.top: signal_bg.bottom
		}

		Text {
			id: labelImmediate
			text: "Immediate Alert Level:"

			color: "#768EA5"
			font.pixelSize: 32

			anchors.top: immediate_bg.top
			anchors.left: immediate_bg.left
			anchors.topMargin: 20
			anchors.leftMargin: 10
		}

		Slider {
			id: immediateAlert
			objectName: "immediateAlert"

			anchors.horizontalCenter: immediate_bg.horizontalCenter
			anchors.top: labelImmediate.bottom
			anchors.topMargin: 15

			Connections {
				target: immediateAlert
				onValueChanged: monitor.onImmediateAlertChange(immediateAlert.value)
				}
		}

		Image {
			id: linkloss_bg
			source: "images/content_bg_alerts.png"

			anchors.top: immediate_bg.bottom
		}

		Text {
			id: labelLinkloss
			text: "Linkloss Alert Level:"

			color: "#768EA5"
			font.pixelSize: 32

			anchors.top: linkloss_bg.top
			anchors.left: linkloss_bg.left
			anchors.topMargin: 20
			anchors.leftMargin: 10
		}

		Slider {
			id: linkloss
			objectName: "linkloss"

			anchors.horizontalCenter: linkloss_bg.horizontalCenter
			anchors.top: labelLinkloss.bottom
			anchors.topMargin: 15

			Connections {
				target: linkloss
				onValueChanged: { monitor.onLinkLossChange(linkloss.value) }
			}
		}

		Connections {
			target: listView
			onCurrentItemChanged: {contentView.enabled = true }
		}
	}

	function valueToInt(property, value) {
		if (property == "Threshold") {
			if (value == "low")
				return 0;
			else if (value == 'medium')
				return 1;
			else if (value == 'high')
				return 2;
		} else {
			if (value == 'none')
				return 0;
			else if (value == 'mild')
				return 1;
			else if (value == 'high')
				return 2;
		}

	}

	Connections {
		target: monitor
		onPropertyValue: {
			if (property == "SignalLevel")
				signalLevel.text = value;
			else if (property == "ImmediateAlertLevel")
				immediateAlert.setValue(valueToInt(property, value))
			else if (property == "LinkLossAlertLevel")
				linkloss.setValue(valueToInt(property, value))
		}
	}
}
