/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFFramework_test;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.ZFFramework.Android.ZF.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("stlport_shared");

        System.loadLibrary("ZFCore");
        System.loadLibrary("ZFAlgorithm");
        System.loadLibrary("ZFUtility");
        System.loadLibrary("ZFUIKit");
        System.loadLibrary("ZFAddition");
        System.loadLibrary("ZFUIWidget");
        System.loadLibrary("ZFLua");
        System.loadLibrary("ZFUIWebKit");

        System.loadLibrary("ZF_impl");
        System.loadLibrary("ZF_loader_impl");
        System.loadLibrary("ZFCore_impl");
        System.loadLibrary("ZFAlgorithm_impl");
        System.loadLibrary("ZFUIKit_impl");
        System.loadLibrary("ZFLua_impl");
        System.loadLibrary("ZFUIWebKit_impl");
        System.loadLibrary("ZFFramework_test");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(BuildConfig.DEBUG) {
            ZFMainEntry.debugModeSet(true);
        }
    }
    private boolean _firstTime = true;
    @Override
    protected void onResume() {
        super.onResume();
        if(_firstTime) {
            _firstTime = false;
            Intent intent = new Intent(Loader.this, ZFMainEntry.class);
            startActivity(intent);
            Loader.this.finish();
        }
    }
}

