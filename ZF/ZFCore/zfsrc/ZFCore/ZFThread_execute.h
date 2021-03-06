/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFThread_execute.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_execute_h_
#define _ZFI_ZFThread_execute_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief execute in main thread
 *
 * automatically retain runnable's owner and params, and release them after finish\n
 * runnable won't be executed immediately even if current thread is main thread,
 * it's queued instead,
 * unless you call this function in main thread and set waitUntilDone to zftrue\n
 * \n
 * note, sender would be retained until execute finish,
 * while owner won't
 * @see ZFThreadExecuteInNewThread, ZFThreadExecuteCancel
 */
extern ZF_ENV_EXPORT zfidentity ZFThreadExecuteInMainThread(ZF_IN const ZFListener &runnable,
                                                            ZF_IN_OPT ZFObject *userData = zfnull,
                                                            ZF_IN_OPT const ZFListenerData &listenerData = ZFListenerData(),
                                                            ZF_IN_OPT ZFObject *owner = zfnull,
                                                            ZF_IN_OPT zfbool waitUntilDone = zffalse);
/**
 * @brief util method for #ZFThreadExecuteInMainThread
 */
inline zfidentity ZFThreadExecuteInMainThreadWaitUntilDone(ZF_IN const ZFListener &runnable,
                                                           ZF_IN_OPT ZFObject *userData = zfnull,
                                                           ZF_IN_OPT const ZFListenerData &listenerData = ZFListenerData(),
                                                           ZF_IN_OPT ZFObject *owner = zfnull)
{
    return ZFThreadExecuteInMainThread(runnable, userData, listenerData, owner, zftrue);
}

/**
 * @brief execute in new thread
 *
 * automatically retain runnable's owner and params, and release them after finish
 * @see ZFThreadExecuteInMainThread, ZFThreadExecuteCancel
 * @note always try this method first to achieve thread processing,
 *   instead of create new ZFThread instance,
 *   since we may have thread pool for performance
 */
extern ZF_ENV_EXPORT zfidentity ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                                           ZF_IN_OPT ZFObject *userData = zfnull,
                                                           ZF_IN_OPT const ZFListenerData &listenerData = ZFListenerData(),
                                                           ZF_IN_OPT ZFObject *owner = zfnull);

/**
 * @brief exeute in main thread after delay, directly schedule an #ZFThreadExecuteInMainThread if (delay <= 0)
 */
extern ZF_ENV_EXPORT zfidentity ZFThreadExecuteInMainThreadAfterDelay(ZF_IN zftimet delay,
                                                                      ZF_IN const ZFListener &runnable,
                                                                      ZF_IN_OPT ZFObject *userData = zfnull,
                                                                      ZF_IN_OPT const ZFListenerData &listenerData = ZFListenerData(),
                                                                      ZF_IN_OPT ZFObject *owner = zfnull);

/**
 * @brief cancel execute of task
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteCancel(ZF_IN zfidentity taskId);

/**
 * @brief cancel all execute task of runnable, ignoring the params, see #ZFThreadExecuteCancel
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteCancel(ZF_IN const ZFListener &runnable);

/**
 * @brief cancel all execute task of owner, see #ZFThreadExecuteCancel
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteCancelByOwner(ZF_IN ZFObject *owner);

/**
 * @brief wait thread task until done
 *
 * return immediately if the task is running in main thread
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteWait(ZF_IN zfidentity taskId);
/**
 * @brief wait thread task until done
 *
 * return ture if wait success,
 * or false if wait timeout or no such task\n
 * return false immediately if the task is running in main thread
 */
extern ZF_ENV_EXPORT zfbool ZFThreadExecuteWait(ZF_IN zfidentity taskId,
                                                ZF_IN const zftimet &miliSecs);

/**
 * @brief attach observer to the tasks started by ZFThreadExecuteXxx, see #ZFObject::observerNotify
 *
 * do nothing if task not exist, auto remove after task finished \n
 * \n
 * list of supported events:
 * -  #ZFThread::EventThreadOnStart
 * -  #ZFThread::EventThreadOnStop
 * -  #ZFThread::EventThreadOnCancel
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteObserverAdd(ZF_IN zfidentity taskId,
                                                     ZF_IN const zfidentity &eventId,
                                                     ZF_IN const ZFListener &callback);
/**
 * @brief see #ZFThreadExecuteObserverAdd
 */
extern ZF_ENV_EXPORT void ZFThreadExecuteObserverRemove(ZF_IN zfidentity taskId,
                                                        ZF_IN const zfidentity &eventId,
                                                        ZF_IN const ZFListener &callback);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_execute_h_

