/* The MIT License (MIT)
 *
 * Copyright (c) 2019 Stefano Trettel
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

/* Currently stb_perlin.h lacks a mechanism to make its functions static, such as
 * STB_IMAGE_STATIC in stb_image.h, so we prepend our moonimage_ prefix.
 */
#define stb_perlin_noise3_internal moonimage_stb_perlin_noise3_internal
float stb_perlin_noise3_internal(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, unsigned char seed);
#define stb_perlin_noise3 moonimage_stb_perlin_noise3
float stb_perlin_noise3(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap);
#define stb_perlin_noise3_seed moonimage_stb_perlin_noise3_seed
float stb_perlin_noise3_seed(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed);
#define stb_perlin_ridge_noise3 moonimage_stb_perlin_ridge_noise3
float stb_perlin_ridge_noise3(float x, float y, float z, float lacunarity, float gain, float offset, int octaves);
#define stb_perlin_fbm_noise3 moonimage_stb_perlin_fbm_noise3
float stb_perlin_fbm_noise3(float x, float y, float z, float lacunarity, float gain, int octaves);
#define stb_perlin_turbulence_noise3 moonimage_stb_perlin_turbulence_noise3
float stb_perlin_turbulence_noise3(float x, float y, float z, float lacunarity, float gain, int octaves);
#define stb_perlin_noise3_wrap_nonpow2 moonimage_stb_perlin_noise3_wrap_nonpow2
float stb_perlin_noise3_wrap_nonpow2(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, unsigned char seed);

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

/*------------------------------------------------------------------------------*
 | Perlin noise                                                                 |
 *------------------------------------------------------------------------------*/

static int Perlin(lua_State *L)
    {
    float value;
    float x = luaL_checknumber(L, 1);
    float y = luaL_optnumber(L, 2, 0);
    float z = luaL_optnumber(L, 3, 0);
    int x_wrap = luaL_optinteger(L, 4, 0);
    int y_wrap = luaL_optinteger(L, 5, 0);
    int z_wrap = luaL_optinteger(L, 6, 0);
    int seed = luaL_optinteger(L, 7, 0); /* 0-256 */
    value = stb_perlin_noise3_seed(x, y, z, x_wrap, y_wrap, z_wrap, seed);
    //@@value = stb_perlin_noise3_wrap_nonpow2(x, y, z, x_wrap, y_wrap, z_wrap, seed);
    /* Clamp between -1 and 1, just to be sure */
    if(value < -1.0f) value=-1.0f; else if(value > 1.0f) value=1.0f;
    lua_pushnumber(L, value);
    return 1;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "perlin", Perlin},
        { NULL, NULL } /* sentinel */
    };

void moonimage_open_perlin(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

