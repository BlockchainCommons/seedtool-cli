## Seedtool on Windows

### Abstract

This document describes building `seedtool` with `MSYS2` and its usage on `Windows 10` 64-bit.

### Installing MSYS2 and Packages

* First, install `MSYS2` by downloading the installer and following the installation guide in [www.msys2.org](www.msys2.org).

* Run `MSYS2` and make sure the package database is updated:

```bash
# pacman -Syu
# pacman -Su
```

* Next, install the compiler and the required packages:

```bash
# pacman -S mingw-w64-x86_64-clang
# pacman -S patch
# pacman -S git
# pacman -S make
# pacman -S mingw-w64-x86_64-libc++
# pacman -S autoconf
# pacman -S automake1.8
```

### Compiling

First, clone `bc-seedtool-cli`, e.g. into `C:\msys64\home`, open `MSYS2 MinGW 64-bit` application and `cd` into `C:\msys64\home\bc-seedtool-cli`

* Now, run the build script with:

```bash
# export CC="clang" && export CXX="clang++" && ./build.sh
```

* and install:

```bash
# make install
```

You can now freely use `seedtool` inside `MSYS2 MinGW 64-bit` console.


### Running Seedtool as a Native Windows App

To be able to use `seedtool` as a native app on Windows outside `msys2/mingw64`, you have to expose 3 files to the system: `seedtool.exe`, `libc++.dll` and `libunwind.dll` which all reside in `C:\msys64\mingw64\bin`. So go ahead and add that folder to the `Windows PATH` by the following command in `Windows Cmd`:

```bash
set PATH=%PATH%;C:\msys64\mingw64\bin
```

That's it. Now you can use `seedtool` as a native Windows app in the Windows command line tool.


*Note:* If you want to pipe seedtool ouput into a QR code generator, you could use:

```bash
seedtool --ur | python -c "import sys; print(sys.stdin.readlines()[0].upper())" | qr > seedqrcode.png
```

For this you'll need `Python` and the following package:

```bash
pip install qrcode[pil]
```

