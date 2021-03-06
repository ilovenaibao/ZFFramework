/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPointerHolder
ZFOBJECT_REGISTER(ZFPointerHolder)
#define _ZFP_ZFPointerHolderCacheSize 20
static ZFPointerHolder *_ZFP_ZFPointerHolderCache[_ZFP_ZFPointerHolderCacheSize];
static ZFPointerHolder **_ZFP_ZFPointerHolderCacheAvailable = _ZFP_ZFPointerHolderCache;
static ZFPointerHolder **_ZFP_ZFPointerHolderCacheAvailableEnd = _ZFP_ZFPointerHolderCache + _ZFP_ZFPointerHolderCacheSize;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPointerHolderCacheHolder, ZFLevelZFFrameworkEssential)
{
    zfmemset(_ZFP_ZFPointerHolderCache, 0, sizeof(_ZFP_ZFPointerHolderCache));
    _ZFP_ZFPointerHolderCacheAvailable = _ZFP_ZFPointerHolderCache;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPointerHolderCacheHolder)
{
    while(_ZFP_ZFPointerHolderCacheAvailable > _ZFP_ZFPointerHolderCache)
    {
        zfReleaseWithoutLeakTest(*_ZFP_ZFPointerHolderCacheAvailable);
        --_ZFP_ZFPointerHolderCacheAvailable;
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFPointerHolderCacheHolder)
ZFPointerHolder *ZFPointerHolder::cacheAccess(void)
{
    ZFPointerHolder *ret = zfnull;
    zfCoreMutexLock();
    if(_ZFP_ZFPointerHolderCacheAvailable > _ZFP_ZFPointerHolderCache)
    {
        ret = *(_ZFP_ZFPointerHolderCacheAvailable - 1);
        --_ZFP_ZFPointerHolderCacheAvailable;
    }
    else
    {
        ret = zfAllocWithoutLeakTest(ZFPointerHolder);
    }
    zfCoreMutexUnlock();
    return ret;
}
void ZFPointerHolder::cacheRelease(ZF_IN ZFPointerHolder *obj)
{
    zfCoreMutexLock();
    obj->holdedData = zfnull;
    obj->tagRemoveAll();
    if(_ZFP_ZFPointerHolderCacheAvailable == _ZFP_ZFPointerHolderCacheAvailableEnd)
    {
        zfReleaseWithoutLeakTest(obj);
    }
    else
    {
        *_ZFP_ZFPointerHolderCacheAvailable = obj;
        ++_ZFP_ZFPointerHolderCacheAvailable;
    }
    zfCoreMutexUnlock();
}

void ZFPointerHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(" ");
    zfsFromPointerT(ret, this->holdedData);
}
zfidentity ZFPointerHolder::objectHash(void)
{
    return zfidentityCalcPointer(this->holdedData);
}
ZFCompareResult ZFPointerHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->holdedData == another->holdedData)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

// ============================================================
// ZFTypeHolder
ZFOBJECT_REGISTER(ZFTypeHolder)

void ZFTypeHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(" ");
    zfsFromPointerT(ret, this->holdedData);
}
zfidentity ZFTypeHolder::objectHash(void)
{
    return zfidentityHash(
        zfidentityCalcPointer(this->holdedData),
        zfidentityCalcPointer(ZFCastReinterpret(void *, this->holderType)));
}
ZFCompareResult ZFTypeHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->holdedData == another->holdedData
        && this->holderType == another->holderType)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, ZFObjectCompare, ZFMP_IN(ZFObject *, e0), ZFMP_IN(ZFObject *, e1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFObjectInfoOfInstanceT, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFObjectInfoOfInstance, ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFObjectInfoT, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFObjectInfo, ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFObjectHash, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_GLOBAL_END
#endif

