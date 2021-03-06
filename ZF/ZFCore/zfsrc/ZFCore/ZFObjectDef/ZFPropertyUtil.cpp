/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyUtil.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPropertyIsValueAccessed
zfbool ZFPropertyIsValueAccessed(ZF_IN const ZFProperty *propertyInfo,
                                 ZF_IN ZFObject *ownerObj)
{
    if(propertyInfo == zfnull || ownerObj == zfnull || !ownerObj->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass()))
    {
        return zffalse;
    }
    return propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
}

// ============================================================
// ZFPropertyIsInitValue
zfbool ZFPropertyIsInitValue(ZF_IN const ZFProperty *propertyInfo,
                             ZF_IN ZFObject *ownerObj)
{
    if(propertyInfo == zfnull || ownerObj == zfnull || !ownerObj->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass()))
    {
        return zffalse;
    }
    return propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, zfnull);
}

// ============================================================
// ZFPropertyCompare
ZFCompareResult ZFPropertyCompare(ZF_IN const ZFProperty *propertyInfo,
                                  ZF_IN ZFObject *obj0,
                                  ZF_IN ZFObject *obj1,
                                  ZF_OUT_OPT zfbool *successfullyCompared /* = zfnull */)
{
    if(propertyInfo == zfnull
       || obj0 == zfnull || !obj0->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass())
       || obj1 == zfnull || !obj1->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass()))
    {
        if(successfullyCompared != zfnull)
        {
            *successfullyCompared = zffalse;
        }
        return ZFCompareUncomparable;
    }
    else
    {
        if(successfullyCompared != zfnull)
        {
            *successfullyCompared = zftrue;
        }
        return propertyInfo->callbackCompare(propertyInfo, obj0, obj1);
    }
}

// ============================================================
// ZFPropertyCopy
zfbool ZFPropertyCopy(ZF_IN const ZFProperty *propertyInfo,
                      ZF_IN ZFObject *dstObj,
                      ZF_IN ZFObject *srcObj)
{
    if(propertyInfo == zfnull
        || dstObj == zfnull || !dstObj->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass())
        || srcObj == zfnull || !srcObj->classData()->classIsTypeOf(propertyInfo->propertyOwnerClass()))
    {
        return zffalse;
    }
    else
    {
        propertyInfo->callbackCopy(propertyInfo, dstObj, srcObj);
        return zftrue;
    }
}

// ============================================================
// retain property reflect
zfbool ZFPropertyRetainSet(ZF_IN const ZFProperty *propertyInfo,
                           ZF_IN ZFObject *dstObj,
                           ZF_IN ZFObject *src)
{
    if(dstObj != zfnull && propertyInfo != zfnull && propertyInfo->callbackRetainSet != zfnull
        && (src == zfnull || src->classData()->classIsTypeOf(propertyInfo->propertyClassOfRetainProperty())))
    {
        propertyInfo->callbackRetainSet(propertyInfo, dstObj, src);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}
ZFObject *ZFPropertyRetainGet(ZF_IN const ZFProperty *propertyInfo,
                              ZF_IN ZFObject *ownerObj,
                              ZF_OUT_OPT zfbool *success /* = zfnull */)
{
    if(ownerObj != zfnull && propertyInfo != zfnull && propertyInfo->callbackRetainGet != zfnull)
    {
        if(success != zfnull)
        {
            *success = zftrue;
        }
        return propertyInfo->callbackRetainGet(propertyInfo, ownerObj);
    }
    else
    {
        if(success != zfnull)
        {
            *success = zffalse;
        }
        return zfnull;
    }
}

// ============================================================
// assign property reflect
zfbool ZFPropertyAssignSet(ZF_IN const ZFProperty *propertyInfo,
                           ZF_IN ZFObject *dstObj,
                           ZF_IN void *src,
                           ZF_IN_OPT const zfchar *srcType /* = ZFPropertyTypeId_none */)
{
    if(dstObj != zfnull && propertyInfo != zfnull && propertyInfo->callbackAssignSet != zfnull
        && (zfscmpTheSame(srcType, propertyInfo->propertyTypeId())
            || zfscmpTheSame(srcType, ZFPropertyTypeId_none)
            || zfscmpTheSame(propertyInfo->propertyTypeId(), ZFPropertyTypeId_none)))
    {
        propertyInfo->callbackAssignSet(propertyInfo, dstObj, src);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}
const void *ZFPropertyAssignGet(ZF_IN const ZFProperty *propertyInfo,
                                ZF_IN ZFObject *ownerObj)
{
    if(ownerObj != zfnull && propertyInfo != zfnull && propertyInfo->callbackAssignGet != zfnull)
    {
        return propertyInfo->callbackAssignGet(propertyInfo, ownerObj);
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
// ZFPropertyCopyAll
void ZFPropertyCopyAll(ZF_IN ZFObject *dstObj,
                       ZF_IN ZFObject *srcObj,
                       ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */,
                       ZF_OUT_OPT ZFCoreArrayPOD<const ZFProperty *> *copiedProperties /* = zfnull */)
{
    if(dstObj == zfnull || srcObj == zfnull)
    {
        return ;
    }

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(dstObj->classData(), filter);
    const ZFClass *srcClass = srcObj->classData();
    for(zfindex i = 0; i < allProperty.count(); ++i)
    {
        const ZFProperty *property = allProperty.get(i);
        if(!srcClass->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        property->callbackCopy(property, dstObj, srcObj);
        if(copiedProperties != zfnull)
        {
            copiedProperties->add(property);
        }
    }
}

// ============================================================
// ZFPropertyGetInfo
void ZFPropertyGetInfo(ZF_IN_OUT zfstring &ret,
                       ZF_IN const ZFProperty *propertyInfo,
                       ZF_IN ZFObject *ownerObject)
{
    if(ownerObject != zfnull && propertyInfo != zfnull)
    {
        propertyInfo->callbackGetInfo(propertyInfo, ownerObject, ret);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsValueAccessed, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsInitValue, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFCompareResult, ZFPropertyCompare, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, obj0), ZFMP_IN(ZFObject *, obj1), ZFMP_OUT_OPT(zfbool *, successfullyCompared, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFPropertyCopy, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, dstObj), ZFMP_IN(ZFObject *, srcObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFPropertyRetainSet, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, dstObj), ZFMP_IN(ZFObject *, src))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFObject *, ZFPropertyRetainGet, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObj), ZFMP_OUT_OPT(zfbool *, success, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFPropertyCopyAll, ZFMP_IN(ZFObject *, dstObj), ZFMP_IN(ZFObject *, srcObj), ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull), ZFMP_OUT_OPT(ZFCoreArrayPOD<const ZFProperty *> *, copiedProperties, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFPropertyGetInfo, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObject))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFPropertyGetInfo, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObject))

ZF_NAMESPACE_GLOBAL_END
#endif

