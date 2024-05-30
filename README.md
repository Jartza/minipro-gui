![minipro.jpg](res%2FAppIcon.ico)
### Very-much experimental GUI wrapper for the awesome [minipro](https://gitlab.com/DavidGriffith/minipro) CLI tool
*(Not affiliated with MiniPRO devices or manufacturers, nor with the minipro CLI project.)*

![Screenshot 2024-05-28 at 09.18.19.png](res%2Fscreenshot.png)
`git clone https://github.com/twelve-chairs/minipro-gui.git`

`cd minipro-gui`

#### MacOS

`brew install cmake qt6 minipro`

`cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B "./cmake-build-minsizerel"`

`cmake --build ./cmake-build-minsizerel --target minipro-gui -- -j 2`

`./cmake-build-minsizerel/minipro-gui`

#### Ubuntu
Install minipro: https://gitlab.com/DavidGriffith/minipro#install-build-dependencies

`sudo apt-get install build-essential libgl1-mesa-dev cmake qt6-base-dev`

`cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" . -B "./cmake-build-minsizerel"`

`cmake --build ./cmake-build-minsizerel --target minipro-gui -- -j 2`

`./cmake-build-minsizerel/minipro-gui`

*If your OS uses Wayland (e.g. Raspberry PI5), please also install `qt6-wayland` via apt-get*

#### Fedora
Install minipro: https://gitlab.com/DavidGriffith/minipro#install-build-dependencies

`sudo dnf groupinstall "C Development Tools and Libraries"`

`sudo dnf install cmake mesa-libGL-devel qt6-qtbase-devel`

`cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" . -B "./cmake-build-minsizerel"`

`cmake --build ./cmake-build-minsizerel --target minipro-gui -- -j 2`

`./cmake-build-minsizerel/minipro-gui`
