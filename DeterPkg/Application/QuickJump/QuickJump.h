/**
 *
 * QuickJump - A bootloader that enables quick server reboot
 *
 * Copyright (c) 2016, The Deter Project. All rights reserved
 *
 * This program and the accompanying materials
 * are licensed and made available under the terms and conditions of the BSD License
 * which accompanies this distribution. The full text of the license may be found at
 * http://opensource.org/licenses/bsd-license.php
 * 
 * THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 * WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 *
 **/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <stdio.h>

#include <FrameworkDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/ShellLib.h>
//#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
//#include <Library/GraphicsLib.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/UgaDraw.h>
#include <Protocol/Mtftp4.h>

#include <Burgaler.h>

///
/// Graphics Functions
///

EFI_STATUS initGraphics(),
           initTop(),
           initGop();
void maxRes();
EFI_HANDLE * readLogoLocal(UINTN *fs),
           * readLogoRemote(UINTN *fs);

EFI_STATUS showLogo();

///
/// Network Functions
///
EFI_STATUS initNetwork(),
           initTftp();

///
/// Runtime Functions
///

void run();

///
/// Utility Functions
///

void printMsg(const CHAR16 *msg);

