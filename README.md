# PandaOS Dock

dock for pandaos, framework code reference dde-dock.

## Dependencies

* sudo apt install g++ qt5-default qttools5-dev-tools libxcb-image0-dev libxcb-composite0-dev libxcb-ewmh-dev libqt5x11extras5-dev libxcb-damage0-dev libqt5svg5-dev libxtst-dev libkf5windowsystem-dev

## Build

* mkdir build
* cd build
* qmake ..
* make

## Build a deb package

* dpkg-buildpackage -b

## License

pandaos-dock is licensed under GPLv3.
