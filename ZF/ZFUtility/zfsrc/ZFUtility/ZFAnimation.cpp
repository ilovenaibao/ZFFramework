/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnimation.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFAnimationAniList : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFAnimationAniList, ZFObject)
public:
    ZFCoreArrayPOD<ZFAnimation *> aniList;
};

// ============================================================
// _ZFP_ZFAnimationPrivate
zfclassNotPOD _ZFP_ZFAnimationPrivate
{
public:
    ZFObject *aniTarget;
    zfbool aniRunning;
    zfbool aniDelaying;
    zfidentity aniDelayTaskId;
    zfidentity aniDelayThreadId;
    zfidentity aniDummyThreadId;
    zfbool aniStopCalled;
    zfbool aniStoppedByUser;
    ZFIdentityGenerator aniIdGenerator;

public:
    _ZFP_ZFAnimationPrivate(void)
    : aniTarget(zfnull)
    , aniRunning(zffalse)
    , aniDelaying(zffalse)
    , aniDelayTaskId(zfidentityInvalid)
    , aniDelayThreadId(zfidentityInvalid)
    , aniDummyThreadId(zfidentityInvalid)
    , aniStopCalled(zffalse)
    , aniStoppedByUser(zffalse)
    , aniIdGenerator()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAnimationTaskHolder, ZFLevelZFFrameworkEssential)
{
    this->delayOnFinishListener = ZFCallbackForRawFunction(zfself::delayOnFinish);
    this->dummyOnFinishListener = ZFCallbackForRawFunction(zfself::dummyOnFinish);
}
public:
    ZFListener delayOnFinishListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(delayOnFinish)
    {
        ZFAnimation *ani = userData->to<ZFObjectHolder *>()->holdedObj;
        ZFValue *taskId = listenerData.param0->to<ZFValue *>();
        ani->_ZFP_ZFAnimation_aniImplDelayNotifyFinish(taskId->identityValue());
    }
    ZFListener dummyOnFinishListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(dummyOnFinish)
    {
        ZFAnimation *ani = userData->to<ZFObjectHolder *>()->holdedObj;
        ani->_ZFP_ZFAnimation_aniDummyNotifyStop();
    }
ZF_GLOBAL_INITIALIZER_END(ZFAnimationTaskHolder)

// ============================================================
ZFOBJECT_REGISTER(ZFAnimation)

ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnInvalid)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnDelayFinish)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStopOrOnInvalid)

ZFObject *ZFAnimation::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationPrivate);
    return this;
}
void ZFAnimation::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimation::objectOnDeallocPrepare(void)
{
    this->aniStop();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAnimation::objectHash(void)
{
    return (zfidentity)(zft_zfidentity)this->aniDuration();
}
ZFCompareResult ZFAnimation::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->aniDuration() == another->aniDuration())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

void ZFAnimation::aniTargetSet(ZF_IN ZFObject *aniTarget)
{
    zfCoreAssertWithMessage(!d->aniRunning, zfTextA("change animation's target while animation is running"));
    d->aniTarget = aniTarget;
}
ZFObject *ZFAnimation::aniTarget(void)
{
    return d->aniTarget;
}

void ZFAnimation::aniStart(void)
{
    this->_ZFP_ZFAnimation_aniReadyStart();

    d->aniStoppedByUser = zffalse;
    if(!this->aniValid())
    {
        this->aniOnInvalid();
        this->aniOnStopOrOnInvalid(zffalse);
        return ;
    }

    zfRetain(this);
    zfRetain(this->aniTarget());

    d->aniRunning = zftrue;
    d->aniStopCalled = zffalse;
    d->aniIdGenerator.next();
    this->aniOnStart();

    if(this->aniDelay() > 0)
    {
        d->aniDelaying = zftrue;
        this->aniImplDelay();
    }
    else
    {
        d->aniDelaying = zffalse;
        this->aniImplStart();
    }
}
zfbool ZFAnimation::aniRunning(void)
{
    return d->aniRunning;
}
void ZFAnimation::aniStop(void)
{
    if(!(d->aniRunning) || d->aniStopCalled)
    {
        return ;
    }
    d->aniStopCalled = zftrue;
    d->aniStoppedByUser = zftrue;
    d->aniIdGenerator.next();
    if(d->aniDelaying)
    {
        d->aniDelaying = zffalse;
        this->aniImplDelayCancel();
    }
    else
    {
        this->aniImplStop();
    }
    this->aniImplNotifyStop();
}
zfbool ZFAnimation::aniStoppedByUser(void)
{
    return d->aniStoppedByUser;
}

