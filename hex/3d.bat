rem 作者：Wellin Boss
rem 链接：https://www.zhihu.com/question/275611095/answer/383832974
rem 来源：知乎
rem 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

chcp 437
if "%1"=="" (
  for %%a in (  FontSize:00080008
                FontFamily:00000030
                WindowSize:003d0050
                ScreenColors:0000000f
                CodePage:000001b5
                ScreenBufferSize:003d0050
  ) do for /f "tokens=1,2 delims=:" %%b in ("%%a") do (
    >nul reg add HKCU\Console\DosBatch_3D_Engine /v %%b /t reg_dword /d 0x%%c /f
  )
  start "DosBatch_3D_Engine" /max "%ComSpec%" /c "%~s0" 1&goto:eof

) else ( >nul reg delete HKCU\Console\DosBatch_3D_Engine /f )


@echo off & setlocal EnableDelayedExpansion & title Sphere 3D & set /a cols=62, lines=58 & goto :Init_system


    :: By einstein1969. Dedicated to jeb, dbenham, penpen, carlos, aGerman, Aacini, EdDyreen, 
    :: npocmaka_, Sponge Belly, Magialisk, the users and the staff of Dostips forum.

    :: Use raster font 8x8

    :: v 0.0.2 passed from 16.6 FPS to ~20.0 FPS

    :: Variables enigma:

    :: ?   =   Sphere radius
    :: rt   =   Rotation angle
    :: ds   =   Distance Z of sphere
    :: _dx,_dy   =   Offset x,y of projecting coordinates in 2D plane.
    :: a,b   =   Sine, cosine of rotation angle
    :: ax,ay,az   =   Rotation coordinate 3D, now a, b, c
    :: c,d   =   X,Y coordinate of projecting 3D to 2D, now a,b
    :: e   =   Z coordinate, now c

:Main

set /a ?=100, rt=0, ds=0, _dx=cols/2, _dy=lines/2, _dz=4000, _1=10000

:loop

(setlocal

  for /F "Tokens=1 delims==" %%v in ('set _') do set "%%v="
  set lines=
  set cols=

  rem giro completo (120)*31416/60 o multipli di 120
  for /L %%\ in (0,1,1199) do (

   set /a "rt+=31416/60"

   if !ds! lss 1000 (set /a ds+=10) else %_FPS%

   setlocal

   set /a "a=(15708-rt) %% 62832, c=(a>>31|1)*a"

   if !c! gtr 47124 (set /a "a=a-(a>>31|1)*62832, b=%_SIN%, a=rt %% 62832, c=(a>>31|1)*a")  else (if !c! gtr 15708 (set /a "a=(a>>31|1)*31416-a, b=%_SIN%, a=rt %% 62832, c=(a>>31|1)*a") else set /a "b=%_SIN%, a=rt %% 62832, c=(a>>31|1)*a")
   if !c! gtr 47124 (set /a "a=a-(a>>31|1)*62832, a=%_SIN%")  else (if !c! gtr 15708 (set /a "a=(a>>31|1)*31416-a, a=%_SIN%") else set /a "a=%_SIN%")

   For /f "tokens=1,2" %%a in ("!a! !b!") do for %%f in ("0 9999" "-5000 8661" "-8661 5000" "-9999 0" "-8661 -5000" "-5000 -8661" "0 -9999" "5000 -8661" "8661 -5000" "9999 0" "8661 5000" "5000 8661") do for /f "tokens=1,2" %%g in (%%f) do (

   for %%t in ("3827 9239" "7071 7071" "9239 3827" "9999 0" "9239 -3827" "7071 -7071" "3827 -9239") do for /f "tokens=1,2" %%u in (%%t) do (

     set /a "a=?*%%h/10000*%%u/10000, c=?*%%g/10000*%%u/10000, b=?*%%v/10000, a=(a*%%b/10000-(b*%%a+c*%%b)/10000*%%a/10000)*%%b/10000-(b*%%b-c*%%a)/10000*%%a/10000, b=(?*%%h/10000*%%u/10000*%%b/10000-(b*%%a+c*%%b)/10000*%%a/10000)*%%a/10000+(b*%%b-c*%%a)/10000*%%b/10000, c=?*%%h/10000*%%u/10000*%%a/10000+(?*%%v/10000*%%a+c*%%b)/10000*%%b/10000, a=ds*a/(c-%_dz%)+%_dx%, b=ds*b/(c-%_dz%)+%_dy%"

     if not defined L!b! set L!b!=%_empty%

     if !c! lss 0 (%_$PLOT_% !a! !b! 1 %_PLOT$_%) else %_$PLOT_% !a! !b! 2 %_PLOT$_%

    )   
   )

   if not "!OT!"=="!time:~-1!" (
     cls & (For /L %%l in (1,1,%lines%) do if not defined L%%l (echo() else echo( !L%%l!)>CON
     endlocal
     set OT=!time:~-1!
   ) else endlocal
  )

endlocal)

set /a ds=1000, rt=1200*31416/60

goto :loop

:Init_system

  set /a cc=cols+2, ll=lines+2
  mode %cc%,%ll% & cls

(  
  for /F "Tokens=1 delims==" %%v in ('set') do set "%%v="
  set /a cols=%cols%, lines=%lines%
)

  setlocal DisableDelayedExpansion

  set "_$PLOT_=For /F usebackq^ tokens^=1-3 %%x in ('"
  set "_PLOT$_=') do set c=!L%%y:~%%x!& set L%%y=!L%%y:~0,%%x!!g:~%%z,1!!c:~1!"

  set "_SIN=(a-a*a/1920*a/312500+a*a/1920*a/15625*a/15625*a/2560000-a*a/1875*a/15360*a/15625*a/15625*a/16000*a/44800000)"

  set _FPS=if   %%\  geq !_FPS_NC! (set /a "_FPS_NC=1!time:~-5,-3!!time:~-2!, _dt_F=_FPS_NC-t0_F, t0_F=_FPS_NC, mFPS=_dt_F-1>>31, _FPS_NC=_dt_F/(_dt_F+(mFPS&1)), _dt_F=(%%\-oFPS)*10000/(_dt_F+(mFPS&6000)), _FPS_NC=%%\+8*_dt_F/100+(1-_FPS_NC)*100, oFPS=%%\, mFPS=1000000000/(_dt_F+(_dt_F-1>>31&1))" ^&title Sphere 3D - FPS=!_dt_F:~0,-2!.!_dt_F:~-2! [!mFPS:~0,-1!.!mFPS:~-1! ?s] [#%%\]^& set mFPS=^& set _dt_F=)

  setlocal EnableDelayedExpansion

  For /L %%l in (1,1,%cols%) do set "_empty=!_empty! "

  set g= .@"

Goto :Main