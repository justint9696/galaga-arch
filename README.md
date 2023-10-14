## GALAGA

### Dependencies
#### Linux (Arch)
The following packages are required to run the application:
```
sudo pacman -S sdl2 sdl2_ttf sdl2_image
```

### Building
Use the following commands to clone and compile the application:
```
git clone https://github.com/justint9696/galaga-arch.git
cd galaga-arch
make all
```

Or if the SDL2 static libraries are needed:
```
git clone --recurse-submodules https://github.com/justint9696/galaga-arch.git
cd galaga-arch
make libs all
```

After a successful build has been made, it can be ran from the root directory with: 
```
make run
```