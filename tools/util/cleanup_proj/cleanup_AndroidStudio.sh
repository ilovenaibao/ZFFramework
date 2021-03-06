#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x-$PROJ_ROOT" = "x-" || test "x-$PROJ_NAME" = "x-" ; then
    echo usage:
    echo   cleanup_AndroidStudio.sh PROJ_ROOT PROJ_NAME
    exit 1
fi

rm -rf "$PROJ_ROOT/.gradle" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/.idea" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/build" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/local.properties" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/*.iml" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/lib/.externalNativeBuild" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/lib/build" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/lib/*.iml" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/app/.externalNativeBuild" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/app/build" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/app/*.iml" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/app/src/main/assets/zfres" >/dev/null 2>&1

exit 0

