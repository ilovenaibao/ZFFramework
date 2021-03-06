/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreEnvConfig.h
 * @brief global definition
 *
 * this file would be included by all files before any other files,
 * so put things here ONLY if necessary
 */
#ifndef _ZFI_ZFCoreEnvConfig_h_
#define _ZFI_ZFCoreEnvConfig_h_

#if 0
/*
 * to make doc with Doxygen, you should
 * specify INPUT and INCLUDE_PATH to the "zfsrc" dir of each module,
 * and:
 * ============================================================ */
FULL_PATH_NAMES        = NO
SORT_MEMBER_DOCS       = NO
MAX_INITIALIZER_LINES  = 0
RECURSIVE              = YES
HTML_FOOTER            = doc_footer.html
GENERATE_HTMLHELP      = YES
GENERATE_LATEX         = NO
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = YES
SKIP_FUNCTION_MACROS   = YES
EXPAND_ONLY_PREDEF     = YES
EXTRACT_PRIVATE        = NO
EXTRACT_LOCAL_CLASSES  = NO
EXTRACT_LOCAL_METHODS  = NO
AUTOLINK_SUPPORT       = YES
USE_MDFILE_AS_MAINPAGE = ../../../README.md

INPUT                  =
INPUT                  += ../../../README.md
INPUT                  += ../docs
INPUT                  += ../../../ZF/ZF_impl/zfsrc
INPUT                  += ../../../ZF/ZF_loader_impl/zfsrc
INPUT                  += ../../../ZF/ZFCore/zfsrc
INPUT                  += ../../../ZF/ZFCore_impl/zfsrc
INPUT                  += ../../../ZF/ZFAlgorithm/zfsrc
INPUT                  += ../../../ZF/ZFAlgorithm_impl/zfsrc
INPUT                  += ../../../ZF/ZFUtility/zfsrc
INPUT                  += ../../../ZF/ZFUIKit/zfsrc
INPUT                  += ../../../ZF/ZFUIKit_impl/zfsrc
INPUT                  += ../../../ZF/ZFAddition/zfsrc
INPUT                  += ../../../ZF/ZFUIWidget/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit_impl/zfsrc

INCLUDE_PATH           =
INCLUDE_PATH           += ../../../ZF/ZF_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZF_loader_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUtility/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFAddition/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWidget/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit_impl/zfsrc

PREDEFINED            += ZF_DOXYGEN_FLAG
PREDEFINED            += __declspec(x):=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_BEGIN:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_END:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_USE:=
PREDEFINED            += _ZFP_ZFIMPLEMENTS_DECLARE(...):=
PREDEFINED            += ZFIMPLEMENTS_DECLARE(...):=

