@echo off
if "%CFG%"=="" set CFG=ALL
msdev winx/step001/hello.dsw /MAKE "ALL - %CFG%"
msdev winx/step002-bkgnd/bkgnd.dsw /MAKE "ALL - %CFG%"
msdev winx/step003-command/command.dsw /MAKE "ALL - %CFG%"
msdev winx/step004-user-ctrl/user-ctrl.dsw /MAKE "ALL - %CFG%"
msdev winx/step005-accel/accel.dsw /MAKE "ALL - %CFG%"
msdev winx/step006-layout/layout.dsw /MAKE "ALL - %CFG%"
msdev winx/step007-icon/icon.dsw /MAKE "ALL - %CFG%"
msdev winx/step008-openfiles/openfiles.dsw /MAKE "ALL - %CFG%"
msdev winx/step009-mousewheel/mousewheel.dsw /MAKE "ALL - %CFG%"
msdev winx/step010-xptheme/xpstyle.dsw /MAKE "ALL - %CFG%"
msdev winx/step011-linkctrl/linkctrl.dsw /MAKE "ALL - %CFG%"
msdev winx/step012-activex/activex.dsw /MAKE "ALL - %CFG%"
msdev winx/step013-autocomplete/autocomplete.dsw /MAKE "ALL - %CFG%"
msdev winx/step014-behavior/behavior.dsw /MAKE "ALL - %CFG%"
msdev winx/step015-property-sheet/property-sheet.dsw /MAKE "ALL - %CFG%"
msdev winx/step016-mfc-together/mfc-together.dsw /MAKE "ALL - %CFG%"
msdev winx/step017-ddx/ddx.dsw /MAKE "ALL - %CFG%"
msdev winx/step018-xslt/xslt.dsw /MAKE "ALL - %CFG%"
msdev winx/step019-scroll-window/scrollw.dsw /MAKE "ALL - %CFG%"
msdev winx/step020-xp-menu/xp-menu.dsw /MAKE "ALL - %CFG%"
msdev winx/step021-api-hook/api-hook.dsw /MAKE "ALL - %CFG%"
msdev winx/step022-modeless/modeless.dsw /MAKE "ALL - %CFG%"
msdev opencv/histgram.dsw /MAKE "ALL - %CFG%"
msdev gdiplus/step001-save-image/save-image.dsw /MAKE "ALL - %CFG%"
pause