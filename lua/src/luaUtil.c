#include <stdarg.h>
#include <stdio.h>  
#include <stdlib.h>

#include <lua.h>
#include "luaUtil.h"

void lua_errorAbort(lua_State* L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

void lua_stackDump(lua_State* L) {
    int i;
    int top = lua_gettop(L);
    for( i = 1; i <= top; i++) {
        int t = lua_type(L,i);
        switch(t) {
            case LUA_TSTRING:
                printf("%s", lua_tostring(L,i));
                break;
            case LUA_TBOOLEAN:
                printf(lua_toboolean(L,i) ? "true":"false");
                break;
            case LUA_TNUMBER:
                printf("%g",lua_tonumber(L,i));
                break;
            default:
                printf("%s",lua_typename(L,t));
                break;
        }
        printf("  ");
    }
    printf("\n");
}


