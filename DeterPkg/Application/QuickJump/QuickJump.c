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

#include <Library/UefiBootServicesTableLib.h>
#include <FrameworkDxe.h>
#include <Library/DxeServicesLib.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/UgaDraw.h>

void locateGraphics();
void showLogo();
void run();
void initTop();
//void initConsole();
  
EFI_GRAPHICS_OUTPUT_PROTOCOL  *gop = NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *top = NULL;

EFI_STATUS
EFIAPI
QuickJumpEntry(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
  gST->ConOut->ClearScreen (gST->ConOut);
  initTop();
  locateGraphics();
  showLogo();
  run();
  //initConsole();

  return EFI_SUCCESS;
}


/*
void initConsole()
{
  EFI_STATUS status = gBS->LocateProtocol(
      &gEfiConsoleControlProtocolGuid,
      NULL,
      (VOID**), &ccp);

  if(status == EFI_SUCCESS)
  {
    printf("ccp found, yay!\n");
    ccp->SetMode(ccp, EfiConsoleControlScreenGraphics);
  }
  else if(status == EFI_INVALID_PARAMETER)
  {
    printf("ccp locate: invalid param\n");
  }
  else if(status == EFI_NOT_FOUND)
  {
    printf("ccp locate: not found, :(\n");
  }

}
*/

int row = 3;
void printMsg(const CHAR16 *msg)
{
  gST->ConOut->SetCursorPosition(gST->ConOut, 7, row++);
  Print(msg);
}

void initTop()
{
  //get a handle to the simple text output protocol
  EFI_STATUS status = gBS->HandleProtocol(
      gST->ConsoleOutHandle,
      &gEfiSimpleTextOutProtocolGuid,
      (VOID**) &top);

  if(EFI_ERROR(status)) 
  {
    printMsg(L"TOP not supported");
    return;
  }
  printMsg(L"TOP initailized");
}

void locateGraphics()
{
  printMsg(L"locating the gop\n");

  EFI_STATUS status = gBS->LocateProtocol(
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      (VOID**) &gop);
  if(status == EFI_SUCCESS)
  {
    printMsg(L"gop found, yay!\n");
  }
  else if(status == EFI_INVALID_PARAMETER)
  {
    printMsg(L"invalid param\n");
  }
  else if(status == EFI_NOT_FOUND)
  {
    printMsg(L"gop not found, :(\n");
  }
}


void showLogo()
{

  //get a handle to the graphics output protocol
  EFI_STATUS status = gBS->HandleProtocol(
      gST->ConsoleOutHandle,
      &gEfiGraphicsOutputProtocolGuid,
      (VOID**) &gop);

  if(EFI_ERROR(status)) 
  {
    printMsg(L"GOP not supported");
    if(status == EFI_INVALID_PARAMETER)
    {
      printMsg(L"gop: invalid param\n");
    }
    if(status == EFI_NOT_FOUND)
    {
      printMsg(L"gop: not found, :(\n");
    }
    //return;
  }
  else
  {
    printMsg(L"GOP initailized");
  }

  printMsg(L"getting logo from firmware");
  //get the deter logo from the firmware volume
  UINT8 *ImageData;
  UINTN ImageSize;
  status = GetSectionFromAnyFv(
      PcdGetPtr(PcdLogoFile),
      EFI_SECTION_RAW,
      0, //read from the first section of the file
      (VOID**) &ImageData, &ImageSize);

  if(EFI_ERROR(status))
  {
    printMsg(L"could not read logo from firmware volume");
    return;
  }

  printMsg(L"Deter QuickJump");
}

void run()
{
  printMsg(L"Entering run loop");
  BOOLEAN exit = FALSE;
  while(!exit)
  {
    UINTN index;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);
    EFI_INPUT_KEY key;
    EFI_STATUS status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    if(!EFI_ERROR(status))
    {
      switch(key.UnicodeChar)
      {
        case 'q': 
          printMsg(L"bye!");
          gST->ConOut->ClearScreen (gST->ConOut);
          return;
        default: break;
      }
    }
  }
}
