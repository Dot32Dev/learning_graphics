# Learning Graphics
### Dual-Grid autotiling with C and OpenGL

My first ever OpenGL project

<img width="912" height="740" alt="Screenshot 2025-07-16 at 11 03 15 pm" src="https://github.com/user-attachments/assets/cb2096d5-0847-4234-9f0e-1f40abb57d0a" />

I first taught myself dual-grid autotiling with my Lua/Löve [dualgrid](https://github.com/Dot32Dev/dualgrid) project. Feels cool implementing it in C and OpenGL!

## Building
Probably look at the gitignore and try to get the library files listed there. 

For glad:
- Go to https://glad.dav1d.de/
- Under gl, select 4.1
- For profile, choose core
- Generatre, dunk the file in /lib

For stb_image
- Go to https://github.com/nothings/stb/blob/master/stb_image.h
- Download raw
- Place it in lib/include/stb_image/

For GLFW
- Go to https://github.com/glfw/glfw/releases/tag/3.4
- Download the bin zip for your system
- For MacOS, get the lib-arm64 folder and put it in /lib <br>
(for other OS's, I guess rename a folder to lib-arm64 too and put glfw binaries in there)
