## GALAGA

### Dependencies
#### Linux (Arch)
The following packages are required to run the application:
```
sudo pacman -S sdl2 sdl2_ttf sdl2_image
```
#### Windows
To compile on Windows, download and install MinGW with the ```mingw-developer-tools``` and ```mingw-gcc-g++``` packages.<br>

[Here](https://ics.uci.edu/~pattis/common/handouts/mingweclipse/mingw.html) is an easy-to-follow guide on how to set up MinGW.<br>

### Building
Use the following commands to clone and compile the application:<br>

#### Linux
```
git clone https://github.com/justint9696/galaga-arch.git
cd galaga-arch
make all
```
#### Windows
```
git clone https://github.com/justint9696/galaga-arch.git
cd galaga-arch
make win32
```

After a successful build has been made, it can be ran from the **root directory** with ```bin/galaga```.
