/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSet.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFSet_h_
#define _ZFI_ZFSet_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFMapEditable;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZF_ENV_EXPORT ZFSet : zfextends ZFContainer
{
    ZFOBJECT_DECLARE(ZFSet, ZFContainer)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->removeAll();
        this->addFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

public:
    /** @brief see #ZFObject::objectOnInit */
    virtual ZFObject *objectOnInit(ZF_IN ZFContainer *another);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief return number of content
     */
    virtual zfindex count(void);

    /**
     * @brief return true if empty or false if not empty
     */
    virtual zfbool isEmpty(void);

    /**
     * @brief return true if contains the object,
     *   compared by #ZFObject::objectCompare
     */
    virtual zfbool isContain(ZF_IN ZFObject *obj);

protected:
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    virtual void add(ZF_IN ZFObject *obj);
    /**
     * @brief add objects from another container
     */
    virtual void addFrom(ZF_IN ZFContainer *another);

    /**
     * @brief remove object, do nothing if not exist
     */
    virtual void removeElement(ZF_IN ZFObject *obj);
    /**
     * @brief remove all object
     */
    virtual void removeAll(void);

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    virtual zfiterator iterator(void);

    /** @brief see #zfiterator */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *value);

    /** @brief see #zfiterator */
    virtual zfbool iteratorIsValid(ZF_IN const zfiterator &it);
    /** @brief see #zfiterator */
    virtual zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorGet(ZF_IN const zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorNext(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorPrev(ZF_IN_OUT zfiterator &it);

protected:
    /** @brief see #zfiterator */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid);

private:
    ZFMapEditable *d;
};

// ============================================================
/**
 * @brief editable container of ZFObject
 */
zfclass ZF_ENV_EXPORT ZFSetEditable : zfextends ZFSet, zfimplements ZFIterableEditable
{
    ZFOBJECT_DECLARE(ZFSetEditable, ZFSet)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable)

public:
    zfoverride
    virtual void add(ZF_IN ZFObject *obj)
    {
        zfsuper::add(obj);
    }
    zfoverride
    virtual void addFrom(ZF_IN ZFContainer *another)
    {
        zfsuper::addFrom(another);
    }

    zfoverride
    virtual void removeElement(ZF_IN ZFObject *obj)
    {
        zfsuper::removeElement(obj);
    }
    zfoverride
    virtual void removeAll(void)
    {
        zfsuper::removeAll();
    }

public:
    /** @brief see #zfiterator */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value)
    {
        zfsuper::iteratorSet(it, value);
    }
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it)
    {
        zfsuper::iteratorRemove(it);
    }

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid)
    {
        zfsuper::iteratorAdd(value, it);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSet_h_

