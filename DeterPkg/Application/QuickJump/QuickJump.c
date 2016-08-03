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

#include <Burgaler.h>

void locateGraphics();
void maxRes();
void showLogo();
void run();
void initTop();
  
EFI_GRAPHICS_OUTPUT_PROTOCOL  *gop = NULL;
UINT32 mode_width = 0,
       mode_height = 0;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *top = NULL;
UINTN text_cols = 0,
      text_rows = 0;

EFI_STATUS
EFIAPI
QuickJumpEntry(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
  gST->ConOut->ClearScreen (gST->ConOut);
  initTop();
  locateGraphics();
  maxRes();
  showLogo();
  run();

  return EFI_SUCCESS;
}


int row = 1;
void printMsg(const CHAR16 *msg)
{
  UINTN sl = StrLen(msg);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, text_rows - 1);
  Print(L"                                                                  ");
  gST->ConOut->SetCursorPosition(gST->ConOut, text_cols/2 - sl/2 - 1, text_rows - 1);
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
  //printMsg(L"locating the gop\n");

  EFI_STATUS status = gBS->LocateProtocol(
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      (VOID**) &gop);
  if(status == EFI_SUCCESS)
  {
    //printMsg(L"gop found, yay!\n");
    //gop->SetMode(gop, gop->Mode->MaxMode);
  }
  else if(status == EFI_INVALID_PARAMETER)
  {
    //printMsg(L"invalid param\n");
  }
  else if(status == EFI_NOT_FOUND)
  {
    //printMsg(L"gop not found, :(\n");
  }
}

void maxRes()
{
  int mode = gop->Mode->Mode;
  for(int i = 0; i < gop->Mode->MaxMode; i++)
  {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN info_sz;
    UINTN px = 0;

    gop->QueryMode(gop, i, &info_sz, &info);
    if(info->PixelsPerScanLine > px)
    {
      px = info->PixelsPerScanLine;
      mode = i;
    }
  }
  gop->SetMode(gop, mode);
  top->SetMode(top, mode);
    
  mode_width = gop->Mode->Info->HorizontalResolution,
  mode_height = gop->Mode->Info->VerticalResolution;

  top->QueryMode(top, mode, &text_cols, &text_rows);
}

void showLogo()
{

  //open logo file
  SHELL_FILE_HANDLE fh;
  EFI_STATUS status = ShellOpenFileByName(
      L"deter.bmp",
      &fh,
      EFI_FILE_MODE_READ,
      0 );

  if(EFI_ERROR(status))
  {
    printMsg(L"could not open deter logo");
    return;
  }
  printMsg(L"opened deter logo");

  //allocate buffer for file
  EFI_FILE_INFO *fi = ShellGetFileInfo(fh);
  EFI_HANDLE *fb = AllocateZeroPool( (UINTN)fi->FileSize );
  if(fb == NULL) {
    printMsg(L"failed to allocate buffer for logo");
    return;
  }

  //read logo file into buffer
  UINTN fs = (UINTN) fi->FileSize;
  status = ShellReadFile(fh, &fs, fb);
  if(EFI_ERROR(status))
  {
    printMsg(L"failed to read logo file into buffer");
    return;
  }

  ShellCloseFile(&fh);
  printMsg(L"read logo file into buffer");

  VOID *gopBlt = NULL;
  UINTN gopBltSz = 0;
  UINTN logoHeight = 0,
        logoWidth = 0;

  UINTN bmp_error;
  status = ConvertBmpToGopBlt(
      (VOID*)fb,
      fi->FileSize,
      &gopBlt,
      &gopBltSz,
      &logoHeight,
      &logoWidth,
      &bmp_error);

  if(EFI_ERROR(status))
  {
    printMsg(L"failed to convert BMP to BLT");
    if(status == EFI_UNSUPPORTED) { printMsg(L"unsupported"); }
    if(status == EFI_INVALID_PARAMETER) { printMsg(L"invalid parameter"); }
    if(status == EFI_BUFFER_TOO_SMALL) { printMsg(L"why so stingy?"); }

    if(bmp_error == BMP_BAD_CHAR) { printMsg(L"bmp: bad char"); }
    if(bmp_error == BMP_NO_COMPRESSION) { printMsg(L"bmp: no compression"); }
    if(bmp_error == BMP_BAD_HEADER) { printMsg(L"bmp: bad header"); }
    return;
  }
  printMsg(L"converted bmp to blt");

  UINT32 xoff = mode_width / 2 - logoWidth / 2,
         yoff = mode_height / 2 - logoHeight / 2;
  status = gop->Blt(
      gop,
      gopBlt,
      EfiBltBufferToVideo,
      0, 0,
      xoff, yoff,
      logoWidth, logoHeight,
      0
  );
  if(EFI_ERROR(status))
  {
    printMsg(L"place image on screen");
    return;
  }

  printMsg(L"Deter QuickJump");
}

void run()
{
  printMsg(L"node ready, awaiting allocation");
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
