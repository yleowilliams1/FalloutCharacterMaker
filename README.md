## Requirements
 
- A C compiler (`gcc` or `clang`)
- [raylib](https://www.raylib.com/) installed where the compiler can find it
- Bash, to run the build script (native on Mac/Linux; see Windows notes below)
## Building
 
### Windows
 
1. **Install MinGW-w64** (provides `gcc`). The easiest route is
   [WinLibs](https://winlibs.com/) or the MinGW-w64 build from
   [w64devkit](https://github.com/skeeto/w64devkit) — download, unzip, and add
   its `bin` folder to your `PATH`.
2. **Install raylib for MinGW.** Download the `Win64 mingw` build from the
   [raylib releases page](https://github.com/raysan5/raylib/releases). Unzip
   it and note the path to its `include` and `lib` folders.
3. **Build:**
   - If you have Git Bash or MSYS2 installed, just run the original script:
```
     ./LINUX.sh
```
   - Otherwise, use `WINDOWS.bat` (included in this repo). If raylib isn't in
     your compiler's default search path, open `cpil.bat` and add your paths
     right before `-lraylib`, e.g.:
```
     -I"C:\raylib\include" -L"C:\raylib\lib"
```
     Then run:
```
     WINDOWS.bat
```
4. This produces `app.exe`. If raylib was linked dynamically, make sure its
   `.dll` is next to `app.exe` (or on your `PATH`) before running it.
### macOS
 
1. **Install Xcode Command Line Tools** (provides `clang`, aliased as `gcc`):
```
   xcode-select --install
```
2. **Install raylib via Homebrew:**
```
   brew install raylib
```
3. **Build.** The included script needs macOS-specific frameworks
   added to link successfully — run this instead of `./MAC` directly:
```
   gcc src/*.c src/util/*.c -o app \
       -lraylib -lm \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```
   (If you'd rather just run `./MAC`, add the four `-framework` flags to the
   `gcc` line inside the script — happy to patch that in for you.)
 
### Linux
 
1. **Install raylib** via your package manager, e.g.:
```
   sudo apt install libraylib-dev
```
2. **Build:**
```
   ./LINUX
```
 
## Running
 
Run the produced binary (`app` on Mac/Linux, `app.exe` on Windows) from the
repo's root directory — it expects `CharacterSheet.png`, `fallout.ttf`, and
the `data/` folder to be alongside it.

Run the application with the argument NEW to create a new file, when you exit the program it automatically overrides the current save so make sure you don't type NEW. Other wise running it normally will look fro the .SAVE and load that.
