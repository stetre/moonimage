/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Stefano Trettel
 *
 * Software repository: MoonImage, https://github.com/stetre/moonimage
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

static int Error(lua_State *L)
    {
    return luaL_error(L, "operation failed");
    }

static int Write_png(lua_State *L)
    {
    size_t len;
    const char *filename = luaL_checkstring(L, 1);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    const void *data = luaL_checklstring(L, 5, &len);
    int stride = luaL_optinteger(L, 6, 0);
    if(stride < 0)
        return luaL_argerror(L, 6, "stride must be non negative");
    if(len != (size_t)(w*h*channels))
        return luaL_error(L, "invalid data length");
    if(!stbi_write_png(filename, w, h, channels, data, stride)) return Error(L);
    return 0;
    }

static int Write_bmp(lua_State *L)
    {
    size_t len;
    const char *filename = luaL_checkstring(L, 1);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    const void *data = luaL_checklstring(L, 5, &len);
    if(len != (size_t)(w*h*channels))
        return luaL_error(L, "invalid data length");
    if(!stbi_write_bmp(filename, w, h, channels, data)) return Error(L);
    return 0;
    }

static int Write_tga(lua_State *L)
    {
    int rc;
    size_t len;
    const char *filename = luaL_checkstring(L, 1);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    const void *data = luaL_checklstring(L, 5, &len);
    int rle = optboolean(L, 6, 0);
    if(len != (size_t)(w*h*channels))
        return luaL_error(L, "invalid data length");
    if(!rle) stbi_write_tga_with_rle = 0;
    rc = stbi_write_tga(filename, w, h, channels, data);
    stbi_write_tga_with_rle = 1;
    if(!rc) return Error(L);
    return 0;
    }


static int Write_hdr(lua_State *L)
    {
    size_t len;
    const char *filename = luaL_checkstring(L, 1);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    const void *data = luaL_checklstring(L, 5, &len);
    if(len != (size_t)(w*h*channels*sizeof(float)))
        return luaL_error(L, "invalid data length");
    if(!stbi_write_hdr(filename, w, h, channels, (const float*)data)) return Error(L);
    return 0;
    }


static int Write_jpg(lua_State *L)
    {
    int rc;
    size_t len;
    const char *filename = luaL_checkstring(L, 1);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    const void *data = luaL_checklstring(L, 5, &len);
    int quality = luaL_checkinteger(L, 6);
    if((quality < 1) || (quality > 100))
        return luaL_argerror(L, 6, "quality must be between 1 (min) and 100 (max)");
    if(len != (size_t)(w*h*channels))
        return luaL_error(L, "invalid data length");
    rc = stbi_write_jpg(filename, w, h, channels, data, quality);
    if(!rc) return Error(L);
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "write_png", Write_png },
        { "write_bmp", Write_bmp },
        { "write_tga", Write_tga },
        { "write_hdr", Write_hdr },
        { "write_jpg", Write_jpg },
        { NULL, NULL } /* sentinel */
    };


void moonimage_open_write(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


