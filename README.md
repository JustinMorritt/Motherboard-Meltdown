# Motherboard Meltdown!
Direct X 11 Game I Made



Needed In the VC++ 

$(ExecutablePath);$(DXSDK_DIR)Utilities\bin\x86;
$(IncludePath);$(DXSDK_DIR)Include;Common;Common\FMOD\inc
$(LibraryPath);$(DXSDK_DIR)Lib\x86;Common;Common\FMOD\lib


PRE-BUILD EVENTS 
 copy “$(SolutionDir)Common\FMOD\lib\fmodL.dll” “$(OutputPath)”;
 copy “$(SolutionDir)Common\FMOD\lib\fmod.dll” “$(OutputPath)”;

 
FMOD******************************************* 
linker inputs-->    fmod_vc.lib    fmodL_vc.lib


