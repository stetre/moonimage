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

/********************************************************************************
 * Internal common header                                                       *
 ********************************************************************************/

#ifndef internalDEFINED
#define internalDEFINED

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "moonimage.h"

#define TOSTR_(x) #x
#define TOSTR(x) TOSTR_(x)

/* Note: all the dynamic symbols of this library (should) start with 'moonimage_' .
 * The only exception is the luaopen_moonimage() function, which is searched for
 * with that name by Lua.
 * MoonImage's string references on the Lua registry also start with 'moonimage_'.
 */

#if 0
/* .c */
#define  moonimage_
#endif

/* utils.c */
#define noprintf moonimage_noprintf
int noprintf(const char *fmt, ...); 
#define notavailable moonimage_notavailable
int notavailable(lua_State *L, ...);
#define malloc_init moonimage_malloc_init
void malloc_init(lua_State *L);
#define Malloc moonimage_Malloc
void *Malloc(lua_State *L, size_t size);
#define MallocNoErr moonimage_MallocNoErr
void *MallocNoErr(lua_State *L, size_t size);
#define Strdup moonimage_Strdup
char *Strdup(lua_State *L, const char *s);
#define Free moonimage_Free
void Free(lua_State *L, void *ptr);
#define checkboolean moonimage_checkboolean
int checkboolean(lua_State *L, int arg);
#define testboolean moonimage_testboolean
int testboolean(lua_State *L, int arg, int *err);
#define optboolean moonimage_optboolean
int optboolean(lua_State *L, int arg, int d);

#define checkchannels moonimage_checkchannels
int checkchannels(lua_State *L, int arg);
#define optchannels moonimage_optchannels
int optchannels(lua_State *L, int arg);
#define pushchannels moonimage_pushchannels
int pushchannels(lua_State *L, int val);
#define checkchantype moonimage_checkchantype
int checkchantype(lua_State *L, int arg);
#define optchantype moonimage_optchantype
int optchantype(lua_State *L, int arg, int defval);
#define pushchantype moonimage_pushchantype
int pushchantype(lua_State *L, int val);

#define checkflags(L, arg) (uint32_t)luaL_checkinteger((L), (arg))
#define optflags(L, arg, defval) (uint32_t)luaL_optinteger((L), (arg), (defval))


/* Color channels.
 * Note that the length of the corresponding Lua literal gives the number of channels
 * (y = 1, ya = 2, rgb = 3, rgba = 4).
 */
#define CHAN_default    0 /* STBI_default */
#define CHAN_y          1 /* STBI_grey */
#define CHAN_ya         2 /* STBI_grey_alpha */
#define CHAN_rgb        3 /* STBI_rgb */
#define CHAN_rgba       4 /* STBI_rgb_alpha */

/* Color channel type. */
#define CHANTYPE_u8     0
#define CHANTYPE_u16    1
#define CHANTYPE_f      2
/* Internal error codes */
#define ERR_NOTPRESENT       1
#define ERR_SUCCESS          0
#define ERR_GENERIC         -1
#define ERR_TYPE            -2
#define ERR_VALUE           -3
#define ERR_TABLE           -4
#define ERR_EMPTY           -5
#define ERR_MEMORY          -6
#define ERR_LENGTH          -7
#define ERR_POOL            -8
#define ERR_BOUNDARIES      -9
#define ERR_UNKNOWN         -10
#define errstring moonimage_errstring
const char* errstring(int err);

/* main.c */
int luaopen_moonimage(lua_State *L);
void moonimage_open_load(lua_State *L);
void moonimage_open_write(lua_State *L);
void moonimage_open_perlin(lua_State *L);

/*------------------------------------------------------------------------------*
 | Debug and other utilities                                                    |
 *------------------------------------------------------------------------------*/

/* If this is printed, it denotes a suspect bug: */
#define UNEXPECTED_ERROR "unexpected error (%s, %d)", __FILE__, __LINE__

#define unexpected(L) luaL_error((L), UNEXPECTED_ERROR)
#define notsupported(L) luaL_error((L), "operation not supported")

#define badvalue(L,s)   lua_pushfstring((L), "invalid value '%s'", (s))

/* DEBUG -------------------------------------------------------- */
#if defined(DEBUG)

#define DBG printf
#define TR() do { printf("trace %s %d\n",__FILE__,__LINE__); } while(0)
#define BK() do { printf("break %s %d\n",__FILE__,__LINE__); getchar(); } while(0)

#else 

#define DBG noprintf
#define TR()
#define BK()

#endif /* DEBUG ------------------------------------------------- */


#endif /* internalDEFINED */
