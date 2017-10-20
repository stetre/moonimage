#!/usr/bin/env lua
-- MoonImage example: hello.lua

mi = require("moonimage")

-- Load an image from a jpg file:
data, w, h, channels = mi.load("sunflowers.jpg", 'rgba')

-- Write it to a bmp file:
mi.write_bmp("output.bmp", w, h, channels, data)

