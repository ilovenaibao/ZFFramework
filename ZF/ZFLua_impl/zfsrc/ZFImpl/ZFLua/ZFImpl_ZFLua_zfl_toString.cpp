/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfl_toString(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfl_toString] takes only one param, got %zi"),
            (zfindex)count);
        return luaL_error(L, "");
    }

    zfstring ret;
    if(!ZFImpl_ZFLua_toString(ret, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[zfl_toString] unknown param type, got %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1).cString());
        return luaL_error(L, "");
    }
    lua_pushstring(L, zfsCoreZ2A(ret.cString()));
    return 1;
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_toString, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_toString"), _ZFP_ZFImpl_ZFLua_zfl_toString);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