EXPAND_AS_DEFINED     += ZF_NAMESPACE_BEGIN
EXPAND_AS_DEFINED     += ZF_NAMESPACE_END
EXPAND_AS_DEFINED     += ZF_NAMESPACE_USE
EXPAND_AS_DEFINED     += ZF_IN
EXPAND_AS_DEFINED     += ZF_IN_OPT
EXPAND_AS_DEFINED     += ZF_OUT
EXPAND_AS_DEFINED     += ZF_OUT_OPT
EXPAND_AS_DEFINED     += ZF_IN_OUT
EXPAND_AS_DEFINED     += ZF_IN_OUT_OPT
EXPAND_AS_DEFINED     += ZFT_INT_WEAK
EXPAND_AS_DEFINED     += ZFT_INT_STRONG
EXPAND_AS_DEFINED     += ZFT_INT_STRONG_WITH_BIT
EXPAND_AS_DEFINED     += ZFT_INT_STRONG_DETAIL
EXPAND_AS_DEFINED     += zfclass
EXPAND_AS_DEFINED     += zfclassFwd
EXPAND_AS_DEFINED     += zfclassPOD
EXPAND_AS_DEFINED     += zfclassLikePOD
EXPAND_AS_DEFINED     += zfclassNotPOD
EXPAND_AS_DEFINED     += zfabstract
EXPAND_AS_DEFINED     += zfextends
EXPAND_AS_DEFINED     += zfextendsPOD
EXPAND_AS_DEFINED     += zfextendsLikePOD
EXPAND_AS_DEFINED     += zfextendsNotPOD
EXPAND_AS_DEFINED     += zfoverride
EXPAND_AS_DEFINED     += zffinal
EXPAND_AS_DEFINED     += zfinterface
EXPAND_AS_DEFINED     += zfimplements
EXPAND_AS_DEFINED     += ZFCLASS_DISALLOW_COPY_CONSTRUCTOR
EXPAND_AS_DEFINED     += ZFCORETYPE_STRING_CONVERTER_DECLARE
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE_ALIAS
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT
EXPAND_AS_DEFINED     += ZFCORE_POD_COMPARER_DECLARE
EXPAND_AS_DEFINED     += ZFCOREPOINTER_DECLARE
EXPAND_AS_DEFINED     += ZFM_CLASS_HAS_MEMBER_DECLARE
EXPAND_AS_DEFINED     += ZFFILTER_DECLARE
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_OPTIONAL
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_CLASS
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_BEGIN
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_END
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_ABSTRACT
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_ABSTRACT_ALLOW_CUSTOM_CONSTRUCTOR
EXPAND_AS_DEFINED     += _ZFP_ZFINTERFACE_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_INIT_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_INIT_DEFINE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_DEALLOC_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_DEALLOC_DEFINE
EXPAND_AS_DEFINED     += ZFAUTOOBJECT_DECLARE
EXPAND_AS_DEFINED     += ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO
EXPAND_AS_DEFINED     += ZFIDMAP
EXPAND_AS_DEFINED     += ZFIDMAP_DETAIL
EXPAND_AS_DEFINED     += ZFIDMAP_GLOBAL
EXPAND_AS_DEFINED     += ZFIDMAP_GLOBAL_DETAIL
EXPAND_AS_DEFINED     += ZFOBSERVER_EVENT
EXPAND_AS_DEFINED     += ZFOBSERVER_EVENT_GLOBAL
EXPAND_AS_DEFINED     += ZFOBSERVER_EVENT_GLOBAL_WITH_NAMESPACE
EXPAND_AS_DEFINED     += ZFCLASS_SINGLETON_DECLARE
EXPAND_AS_DEFINED     += ZFCLASS_SINGLETON_DECLARE_DETAIL
EXPAND_AS_DEFINED     += ZFOBJECT_SINGLETON_DECLARE
EXPAND_AS_DEFINED     += ZFOBJECT_SINGLETON_DECLARE_DETAIL
EXPAND_AS_DEFINED     += _ZFP_ZFMETHOD_INVOKER
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_8
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_STATIC_8
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_8
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_NO_AUTOREGISTER_8
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_0
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_0
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_1
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_1
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_2
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_2
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_3
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_3
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_4
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_4
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_5
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_5
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_6
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_6
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_7
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_7
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_8
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_DETAIL_8
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE_BEGIN
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE_END
EXPAND_AS_DEFINED     += _ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS
EXPAND_AS_DEFINED     += ZFCALLBACK_LOCAL_BEGIN
EXPAND_AS_DEFINED     += ZFCALLBACK_LOCAL_END
EXPAND_AS_DEFINED     += _ZFP_ZFCALLBACK_INVOKER
EXPAND_AS_DEFINED     += ZFLISTENER_DECLARE
EXPAND_AS_DEFINED     += ZFLISTENER_OVERRIDE_DECLARE
EXPAND_AS_DEFINED     += ZFLISTENER_PROTOTYPE_EXPAND
EXPAND_AS_DEFINED     += ZFLISTENER_LOCAL_BEGIN
EXPAND_AS_DEFINED     += ZFLISTENER_LOCAL_END
EXPAND_AS_DEFINED     += ZFCORE_PARAM_DECLARE_SELF
EXPAND_AS_DEFINED     += ZFCORE_PARAM
EXPAND_AS_DEFINED     += ZFCORE_PARAM_WITH_INIT
EXPAND_AS_DEFINED     += ZFCORE_PARAM_RETAIN
EXPAND_AS_DEFINED     += ZFCORE_PARAM_RETAIN_WITH_INIT
EXPAND_AS_DEFINED     += _ZFP_ZFCORE_PARAM_RETAIN_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_WITH_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_READONLY
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_NOT_SERIALIZABLE
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_NOT_SERIALIZABLE_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_WITH_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_READONLY
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_MARK_AUTO_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_INIT_VALUE_CHECKER_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_VALUE_COMPARER_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_INIT_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_DEALLOC_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_VERIFY_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_ATTACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_DETACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DEALLOC_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_UPDATE_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_TYPE_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_TYPE_ACCESS_ONLY_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_TYPE_ALIAS_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_TYPE_DISABLE
EXPAND_AS_DEFINED     += ZFSTYLE_DEFAULT_DECLARE
EXPAND_AS_DEFINED     += ZFENUM_BEGIN
EXPAND_AS_DEFINED     += ZFENUM_VALUE
EXPAND_AS_DEFINED     += ZFENUM_VALUE_WITH_INIT
EXPAND_AS_DEFINED     += ZFENUM_SEPARATOR
EXPAND_AS_DEFINED     += ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE
EXPAND_AS_DEFINED     += ZFENUM_VALUE_REGISTER
EXPAND_AS_DEFINED     += ZFENUM_VALUE_REGISTER_WITH_NAME
EXPAND_AS_DEFINED     += ZFENUM_END
EXPAND_AS_DEFINED     += ZFENUM_END_WITH_DEFAULT
EXPAND_AS_DEFINED     += ZFENUM_FLAGS_DECLARE
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE_TEMPLATE
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_READONLY_DECLARE
EXPAND_AS_DEFINED     += _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
EXPAND_AS_DEFINED     += _ZFP_ZFValue_method_DECLARE
EXPAND_AS_DEFINED     += _ZFP_ZFValueEditable_method_DECLARE
EXPAND_AS_DEFINED     += ZFLANGAPPLY_IMPL
EXPAND_AS_DEFINED     += ZFSKINAPPLY_IMPL
EXPAND_AS_DEFINED     += ZFTIMELINEPROPERTY_DECLARE_BY_VALUE
EXPAND_AS_DEFINED     += ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL
EXPAND_AS_DEFINED     += ZFTIMELINEPROPERTY_DECLARE_CUSTOM

EXCLUDE_PATTERNS      += */*.java
EXCLUDE_PATTERNS      += */_repo/*
EXCLUDE_PATTERNS      += */_tmp/*

EXCLUDE_SYMBOLS       += _ZFP_*
EXCLUDE_SYMBOLS       += _ZFI_*
EXCLUDE_SYMBOLS       += _ZFT_*
EXCLUDE_SYMBOLS       += *zfstl_impl*
/* ============================================================ */
#endif

// ignore MSVC warnings
#if defined(_MSC_VER)
    // reference type warnings
    #pragma warning(disable:4180)

    // placement new for POD with no-param-constructor:
    //   new addr Type();
    #pragma warning(disable:4345)

    // array member init
    #pragma warning(disable:4351)

    // unsafe APIs
    #pragma warning(disable:4996)
#endif

#endif // #ifndef _ZFI_ZFCoreEnvConfig_h_