zfidentity ZFAnimation::aniId(void)
{
    return d->aniIdGenerator.current();
}

zfbool ZFAnimation::aniValid(void)
{
    return this->aniImplCheckValid();
}
zfbool ZFAnimation::aniImplCheckValid(void)
{
    return (this->aniDuration() > 0);
}

void ZFAnimation::_ZFP_ZFAnimation_aniImplDelayNotifyFinish(ZF_IN zfidentity taskId)
{
    if(taskId == d->aniDelayTaskId)
    {
        this->aniImplDelayNotifyFinish();
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniReadyStart(void)
{
    if(this->aniTarget() != zfnull)
    {
        _ZFP_I_ZFAnimationAniList *aniList = this->aniTarget()->tagGet<_ZFP_I_ZFAnimationAniList *>(_ZFP_I_ZFAnimationAniList::ClassData()->className());
        if(aniList == zfnull)
        {
            aniList = zfAllocWithoutLeakTest(_ZFP_I_ZFAnimationAniList);
            this->aniTarget()->tagSet(_ZFP_I_ZFAnimationAniList::ClassData()->className(), aniList);
            zfReleaseWithoutLeakTest(aniList);
        }
        if(this->aniAutoStopPrev())
        {
            while(!aniList->aniList.isEmpty())
            {
                aniList->aniList.getFirst()->aniStop();
            }
        }
        aniList->aniList.add(this);
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniReadyStop(void)
{
    if(this->aniTarget() != zfnull)
    {
        _ZFP_I_ZFAnimationAniList *aniList = this->aniTarget()->tagGet<_ZFP_I_ZFAnimationAniList *>(_ZFP_I_ZFAnimationAniList::ClassData()->className());
        if(aniList != zfnull)
        {
            aniList->aniList.removeElement(this);
        }
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniDummyNotifyStop(void)
{
    d->aniDummyThreadId = zfidentityInvalid;
    this->aniImplNotifyStop();
}
void ZFAnimation::aniImplDelay(void)
{
    ++(d->aniDelayTaskId);
    d->aniDelayThreadId = ZFThreadExecuteInMainThreadAfterDelay(
        this->aniDelay(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAnimationTaskHolder)->delayOnFinishListener,
        this->objectHolder(),
        ZFListenerData().param0Set(ZFValue::identityValueCreate(d->aniDelayTaskId).toObject())
        );
}
void ZFAnimation::aniImplDelayCancel(void)
{
    ++(d->aniDelayTaskId);
    ZFThreadExecuteCancel(d->aniDelayThreadId);
}
void ZFAnimation::aniImplDelayNotifyFinish(void)
{
    zfCoreAssertWithMessage(d->aniDelaying, zfTextA("notify delay finish an animation which not delaying"));
    d->aniDelaying = zffalse;
    this->aniImplStart();
}

void ZFAnimation::aniImplStart(void)
{
    // start a dummy animation if not implemented
    if(this->classData() == ZFAnimation::ClassData())
    {
        d->aniDummyThreadId = ZFThreadExecuteInMainThreadAfterDelay(
            this->aniDuration(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAnimationTaskHolder)->dummyOnFinishListener,
            this->objectHolder());
    }
}
void ZFAnimation::aniImplStop(void)
{
    if(this->classData() == ZFAnimation::ClassData())
    {
        ZFThreadExecuteCancel(d->aniDummyThreadId);
    }
}

void ZFAnimation::aniImplNotifyStop(void)
{
    zfCoreAssertWithMessage(d->aniRunning, zfTextA("notify stop an animation which not started"));
    this->_ZFP_ZFAnimation_aniReadyStop();
    ZFObject *aniTargetToRelease = this->aniTarget();

    d->aniRunning = zffalse;
    d->aniStopCalled = zffalse;
    this->aniOnStop();
    this->aniOnStopOrOnInvalid(zftrue);

    zfRelease(aniTargetToRelease);
    zfRelease(this);
}

ZF_NAMESPACE_GLOBAL_END

