/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializable.h"
#include "ZFObjectImpl.h"
#include "ZFObjectCreator.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cache in class's meta data for performance
zfclassNotPOD _ZFP_ZFSerializable_PropertyTypeData
{
public:
    const ZFProperty *property;
    ZFSerializablePropertyType propertyType;
};
zfclass _ZFP_I_ZFSerializablePropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFSerializablePropertyTypeHolder, ZFObject)
public:
    ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> serializableProperty;
    ZFCoreMap serializablePropertyMap; // _ZFP_ZFSerializable_PropertyTypeData *

public:
    void addData(ZF_IN const ZFProperty *property,
                 ZF_IN ZFSerializablePropertyType propertyType)
    {
        _ZFP_ZFSerializable_PropertyTypeData *typeData = zfnew(_ZFP_ZFSerializable_PropertyTypeData);
        typeData->property = property;
        typeData->propertyType = propertyType;

        this->serializableProperty.add(typeData);
        this->serializablePropertyMap.set(property->propertyName(),
            ZFCorePointerForObject<_ZFP_ZFSerializable_PropertyTypeData *>(typeData));
    }
};

// ============================================================
// ZFSerializable
ZFSerializable::~ZFSerializable(void)
{
    zfsChange(this->_ZFP_ZFSerializable_editModeWrappedClassName, (const zfchar *)zfnull);
    if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        zfdelete(this->_ZFP_ZFSerializable_editModeWrappedElementDatas);
        this->_ZFP_ZFSerializable_editModeWrappedElementDatas = zfnull;
    }
    if(this->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
    {
        zfdelete(this->_ZFP_ZFSerializable_referenceInfoMap);
        this->_ZFP_ZFSerializable_referenceInfoMap = zfnull;
    }
    zfsChange(this->_ZFP_ZFSerializable_styleableType, (const zfchar *)zfnull);
    zfsChange(this->_ZFP_ZFSerializable_styleableData, (const zfchar *)zfnull);
}

ZFCoreMap &ZFSerializable::editModeData(void)
{
    static ZFCoreMap m;
    return m;
}
zfbool &ZFSerializable::editMode(void)
{
    static zfbool v = zffalse;
    return v;
}
const zfchar *ZFSerializable::editModeWrappedClassName(void)
{
    return this->_ZFP_ZFSerializable_editModeWrappedClassName;
}
void ZFSerializable::editModeWrappedClassNameSet(ZF_IN const zfchar *value)
{
    zfsChange(this->_ZFP_ZFSerializable_editModeWrappedClassName, value);
}
ZFCoreArray<ZFSerializableData> &ZFSerializable::editModeWrappedElementDatas(void)
{
    if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas == zfnull)
    {
        this->_ZFP_ZFSerializable_editModeWrappedElementDatas = zfnew(ZFCoreArray<ZFSerializableData>);
    }
    return *(this->_ZFP_ZFSerializable_editModeWrappedElementDatas);
}

