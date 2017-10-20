## MoonImage: Image Loading for Lua

MoonImage is a Lua image loading library based on Sean Barrett's 
[STB libraries](https://github.com/nothings/stb).

It runs on GNU/Linux and on Windows (MSYS2/MinGW) and requires
[Lua](http://www.lua.org/) (>=5.3).

_Author:_ _[Stefano Trettel](https://www.linkedin.com/in/stetre)_

[![Lua logo](./doc/powered-by-lua.gif)](http://www.lua.org/)

#### License

MIT/X11 license (same as Lua). See [LICENSE](./LICENSE).

#### Documentation

See the [Reference Manual](https://stetre.github.io/moonimage/doc/index.html).

#### Getting and installing

Setup the build environment as described [here](https://github.com/stetre/moonlibs), then:

```sh
$ git clone https://github.com/stetre/moonimage
$ cd moonimage
moonimage$ make
moonimage$ sudo make install
```

#### Example

The example below loads an image from a JPG file and writes it to a BMP file.

Other examples can be found in the **examples/** directory contained in the release package.

```lua
-- MoonImage example: hello.lua
mi = require('moonimage')

-- Load an image from a jpg file:
data, w, h, channels = mi.load("sunflowers.jpg", 'rgba')

-- Write it to a bmp file:
mi.write_bmp("output.bmp", w, h, channels, data)
```

The script can be executed at the shell prompt with the standard Lua interpreter:

```shell
$ lua hello.lua
```

#### See also

* [MoonLibs - Graphics and Audio Lua Libraries](https://github.com/stetre/moonlibs).
