
#include <stdarg.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "luaUtil.h"


#define MAX_COLOR   (255)

typedef struct _ColorTable {
    char* name;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}ColorTable;


static void loadfile(lua_State* L, const char* fname);
static int getfield(lua_State* L, const char* key);
static void setfield(lua_State* L, const char* index, int value);
static void setcolor(lua_State* L, ColorTable* ct);

static void call_va(lua_State*L, const char* func, const char* sig, ...);

static void call_va(lua_State*L, const char* func, const char* sig, ...)
{
    va_list vl;
    int narg;    /**< 引数の数 */
    int nres;    /**< 戻り値の数 */ 

    va_start(vl, sig);

    /* 関数をプッシュする */
    lua_getglobal(L, func);

    /* 引数を取り出してプッシュする*/
    for(narg = 0; *sig; narg++) {
        luaL_checkstack(L, 1, "too many arguments");

        switch(*sig++) {
        case 'd':
            lua_pushnumber(L, va_arg(vl,double));
            break;
        case 'i':
            lua_pushnumber(L, va_arg(vl,int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl,char*));
            break;
        case '>':
            goto endargs;
            break;
        default:
            lua_errorAbort(L, "invalid option (%c)", *(sig-1));
            break;
        }
    }
endargs:

    nres = strlen(sig);

    if(lua_pcall(L, narg, nres, 0) != 0)
        lua_errorAbort(L, "error calling '%s' :%s", func, lua_tostring(L,-1));

    nres = -nres;
    while(*sig) {
        switch(*sig++) {
        case 'd':
            if( !lua_isnumber(L, -1) ) {
                lua_errorAbort(L, "'wrong result type");
            }
            {
            *va_arg(vl,double*) = lua_tonumber(L,nres);
            }
            break;
        case 'i':
            if( !lua_isnumber(L, -1) ) {
                lua_errorAbort(L, "'wrong result type");
            }
            *va_arg(vl, int*) = lua_tonumber(L, nres);
            break;
        case 's':
            if( !lua_isstring(L, -1) ) {
                lua_errorAbort(L, "'wrong result type");
            }
            *va_arg(vl, const char**) = lua_tostring(L, nres);
            break;
        default:
            lua_errorAbort(L, "invalid option (%c)", *(sig-1));
            break;
        }
        nres++;
        
    }
    va_end(vl);

    return;
}




ColorTable colortable[] = {
    { "WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
    { "RED", MAX_COLOR, 0, 0},

    { NULL, 0, 0, 0},
};


static int getfield(lua_State* L, const char* key){
    int result = 0;
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if(!lua_isnumber(L, -1)) {
        lua_errorAbort(L, "'invalied componet is background color");
    }

    result = (int)lua_tonumber(L,-1);
    lua_pop(L,1);
    return result;
}

static void loadfile(lua_State* L, const char* fname) {
    if( luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0) ) {
        lua_errorAbort(L, "cannot run config. file: %s", lua_tostring(L,-1));
    }
}

static void setfield(lua_State* L, const char* index, int value) {
    lua_pushnumber(L, (double)value/MAX_COLOR);
    lua_setfield(L, -2, index);
}

static void setcolor(lua_State* L, ColorTable* ct) {
    lua_newtable(L);
    setfield(L, "r", ct->red);
    setfield(L, "g", ct->green);
    setfield(L, "b", ct->blue);
    lua_setglobal(L, ct->name);
}

#if 0
static void load(lua_State* L, const char* fname, int* w, int* h) {
    if( luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0) ) {
        lua_errorAbort(L, "cannot run config. file: %s", lua_tostring(L,-1));
    }

    lua_getglobal(L,"width");
    lua_getglobal(L,"height");
    if(!lua_isnumber(L,-2)) {
        lua_errorAbort(L, "'width' should be a number\n");
    }

    if(!lua_isnumber(L,-1)) {
        lua_errorAbort(L, "'height' should be a number\n");
    }
    *w = lua_tointeger(L, -2);
    *h = lua_tointeger(L, -1);
}
#endif


int main(int argc, char const* argv[])
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    int i = 0;
    while(colortable[i].name != NULL )
        setcolor(L, &colortable[i++]);

    loadfile(L,"./conf/config.lua");

    call_va(L, "print", "s", "hello", "NULL");
    double result = 0;
    call_va(L, "f", "dd>d", 2.0, 3.0, &result);
    printf("z=%lf\n", result);

#if 0
    int w = 0;
    int h = 0;
    load(L, "./script/config.lua", &w, &h);
    printf("width=%d\n", w);
    printf("height=%d\n", h);
#endif

    lua_getglobal(L, "bcakground");
    if(!lua_istable(L,-1))
        lua_errorAbort(L, "'bcakground' is not a table");

    int red = getfield(L, "r");
    int green =getfield(L, "g");
    int blue = getfield(L,"b");
    printf("red=%d green=%d blue=%d\n", red,green,blue);
    
    lua_close(L);
    return 0;
}