zfbool ZFSerializable::serializable(void)
{
    return this->serializableOnCheck();
}
zfbool ZFSerializable::serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    this->serializableOnSerializeFromDataPrepare(serializableData);

    // reference logic
    if(serializableData.referenceInfoExist())
    {
        ZFSerializableData referenceInfo;
        referenceInfo.itemClassSet(serializableData.itemClass());
        referenceInfo.referenceRefTypeSet(serializableData.referenceRefType());
        referenceInfo.referenceRefDataSet(serializableData.referenceRefData());
        this->referenceInfoForSelfSet(&referenceInfo);
    }

    // styleable logic
    {
        const zfchar *styleableType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleableType);
        const zfchar *styleableData = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleableData);
        if((styleableType == zfnull) != (styleableData == zfnull))
        {
            const zfchar *missing = ((styleableType == zfnull) ? ZFSerializableKeyword_styleableType : ZFSerializableKeyword_styleableData);
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("missing %s"), missing);
            return zffalse;
        }
        if(styleableType != zfnull)
        {
            if(serializableData.referenceInfoExist())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("reference logic and styleable logic can not be used together"));
                return zffalse;
            }

            if(!this->classData()->classIsTypeOf(ZFStyleable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("serializing object %s is not type of %s, while created from \"%s\" \"%s\""),
                    this->toObject()->objectInfoOfInstance().cString(),
                    ZFStyleable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }

            zfautoObject styleableObj = ZFObjectCreate(styleableType, styleableData);
            if(styleableObj == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("failed to create object from \"%s\" \"%s\""), styleableType, styleableData);
                return zffalse;
            }
            if(!styleableObj.toObject()->classData()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("object %s is not type of %s, while created from \"%s\" \"%s\""),
                    styleableObj.toObject()->objectInfoOfInstance().cString(),
                    ZFSerializable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }
            if(!styleableObj.toObject()->classData()->classIsTypeOf(ZFStyleable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("object %s is not type of %s, while created from \"%s\" \"%s\""),
                    styleableObj.toObject()->objectInfoOfInstance().cString(),
                    ZFStyleable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }

            ZFCastZFObjectUnchecked(ZFStyleable *, this)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, styleableObj));
        }
        this->serializableStyleableTypeSet(styleableType);
        this->serializableStyleableDataSet(styleableData);
    }

    // editMode
    if(ZFSerializable::editMode())
    {
        ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(this->editModeWrappedClassName());
        if(editModeData != zfnull)
        {
            for(zfindex i = 0; i < serializableData.elementCount(); ++i)
            {
                const ZFSerializableData &element = serializableData.elementAtIndex(i);
                if(element.editMode())
                {
                    this->editModeWrappedElementDatas().add(element.copy());
                    element.resolveMarkAll();
                }
            }
        }
    }

    // property
    {
        const ZFCoreMap &propertyMap = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializablePropertyMap;
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            const ZFSerializableData &element = serializableData.elementAtIndex(i);
            if(element.resolved() || element.category() != zfnull)
            {
                continue;
            }
            const zfchar *propertyName = ZFSerializableUtil::checkPropertyName(element);
            if(propertyName == zfnull)
            {
                continue;
            }
            _ZFP_ZFSerializable_PropertyTypeData *data = propertyMap.get<_ZFP_ZFSerializable_PropertyTypeData *>(propertyName);
            if(data == zfnull)
            {
                continue;
            }

            // serialize the property
            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    if(!this->serializableOnSerializePropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    // mark self as resolved
    serializableData.resolveMark();

    // check whether all resoved
    if(!ZFSerializableUtilErrorOutputCallbacks.isEmpty())
    {
        zfstring tmp;
        if(ZFSerializableUtil::printResolveStatus(serializableData, ZFOutputCallbackForString(tmp)))
        {
            ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
            for(zfindex i = 0; i < callbacks.count(); ++i)
            {
                callbacks[i].execute(tmp.cString());
            }
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                       ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    if(referencedObject != zfnull
        && !referencedObject->classData()->classIsTypeOf(this->classData())
        && !this->classData()->classIsTypeOf(referencedObject->classData()))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("serialize with a reference object whose type mismatch, self: %s, reference: %s"),
            this->classData()->className(), referencedObject->classData()->className());
        return zffalse;
    }

    this->serializableOnSerializeToDataPrepare(serializableData);

    ZFSerializable *referencedOwnerOrNull = zfRetain(referencedObject);
    {
        const ZFSerializableData *referenceInfo = this->referenceInfoForSelf();
        if(referenceInfo != zfnull)
        { // reference logic
            ZFSerializableData referencedData = referenceInfo->copy();
            if(!referencedData.referenceInfoLoad(outErrorHint))
            {
                return zffalse;
            }
            zfautoObject referencedOwnerObject;
            if(!ZFObjectFromSerializableData(referencedOwnerObject, referencedData, outErrorHint))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("failed to load from reference, type: %s, data: %s"),
                    referencedData.referenceRefType(), referencedData.referenceRefData());
                return zffalse;
            }
            zfRelease(referencedOwnerOrNull);
            referencedOwnerOrNull = zfRetain(ZFCastZFObjectUnchecked(zfself *, referencedOwnerObject.toObject()));

            serializableData.attributeSet(ZFSerializableKeyword_refType, referenceInfo->referenceRefType());
            serializableData.attributeSet(ZFSerializableKeyword_refData, referenceInfo->referenceRefData());
        }
        else if(this->serializableStyleableTypeGet() != zfnull || this->serializableStyleableDataGet() != zfnull)
        { // styleable logic
            zfautoObject styleableObj = ZFObjectCreate(this->serializableStyleableTypeGet(), this->serializableStyleableDataGet());
            if(styleableObj == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("failed to create object from \"%s\" \"%s\""),
                    this->serializableStyleableTypeGet(), this->serializableStyleableDataGet());
                return zffalse;
            }
            zfRelease(referencedOwnerOrNull);
            referencedOwnerOrNull = zfRetain(ZFCastZFObject(zfself *, styleableObj.toObject()));
        }
    }
    zfblockedRelease(referencedOwnerOrNull ? referencedOwnerOrNull->toObject() : zfnull);

    // property
    {
        const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &allProperty = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            _ZFP_ZFSerializable_PropertyTypeData *data = allProperty[i];
            ZFSerializableData propertyData;

            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    if(!this->serializableOnSerializePropertyToData(propertyData,
                                                                    data->property,
                                                                    referencedOwnerOrNull,
                                                                    outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyToData(propertyData,
                                                                           data->property,
                                                                           referencedOwnerOrNull,
                                                                           outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }

            if(propertyData.itemClass() != zfnull)
            {
                propertyData.propertyNameSet(data->property->propertyName());
                serializableData.elementAdd(propertyData);
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint))
    {
        return zffalse;
    }

    if(ZFSerializable::editMode() && this->editModeWrappedClassName() != zfnull)
    {
        serializableData.itemClassSet(this->editModeWrappedClassName());
    }
    else
    {
        serializableData.itemClassSet(this->classData()->className());
    }

    if(ZFSerializable::editMode() && this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        for(zfindex i = 0; i < this->_ZFP_ZFSerializable_editModeWrappedElementDatas->count(); ++i)
        {
            serializableData.elementAdd(this->_ZFP_ZFSerializable_editModeWrappedElementDatas->get(i));
        }
    }

    return zftrue;
}

_ZFP_I_ZFSerializablePropertyTypeHolder *ZFSerializable::_ZFP_ZFSerializable_getPropertyTypeHolder(void)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFSerializablePropertyTypeHolder *holder = this->classData()->classTagGet<_ZFP_I_ZFSerializablePropertyTypeHolder *>(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className());
    if(holder == zfnull)
    {
        zflockfree_zfblockedAllocWithoutLeakTest(_ZFP_I_ZFSerializablePropertyTypeHolder, holderTmp);
        ZFCoreArrayPOD<const ZFProperty *> allProperty;
        {
            // the order affects the serialization step's order,
            // sort parent's property first for human-readable
            ZFCoreArrayPOD<const ZFClass *> allClass;
            const ZFClass *tmpCls = this->classData();
            allClass.add(tmpCls);
            do
            {
                for(zfindex i = tmpCls->implementedInterfaceCount() - 1; i != zfindexMax; --i)
                {
                    allClass.add(tmpCls->implementedInterfaceAtIndex(i));
                }
                if(tmpCls->parentClass() != zfnull)
                {
                    allClass.add(tmpCls->parentClass());
                }
                tmpCls = tmpCls->parentClass();
            } while(tmpCls != zfnull);
            for(zfindex i = allClass.count() - 1; i != zfindexMax; --i)
            {
                tmpCls = allClass[i];
                for(zfindex iProperty = 0; iProperty < tmpCls->propertyCount(); ++iProperty)
                {
                    allProperty.add(tmpCls->propertyAtIndex(iProperty));
                }
            }
        }

        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            const ZFProperty *property = allProperty[i];
            switch(this->serializableOnCheckPropertyType(property))
            {
                case ZFSerializablePropertyTypeNotSerializable:
                    break;
                case ZFSerializablePropertyTypeSerializableProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeSerializableProperty);
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeEmbededProperty);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }

        holder = holderTmp;
        this->classData()->classTagSet(
            _ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className(),
            holderTmp);
        this->classData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className());
    }
    return holder;
}
void ZFSerializable::serializableGetAllSerializablePropertyT(ZF_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeSerializableProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}
void ZFSerializable::serializableGetAllSerializableEmbededPropertyT(ZF_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeEmbededProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}

