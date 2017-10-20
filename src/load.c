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
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_STATIC
#include "stb_image.h"

static int Error(lua_State *L)
    {
    const char * reason = stbi_failure_reason(); /* a VERY brief reason for failure */
    return luaL_error(L, (reason!=NULL) ? reason : "unknown reason");
    }

/* FLAGS --------------------------------------------------------------------*/

static int Reset_load_flags(lua_State *L)
    {
    (void)L;
    stbi_set_unpremultiply_on_load(0);
    stbi_convert_iphone_png_to_rgb(0);
    stbi_set_flip_vertically_on_load(0);
    return 0;
    }

#define SET_FLAG_FUNC(Func, func)       \
static int Func(lua_State *L)           \
    {                                   \
    stbi_##func(checkboolean(L, 1));    \
    return 0;                           \
    }

SET_FLAG_FUNC(Set_unpremultiply_on_load, set_unpremultiply_on_load)
SET_FLAG_FUNC(Convert_iphone_png_to_rgb, convert_iphone_png_to_rgb)
SET_FLAG_FUNC(Set_flip_vertically_on_load, set_flip_vertically_on_load)

#undef SET_FLAG_FUNC

/*@@TODO: Image object
 * image = load_image(...) 
 * Instead of passing the image data as a binary string, returns an image object
 * that holds the pointer to the data returned by stbi_load().
 * The data is retained until the object is destroyed.
 *
 * Methods:
 *   image:data() -> returns the data as a binary string
 *   image:ptr()
 *   image:size()
 *   image:width()
 *   image:height()
 *   image:channels()
 *   image:free()
 *
 * Also, make write_xxx() functions accept a image object in lieu of data,w,h.
 *
 * Advantages: when the loaded image is to be passed to a C library that accepts
 * (ptr, size), with this interface we can just pass the pointer from C to Lua and
 * from Lua to C (zero-copy).
 */

static int Load(lua_State *L)
    {
    int x, y, len, chansz;
    int channels;
    void *data; 
    const char *filename = luaL_checkstring(L, 1);
    int desired_channels = optchannels(L, 2);
    int chantype = optchantype(L, 3, CHANTYPE_u8);

    switch(chantype)
        {
        case CHANTYPE_u8: 
                chansz = sizeof(stbi_uc);
                data = stbi_load(filename, &x, &y, &channels, desired_channels);
                break;
        case CHANTYPE_u16:
                chansz = sizeof(stbi_us);
                data = stbi_load_16(filename, &x, &y, &channels, desired_channels);
                break;
        case CHANTYPE_f:
                chansz = sizeof(float);
                data = stbi_loadf(filename, &x, &y, &channels, desired_channels);
                break;
        default: 
            return unexpected(L);
        }

    if(!data) return Error(L);
    channels = (desired_channels > 0) ? desired_channels : channels;
    len = x * y * channels * chansz;
    lua_pushlstring(L, (const char*)data, len);
    stbi_image_free((void*)data);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    pushchannels(L, channels); 
    /* note that this function returns the used no. of channels, while stbi_load()
     * returns the channels in file even if it is not the used number */
    return 4;
    }


static int Set_gamma_and_scale(lua_State *L)
    {
    float gamma = luaL_checknumber(L, 1);
    float scale = luaL_checknumber(L, 2);
    stbi_hdr_to_ldr_gamma(gamma);
    stbi_hdr_to_ldr_scale(scale);
    stbi_ldr_to_hdr_gamma(gamma);
    stbi_ldr_to_hdr_scale(scale);
    return 0;
    }

static int Gamma_and_scale(lua_State *L)
    {
    lua_pushnumber(L, stbi__l2h_gamma);
    lua_pushnumber(L, stbi__l2h_scale);
//  lua_pushnumber(L, stbi__h2l_gamma_i);
//  lua_pushnumber(L, stbi__h2l_scale_i);
    return 2;
    }

static int Is_hdr(lua_State *L)
    {
    const char *filename = luaL_checkstring(L, 1);
    lua_pushboolean(L, stbi_is_hdr(filename));
    return 1;
    }

static int Info(lua_State *L)
    {
    int x, y, channels;
    const char *filename = luaL_checkstring(L, 1);

    if(!stbi_info(filename, &x, &y, &channels)) 
        return Error(L);

    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    pushchannels(L, channels);
    return 3;
    }

static int Reduce_to_u8(lua_State *L)
    {
    size_t len;
    void *inout;
    const char *data = luaL_checklstring(L, 1, &len);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    int channels = checkchannels(L, 4);
    int chantype = checkchantype(L, 5);

    switch(chantype)
        {
        case CHANTYPE_u8: 
                lua_pushlstring(L, data, len);
                return 1;
        case CHANTYPE_u16:
                if(len != (size_t)(w*h*channels*sizeof(stbi_us)))
                    return luaL_error(L, "invalid data length");
                inout = STBI_MALLOC(len);
                memcpy(inout, data, len);
                inout = stbi__convert_16_to_8((stbi_us*)inout, w, h, channels);
                break;
        case CHANTYPE_f:
                if(len != (size_t)(w*h*channels*sizeof(float)))
                    return luaL_error(L, "invalid data length");
                inout = STBI_MALLOC(len);
                memcpy(inout, data, len);
                inout = stbi__hdr_to_ldr((float*)inout, w, h, channels);
                break;
        default: 
            return unexpected(L);
        }

    if(!inout) return Error(L);
    len = (size_t)(w*h*channels);
    lua_pushlstring(L, (const char*)inout, len);
    STBI_FREE(inout);
    return 1;

    }


static const struct luaL_Reg Functions[] = 
    {
        { "info", Info },
        { "reset_load_flags", Reset_load_flags },
        { "unpremultiply_on_load", Set_unpremultiply_on_load },
        { "convert_iphone_png_to_rgb", Convert_iphone_png_to_rgb },
        { "flip_vertically_on_load", Set_flip_vertically_on_load },
        { "load", Load },
        { "set_gamma_and_scale", Set_gamma_and_scale },
        { "gamma_and_scale", Gamma_and_scale },
        { "is_hdr", Is_hdr },
        { "reduce_to_u8", Reduce_to_u8 },
        { NULL, NULL } /* sentinel */
    };


void moonimage_open_load(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


