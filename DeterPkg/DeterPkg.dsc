##
#
# Copyright (c) 2016, The Deter Project. All rights reserved
#
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution. The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
# 
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
##

[BuildOptions]
  GCC:*_*_X64_CC_FLAGS   = -fno-PIC

[Defines]
  PLATFORM_NAME               = DeterAll
  #versioned identifier for the deter platform (when ver changes this increments)
  PLATFORM_GUID               = 396bf676-5619-11e6-89c6-a30aa944fd24
  PLATFORM_VERSION            = 0.01
  DSC_SPECIFICATION           = 0x00010005
  BUILD_DIRECTORY             = Build/DeterAll
  SUPPORTED_ARCHITECTURES     = X64
  BUILD_TARGETS               = DEBUG|RELEASE
  SKUID_IDENTIFIER            = DEFAULT

[LibraryClasses]
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf  
  #LibC|StdLib/LibC/LibC.inf
  Stdio|StdLib/LibC/Stdio/Stdio.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  #LibCType|StdLib/LibC/Ctype/Ctype.inf
  #LibGdtoa|StdLib/LibC/gdtoa/gdtoa.inf
  #LibLocale|StdLib/LibC/Locale/Locale.inf
  #LibStdLib|StdLib/LibC/StdLib/StdLib.inf
  #LibString|StdLib/LibC/String/String.inf
  #LibTime|StdLib/LibC/Time/Time.inf
  #LibUefi|StdLib/LibC/Uefi/Uefi.inf
  #LibWchar|StdLib/LibC/Wchar/Wchar.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  #LibGen|StdLib/PosixLib/Gen/LibGen.inf
  #DevUtility|StdLib/LibC/Uefi/Devices/daUtility.inf
  #LibStdio|StdLib/LibC/Stdio/Stdio.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf  
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

[SkuIds]
  0|DEFAULT

[LibraryClasses]

[Components]
  DeterPkg/Application/QuickJump/QuickJump.inf

!include StdLib/StdLib.inc
