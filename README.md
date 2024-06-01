![minipro.jpg](res%2FAppIcon.ico)
## MINIPRO-GUI
### Basic GUI wrapper for the amazing [minipro](https://gitlab.com/DavidGriffith/minipro) CLI tool
*(Not affiliated with MiniPRO devices or manufacturers, nor with the minipro CLI project.)*

![screenshot.png](res%2Fscreenshot.png)
### Installation
#### MacOS

```
brew install cmake qt6 minipro
```

#### Ubuntu / Debian
Install minipro: https://gitlab.com/DavidGriffith/minipro#install-build-dependencies

```
sudo apt-get install build-essential cmake libgl1-mesa-dev qt6-base-dev -y
```

  (*If your OS uses Wayland (e.g. Raspberry PI 5), please also install `qt6-wayland` via apt-get*)

#### Fedora
Install minipro: https://gitlab.com/DavidGriffith/minipro#install-build-dependencies

```
sudo dnf groupinstall "C Development Tools and Libraries"
sudo dnf install cmake mesa-libGL-devel qt6-qtbase-devel -y
```

#### openSUSE
Install minipro: https://gitlab.com/DavidGriffith/minipro#install-build-dependencies
```
sudo zypper install --type pattern devel_basis
sudo zypper install cmake Mesa-libGL-devel qt6-base-devel -y
```

### Build and run
```
git clone https://github.com/twelve-chairs/minipro-gui.git
cd minipro-gui
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B "./cmake-build-minsizerel"
cmake --build ./cmake-build-minsizerel --target minipro-gui -- -j 2
./cmake-build-minsizerel/minipro-gui
```