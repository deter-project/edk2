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
//#include <Library/UefiApplicationEntryPoint.h>
#include <Library/ShellCEntryLib.h>

//image
#include <stdio.h>
/*
#include <Pi/PiFirmwareFile.h>
#include <protocol/graphicsoutput.h>
#include <IndustryStandard/Bmp.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
*/

int main(int argc, char **argv)
{
  //Print(L"Deter Quick Boot!\n");
  printf("Deter Quick Booooot!\n");
  return EFI_SUCCESS;
}

