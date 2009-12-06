@echo off

IF [%1]==[] GOTO PrintUsage
IF [%2]==[] GOTO PrintUsage
IF NOT EXIST %1\levels\%2\%2.mef GOTO BadLevel

cd %1\levels\%2
set PATH=%PATH%;..\..\..\..\build\bin\engine\tools

mef2input %2.mef %2.bru %2.dsf %2.obs %2.lum
hdivide %2.bru %2.rbr %2.cbr %2.dgm %2.hgm %2.sbr

REM #################
REM Rendering Section
REM #################

hcsg -r %2.rbr %2.rg1
hbsp -r %2.rg1 %2.hgm %2.rt1
hportal -r %2.rt1 %2.rp1
hflood -r %2.rt1 %2.rp1 %2.rg2
hbsp -r %2.rg2 %2.hgm %2.rt2
hportal -r %2.rt2 %2.rp2
hvis %2.rp2 %2.vis
hdetail %2.rt2 %2.dgm %2.rt3

REM #################
REM Collision Section
REM #################

hexpand %2.dsf %2.cbr

FOR %%f IN (*.ebr) DO (
hcsg -c %%f %%~nf.cg1
hbsp -c %%~nf.cg1 nohints %%~nf.ct1
hportal -c %%~nf.ct1 %%~nf.cp
hflood -c %%~nf.ct1 %%~nf.cp %%~nf.cg2
hbsp -c %%~nf.cg2 nohints %%~nf.ct2
)

SETLOCAL ENABLEDELAYEDEXPANSION
set GEOMTREEPAIRS=
FOR %%f IN (*.cg2) DO (
set GEOMTREEPAIRS=!GEOMTREEPAIRS! %%f %%~nf.ct2
)
hobsp -c%GEOMTREEPAIRS% %2.ot
set GEOMTREEPAIRS=
ENDLOCAL

hoportal -c %2.ot %2.op
hnav %2.dsf %2.ot %2.nav

REM #################
REM Collation Section
REM #################

hcollate -L %2.rt3 %2.rp2 %2.vis %2.ot %2.op %2.nav %2.dsf %2.obs %2.bsp

IF [%3]==[noclean] GOTO ResetDirectory

REM #######################
REM General Cleanup Section
REM #######################

del *.bru
del *.dsf
del *.sbr

REM #########################
REM Cleanup Rendering Section
REM #########################

del *.dgm
del *.hgm
del *.lum
del *.rbr
del *.rg*
del *.rp*
del *.rt*
del *.vis

REM #########################
REM Cleanup Collision Section
REM #########################

del *.cbr
del *.cg*
del *.cp
del *.ct*
del *.ebr
del *.nav
del *.op
del *.ot

:ResetDirectory

cd ..\..\..
GOTO Finished

:PrintUsage

echo Usage: hmake_unlit ^<game^> ^<level^> [noclean]
GOTO Finished

:BadLevel

echo Error: The specified game or level does not exist
GOTO Finished

:Finished