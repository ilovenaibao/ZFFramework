/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFObjectHolder)

void ZFObjectHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(" ");
    zfsFromPointerT(ret, this->holdedObj.toObject());
}

zfidentity ZFObjectHolder::objectHash(void)
{
    return ZFObjectHash(this->holdedObj.toObject());
}
ZFCompareResult ZFObjectHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFObjectCompare(this->holdedObj.toObject(), another->holdedObj.toObject());
}

// ============================================================
ZFOBJECT_REGISTER(ZFListenerHolder)

zfidentity ZFListenerHolder::objectHash(void)
{
    return zfidentityHash(zfidentityCalcString(this->classData()->className())
        , zfidentityCalcPointer(this->listenerData.sender)
        , zfidentityCalcPointer(this->listenerData.param0)
        , zfidentityCalcPointer(this->listenerData.param1)
        , zfidentityCalcPointer(this->userData())
        );
}
ZFCompareResult ZFListenerHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->runnable.objectCompare(another->runnable) == ZFCompareTheSame
        && this->listenerData.sender == another->listenerData.sender
        && this->listenerData.param0 == another->listenerData.param0
        && this->listenerData.param1 == another->listenerData.param1
        && this->userData() == another->userData())
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

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFObjectHolder, ZFAny, holdedObj)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFListenerHolder, ZFListener, runnable)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFListenerHolder, ZFListenerData, listenerData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFListenerHolder, void, runnableExecute)

ZF_NAMESPACE_GLOBAL_END
#endif

