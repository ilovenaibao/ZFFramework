# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

LOCAL_PATH := $(call my-dir)

ifeq ($(OS),Windows_NT)
    ZF_ROOT_PATH = ..\..\..\..\..\ZFFramework
    $(info $(shell call ..\..\..\ZFSetup.bat))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\release\release_Android.bat))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\common\zfsh_if_exist.bat ..\..\..\zfscript\zfmodule_setup.zfsh Android))

    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat ..\..\..\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_fix.bat assets\zfres >nul 2>&1))

    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFCore . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAlgorithm . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUtility . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIKit . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAddition . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIWidget . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFLua . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIWebKit . >nul 2>&1))

    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZF_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZF_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZF_loader_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZF_loader_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFCore_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZFCore_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAlgorithm_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZFAlgorithm_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIKit_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZFUIKit_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFLua_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZFLua_impl.jar >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIWebKit_impl . >nul 2>&1))
    $(info $(shell del /f/q .\libs\ZFUIWebKit_impl.jar >nul 2>&1))
else
    ZF_ROOT_PATH = ../../../../../ZFFramework
    $(info $(shell sh ../../../ZFSetup.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/release/release_Android.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/common/zfsh_if_exist.sh ../../../zfscript/zfmodule_setup.zfsh Android))

    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh ../../../zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_fix.sh assets/zfres >/dev/null 2>&1))

    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFCore/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAlgorithm/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUtility/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIKit/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAddition/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIWidget/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFLua/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIWebKit/. ./ >/dev/null 2>&1))

    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZF_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZF_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZF_loader_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZF_loader_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFCore_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZFCore_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAlgorithm_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZFAlgorithm_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIKit_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZFUIKit_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFLua_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZFLua_impl.jar >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIWebKit_impl/. ./ >/dev/null 2>&1))
    $(info $(shell rm -rf ./libs/ZFUIWebKit_impl.jar >/dev/null 2>&1))
endif

include $(call all-subdir-makefiles)

