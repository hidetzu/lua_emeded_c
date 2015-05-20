#ifndef __LUAUTIL_H__
#define __LUAUTIL_H__

void lua_errorAbort(lua_State* L, const char *fmt, ...);
void lua_stackDump(lua_State* L);


#endif  /*__LUAUTIL_H__*/