ZFSerializablePropertyType ZFSerializable::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(!property->propertyIsSerializable())
    {
        return ZFSerializablePropertyTypeNotSerializable;
    }

    if(property->propertyIsRetainProperty())
    {
        if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
                || !property->propertyClassOfRetainProperty()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeEmbededProperty;
            }
        }
        else
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeSerializableProperty;
            }
        }
    }
    else
    {
        if(property->propertyIsSerializable())
        {
            return ZFSerializablePropertyTypeSerializableProperty;
        }
        else
        {
            return ZFSerializablePropertyTypeNotSerializable;
        }
    }
}

zfbool ZFSerializable::serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                               ZF_IN const ZFProperty *property,
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(property->propertyIsRetainProperty())
    {
        zfautoObject obj;
        if(!ZFObjectFromSerializableData(obj, propertyData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(obj != zfautoObjectNull && !obj.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
                zfText("object %s not type of %s"),
                obj.toObject()->objectInfoOfInstance().cString(), property->propertyClassOfRetainProperty()->className());
            return zffalse;
        }
        property->callbackRetainSet(property, this->toObject(), obj.toObject());
        return zftrue;
    }

    ZFPropertyTypeSerializeFromCallback serializeFromCallback = ZFPropertyTypeGetSerializeFromCallback(propertyData.itemClass());
    if(serializeFromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            propertyData.itemClass(),
            this->classData()->className());
        return zffalse;
    }
    if(!serializeFromCallback(property, this->toObject(), propertyData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    // save assign property's reference info
    if(propertyData.referenceInfoExistRecursively())
    {
        ZFSerializableData referenceInfo;
        referenceInfo.copyFrom(propertyData);
        this->referenceInfoSet(zfstringWithFormat(zfText("p:%s"), property->propertyName()), &referenceInfo);
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                             ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && property->callbackCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(!property->propertyIsSerializable()
        || property->callbackIsInitValue(property, this->toObject(), zfnull))
    {
        return zftrue;
    }

    if(property->propertyIsRetainProperty())
    {
        ZFObject *obj = property->callbackRetainGet(property, this->toObject());
        if(!ZFObjectToSerializableData(propertyData, obj, outErrorHint))
        {
            return zffalse;
        }
        return zftrue;
    }

    ZFPropertyTypeSerializeToCallback serializeToCallback = ZFPropertyTypeGetSerializeToCallback(property->propertyTypeId());
    if(serializeToCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            property->propertyTypeId(),
            this->classData()->className());
        return zffalse;
    }
    if(!serializeToCallback(property, this->toObject(), propertyData, outErrorHint))
    {
        return zffalse;
    }

    // restore reference info
    const ZFSerializableData *referenceInfo = this->referenceInfo(zfstringWithFormat(zfText("p:%s"), property->propertyName()));
    if(referenceInfo != zfnull)
    {
        propertyData.referenceInfoRestore(*referenceInfo);
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                      ZF_IN const ZFProperty *property,
                                                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    ZFObject *obj = property->callbackRetainGet(property, this->toObject());
    if(obj == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("embeded property %s is null while serializing \"%s\""),
            property->propertyName(),
            this->classData()->className());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("not serializable object %s while serializing \"%s\""),
            obj->objectInfoOfInstance().cString(),
            this->classData()->className());
        return zffalse;
    }
    else
    {
        const ZFClass *cls = ZFClass::classForName(propertyData.itemClass());
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                zfText("no class named %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
        else if(!cls->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                zfText("node %s is not type of %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                property->propertyClassOfRetainProperty()->className(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
    }
    return ZFCastZFObjectUnchecked(zfself *, obj)->serializeFromData(propertyData, outErrorHint, outErrorPos);
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                    ZF_IN const ZFProperty *property,
                                                                    ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && property->callbackCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    zfautoObject initValue;
    if(property->callbackIsInitValue(property, this->toObject(), &initValue))
    {
        return zftrue;
    }

    ZFObject *obj = property->callbackRetainGet(property, this->toObject());
    if(obj == zfnull || !ZFObjectIsSerializable(obj))
    {
        return zftrue;
    }

    ZFSerializable *propertyRef = zfnull;
    if(referencedOwnerOrNull != zfnull)
    {
        propertyRef = ZFCastZFObjectUnchecked(ZFSerializable *,
            property->callbackRetainGet(property, referencedOwnerOrNull->toObject()));
    }
    if(propertyRef == zfnull)
    {
        propertyRef = ZFCastZFObjectUnchecked(ZFSerializable *, initValue);
    }

    if(!ZFCastZFObjectUnchecked(zfself *, obj)->serializeToData(propertyData, outErrorHint, propertyRef))
    {
        return zffalse;
    }

    if(propertyData.elementCount() == 0
        && propertyData.attributeCount() == 0
        && propertyData.referenceRefType() == zfnull
        && propertyData.referenceRefData() == zfnull)
    {
        propertyData.itemClassSet(zfnull);
    }
    return zftrue;
}

void ZFSerializable::serializableGetInfoT(ZF_IN_OUT zfstring &ret)
{
    ZFSerializableData serializableData;
    if(!this->serializable()
       || !this->serializeToData(serializableData))
    {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->toObject()->objectInfoOfInstanceT(ret);
        ret += zfText(": not serializable");
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    else
    {
        serializableData.objectInfoT(ret);
    }
}

void ZFSerializable::serializableCopyInfoFrom(ZF_IN ZFSerializable *anotherSerializable)
{
    if(anotherSerializable == zfnull)
    {
        return ;
    }

    this->editModeWrappedClassNameSet(anotherSerializable->editModeWrappedClassName());
    if(anotherSerializable->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        this->editModeWrappedElementDatas().copyFrom(anotherSerializable->editModeWrappedElementDatas());
    }
    else
    {
        if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
        {
            this->_ZFP_ZFSerializable_editModeWrappedElementDatas->removeAll();
        }
    }

    if(anotherSerializable->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
    {
        if(this->_ZFP_ZFSerializable_referenceInfoMap == zfnull)
        {
            this->_ZFP_ZFSerializable_referenceInfoMap = zfnew(ZFCoreMap);
        }
        this->_ZFP_ZFSerializable_referenceInfoMap->removeAll();
        this->_ZFP_ZFSerializable_referenceInfoMap->copyFrom(*(anotherSerializable->_ZFP_ZFSerializable_referenceInfoMap));
    }
    else
    {
        if(this->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
        {
            zfdelete(this->_ZFP_ZFSerializable_referenceInfoMap);
            this->_ZFP_ZFSerializable_referenceInfoMap = zfnull;
        }
    }
    zfsChange(this->_ZFP_ZFSerializable_styleableType, anotherSerializable->_ZFP_ZFSerializable_styleableType);
    zfsChange(this->_ZFP_ZFSerializable_styleableData, anotherSerializable->_ZFP_ZFSerializable_styleableData);
}

void ZFSerializable::referenceInfoSet(ZF_IN const zfchar *key, ZF_IN const ZFSerializableData *referenceInfo)
{
    if(_ZFP_ZFSerializable_referenceInfoMap == zfnull)
    {
        _ZFP_ZFSerializable_referenceInfoMap = zfnew(ZFCoreMap);
    }
    if(referenceInfo == zfnull)
    {
        _ZFP_ZFSerializable_referenceInfoMap->remove(key);
    }
    else
    {
        _ZFP_ZFSerializable_referenceInfoMap->set(key,
            ZFCorePointerForObject<ZFSerializableData *>(zfnew(ZFSerializableData, *referenceInfo)));
    }
}
const ZFSerializableData *ZFSerializable::referenceInfo(ZF_IN const zfchar *key)
{
    if(_ZFP_ZFSerializable_referenceInfoMap != zfnull)
    {
        return _ZFP_ZFSerializable_referenceInfoMap->get<const ZFSerializableData *>(key);
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zftrue;
    }
    else
    {
        ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
        return (tmp != zfnull && tmp->serializable());
    }
}

// ============================================================
zfbool ZFObjectFromString(ZF_OUT zfautoObject &result,
                          ZF_IN const zfchar *encodedData,
                          ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFSerializableDataFromString(serializableData, encodedData, encodedDataLen, outErrorHint))
    {
        return zffalse;
    }
    return ZFObjectFromSerializableData(result, serializableData, outErrorHint);
}
zfautoObject ZFObjectFromString(ZF_IN const zfchar *encodedData,
                                ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromString(result, encodedData, encodedDataLen, outErrorHint);
    return result;
}

zfbool ZFObjectFromInput(ZF_OUT zfautoObject &result,
                         ZF_IN const ZFInputCallback &input,
                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFSerializableDataFromInput(serializableData, input, outErrorHint))
    {
        return zffalse;
    }
    return ZFObjectFromSerializableData(result, serializableData, outErrorHint);
}
zfautoObject ZFObjectFromInput(ZF_IN const ZFInputCallback &input,
                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromInput(result, input, outErrorHint);
    return result;
}

zfbool ZFObjectToString(ZF_OUT zfstring &encodedData,
                        ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToSerializableData(serializableData, obj, outErrorHint)) {return zffalse;}
    return ZFSerializableDataToString(encodedData, serializableData, outErrorHint);
}
zfstring ZFObjectToString(ZF_IN ZFObject *obj,
                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfstring ret;
    ZFObjectToString(ret, obj, outErrorHint);
    return ret;
}
zfbool ZFObjectToOutput(ZF_IN_OUT const ZFInputCallback &output,
                        ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToSerializableData(serializableData, obj, outErrorHint)) {return zffalse;}
    return ZFSerializableDataToOutput(output, serializableData, outErrorHint);
}

// ============================================================
zfbool ZFObjectFromSerializableData(ZF_OUT zfautoObject &result,
                                    ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    result = zfautoObjectNull;

    const zfchar *serializableClass = ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_none, serializableData, outErrorHint, outErrorPos);
    if(serializableClass == zfnull)
    {
        return zffalse;
    }
    if(zfscmpTheSame(serializableClass, ZFSerializableKeyword_null))
    {
        serializableData.resolveMark();
        return zftrue;
    }

    const ZFClass *cls = ZFClass::classForName(serializableClass);
    zfbool editModeWrapped = zffalse;
    if(cls == zfnull)
    {
        if(ZFSerializable::editMode())
        {
            ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(serializableClass);
            if(editModeData != zfnull)
            {
                editModeWrapped = zftrue;
                cls = editModeData->wrappedClass;
            }
        }
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("no class named \"%s\""),
                serializableData.itemClass());
            return zffalse;
        }
    }
    zfautoObject obj;
    {
        const ZFMethod *overridedCreateMethod = cls->methodForNameIgnoreParent(ZFSerializableKeyword_serializableNewInstance);
        if(overridedCreateMethod != zfnull)
        {
            obj = overridedCreateMethod->executeStatic<zfautoObject>();
        }
        else
        {
            obj = cls->newInstance(ZFCallerInfoMake());
        }
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj.toObject());
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("object %s not serializable"),
            ZFObjectInfoOfInstance(obj.toObject()).cString());
        return zffalse;
    }
    if(editModeWrapped)
    {
        tmp->editModeWrappedClassNameSet(serializableClass);
    }
    if(!tmp->serializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    result = obj;
    return zftrue;
}
zfautoObject ZFObjectFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                          ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromSerializableData(result, serializableData, outErrorHint, outErrorPos);
    return result;
}
zfbool ZFObjectToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN ZFObject *obj,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                  ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    if(obj == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("object %s not serializable"),
            obj->objectInfoOfInstance().cString());
        return zffalse;
    }
    return tmp->serializeToData(serializableData, outErrorHint, referencedObject);
}
ZFSerializableData ZFObjectToSerializableData(ZF_IN ZFObject *obj,
                                              ZF_OUT_OPT zfbool *outSuccess /* = zfnull */,
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                              ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    ZFSerializableData serializableData;
    zfbool success = ZFObjectToSerializableData(serializableData, obj, outErrorHint, referencedObject);
    if(outSuccess != zfnull)
    {
        *outSuccess = success;
    }
    if(success)
    {
        return serializableData;
    }
    return ZFSerializableData();
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfbool, serializable)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeFromData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeToData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedObject, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializablePropertyT, ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializableEmbededPropertyT, ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableEmbededProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfstring, serializableGetInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableCopyInfoFrom, ZFMP_IN(ZFSerializable *, anotherSerializable))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFSerializable, void, referenceInfoSet, ZFMP_IN(const zfchar *, key), ZFMP_IN(const ZFSerializableData *, referenceInfo))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, const ZFSerializableData *, referenceInfo, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, referenceInfoForSelfSet, ZFMP_IN(const ZFSerializableData *, referenceInfo))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, const ZFSerializableData *, referenceInfoForSelf)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFSerializable, void, referenceInfoForCategorySet, ZFMP_IN(const zfchar *, key), ZFMP_IN(const ZFSerializableData *, referenceInfo))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, const ZFSerializableData *, referenceInfoForCategory, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableStyleableTypeSet, ZFMP_IN(const zfchar *, styleableType))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, const zfchar *, serializableStyleableTypeGet)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableStyleableDataSet, ZFMP_IN(const zfchar *, styleableData))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, const zfchar *, serializableStyleableDataGet)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfbool, ZFObjectIsSerializable, ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromString, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfautoObject, ZFObjectFromString, ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectFromInput, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfautoObject, ZFObjectFromInput, ZFMP_IN(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectToString, ZFMP_OUT(zfstring &, encodedData), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFObjectToString, ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectToOutput, ZFMP_IN_OUT(const ZFInputCallback &, output), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfautoObject, ZFObjectFromSerializableData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromSerializableData, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectToSerializableData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedObject, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFSerializableData, ZFObjectToSerializableData, ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfbool *, outSuccess, zfnull), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedObject, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

