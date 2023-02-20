# Microsoft Developer Studio Project File - Name="VLCSim" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VLCSim - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VLCSim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VLCSim.mak" CFG="VLCSim - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VLCSim - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VLCSim - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VLCSim - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VLCSim - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VLCSim - Win32 Release"
# Name "VLCSim - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "dialog"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\coms_options.cpp
# End Source File
# Begin Source File

SOURCE=.\custom_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_comms_errors.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_control.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_datetime_memsig.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_events.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_main.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_transactions.cpp
# End Source File
# Begin Source File

SOURCE=.\game_play_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\program_options_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\sdb_dlg.cpp
# End Source File
# End Group
# Begin Group "xseries"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\game_play.cpp
# End Source File
# Begin Source File

SOURCE=.\xseries_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\xseries_transmit_thread.cpp
# End Source File
# End Group
# Begin Group "utils"

# PROP Default_Filter "cpp"
# Begin Group "xedit"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\XEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\colour_list_box.cpp
# End Source File
# Begin Source File

SOURCE=.\cputicker.cpp
# End Source File
# Begin Source File

SOURCE=.\date_time.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_cell.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_hex.cpp
# End Source File
# Begin Source File

SOURCE=.\enumser.cpp
# End Source File
# Begin Source File

SOURCE=.\list_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\mscommctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\notify.cpp
# End Source File
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\timers.cpp
# End Source File
# Begin Source File

SOURCE=.\utilities.cpp
# End Source File
# End Group
# Begin Group "msdev_generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=".\VLCSim.cpp"
# End Source File
# Begin Source File

SOURCE=".\VLCSim.rc"
# End Source File
# End Group
# Begin Group "vlc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\receive_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadTest.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_comms_output.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_id.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_id_list.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_receive_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\vlc_transmit.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "dialog_header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\coms_options.h
# End Source File
# Begin Source File

SOURCE=.\custom_dlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg_comms_errors.h
# End Source File
# Begin Source File

SOURCE=.\dlg_configuration.h
# End Source File
# Begin Source File

SOURCE=.\dlg_control.h
# End Source File
# Begin Source File

SOURCE=.\dlg_datetime_memsig.h
# End Source File
# Begin Source File

SOURCE=.\dlg_events.h
# End Source File
# Begin Source File

SOURCE=.\dlg_exceptions.h
# End Source File
# Begin Source File

SOURCE=.\dlg_main.h
# End Source File
# Begin Source File

SOURCE=.\dlg_monitor.h
# End Source File
# Begin Source File

SOURCE=.\dlg_packet_interface.h
# End Source File
# Begin Source File

SOURCE=.\dlg_transactions.h
# End Source File
# Begin Source File

SOURCE=.\game_play_dlg.h
# End Source File
# Begin Source File

SOURCE=.\sdb_dlg.h
# End Source File
# End Group
# Begin Group "xseries_header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game_play.h
# End Source File
# Begin Source File

SOURCE=.\xseries_packet.h
# End Source File
# Begin Source File

SOURCE=.\xseries_transmit_thread.h
# End Source File
# End Group
# Begin Group "utils_header"

# PROP Default_Filter "h"
# Begin Group "xedit No. 1"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\XEdit.h
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\colour_list_box.h
# End Source File
# Begin Source File

SOURCE=.\cputicker.h
# End Source File
# Begin Source File

SOURCE=.\date_time.h
# End Source File
# Begin Source File

SOURCE=.\edit_cell.h
# End Source File
# Begin Source File

SOURCE=.\edit_hex.h
# End Source File
# Begin Source File

SOURCE=.\enumser.h
# End Source File
# Begin Source File

SOURCE=.\list_ctrl.h
# End Source File
# Begin Source File

SOURCE=.\mscommctrl.h
# End Source File
# Begin Source File

SOURCE=.\notify.h
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE=.\sa_comms_crc.h
# End Source File
# Begin Source File

SOURCE=.\stdtype.h
# End Source File
# Begin Source File

SOURCE=.\timers.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# End Group
# Begin Group "msdev_header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=".\VLCSim.h"
# End Source File
# End Group
# Begin Group "vlc_header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\program_options_dlg.h
# End Source File
# Begin Source File

SOURCE=.\receive_thread.h
# End Source File
# Begin Source File

SOURCE=.\ThreadTest.h
# End Source File
# Begin Source File

SOURCE=.\vlc_comms_output.h
# End Source File
# Begin Source File

SOURCE=.\vlc_exceptions.h
# End Source File
# Begin Source File

SOURCE=.\vlc_id.h
# End Source File
# Begin Source File

SOURCE=.\vlc_id_list.h
# End Source File
# Begin Source File

SOURCE=.\vlc_packet.h
# End Source File
# Begin Source File

SOURCE=.\vlc_receive_thread.h
# End Source File
# Begin Source File

SOURCE=.\vlc_transmit.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo3.BMP
# End Source File
# Begin Source File

SOURCE=".\res\VLCSim.ico"
# End Source File
# Begin Source File

SOURCE=".\res\VLCSim.rc2"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section VLCSim : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscommctrl.h
# 	2:16:DefaultSinkClass:CMSCommCtrl
# End Section
# Section VLCSim : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
