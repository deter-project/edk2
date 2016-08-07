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

#include "QuickJump.h"

///
/// Global Variables
/// 
 
// Graphics  ~~~

EFI_GRAPHICS_OUTPUT_PROTOCOL  *gop = NULL;
UINT32 mode_width = 0,
       mode_height = 0;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *top = NULL;
UINTN text_cols = 0,
      text_rows = 0;

// Networking ~~~

EFI_MTFTP4_PROTOCOL *tftp = NULL;

///
/// Entry point
///

EFI_STATUS
EFIAPI
QuickJumpEntry(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
  EFI_STATUS status = initGraphics();
  if(EFI_ERROR(status)) return status;

  status = initNetwork();
  if(EFI_ERROR(status))
  {
    run(NULL);
    return status;
  }

  status = showLogo();
  if(EFI_ERROR(status))
  {
    run(NULL);
    return status;
  }

  run(L"node ready, awaiting allocation");
  return EFI_SUCCESS;
}


///
/// Utility Functions
///

void printMsg(const CHAR16 *msg)
{
  UINTN sl = StrLen(msg);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, text_rows - 1);
  Print(L"                                                                  ");
  gST->ConOut->SetCursorPosition(gST->ConOut, text_cols/2 - sl/2 - 1, text_rows - 1);
  Print(msg);
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

///
/// Graphics functions
///

EFI_STATUS initGraphics()
{
  gST->ConOut->ClearScreen (gST->ConOut);

  EFI_STATUS status = initTop();
  if(EFI_ERROR(status)) return status;

  status = initGop();
  if(EFI_ERROR(status)) return status;

  //maxRes();
  return status;
}

EFI_STATUS initTop()
{
  //get a handle to the simple text output protocol
  EFI_STATUS status = gBS->HandleProtocol(
      gST->ConsoleOutHandle,
      &gEfiSimpleTextOutProtocolGuid,
      (VOID**) &top);

  if(EFI_ERROR(status)) return status;
  
  top->QueryMode(top, top->Mode->Mode, &text_cols, &text_rows);

  return status;
}


EFI_STATUS initGop()
{
  //get a handle to the graphics output protocol
  EFI_STATUS status = gBS->LocateProtocol(
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      (VOID**) &gop);

  if(EFI_ERROR(status)) 
  {
    printMsg(L"failed to initialize graphics");
    return status;
  }

  mode_width = gop->Mode->Info->HorizontalResolution,
  mode_height = gop->Mode->Info->VerticalResolution;

  return status;
}

EFI_HANDLE* readLogoLocal(UINTN *fs)
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
    return NULL;
  }
  printMsg(L"opened deter logo");

  //allocate buffer for file
  EFI_FILE_INFO *fi = ShellGetFileInfo(fh);
  EFI_HANDLE *fb = AllocateZeroPool( (UINTN)fi->FileSize );
  if(fb == NULL) {
    printMsg(L"failed to allocate buffer for logo");
    return NULL;
  }

  //read logo file into buffer
  *fs = (UINTN) fi->FileSize;
  status = ShellReadFile(fh, fs, fb);
  if(EFI_ERROR(status))
  {
    printMsg(L"failed to read logo file into buffer");
    return NULL;
  }

  ShellCloseFile(&fh);
  printMsg(L"read logo file into buffer");

  return fb;
}

EFIAPI
EFI_STATUS
tftp_timeout_cb( IN EFI_MTFTP4_PROTOCOL *this, IN EFI_MTFTP4_TOKEN *tok )
{
  printMsg(L"read logo: timeout");
  return EFI_SUCCESS;
}

EFI_HANDLE* readLogoRemote(UINTN *fs)
{
  //init tftp token
  EFI_MTFTP4_TOKEN tok;
  tok.Event = NULL;
  tok.Filename = (UINT8*)"deter.bmp";
  tok.ModeStr = NULL;
  tok.OptionCount = 0;
  tok.BufferSize = 0;
  tok.Buffer = NULL;
  tok.Context = NULL;
  tok.CheckPacket = NULL;
  tok.TimeoutCallback = tftp_timeout_cb;
  tok.PacketNeeded = NULL;
  tok.OverrideData = NULL;

  //get the remote file size
  /*
  EFI_STATUS status = tftp->ReadFile(tftp, &tok);
  if(EFI_ERROR(status))
  {
    printMsg(L"failed to get logo file size over tftp");
    return NULL;
  }
  */

  //allocate buffer for file
  printMsg(L"allocating logo buffer");
  tok.BufferSize = 120054;
  *fs = 120054;
  EFI_HANDLE *fb = AllocateZeroPool( (UINTN)tok.BufferSize );
  if(fb == NULL) {
    printMsg(L"failed to allocate buffer for logo");
    return NULL;
  }
  printMsg(L"logo buffer allocated");

  printMsg(L"reading logo over network");
  //read logo file into buffer over tftp
  tok.Buffer = (VOID*)fb;
  EFI_STATUS status = tftp->ReadFile(tftp, &tok);
  if(EFI_ERROR(status))
  {
    switch(status)
    {
      case EFI_INVALID_PARAMETER: printMsg(L"read logo: invalid parameter"); break;
      case EFI_UNSUPPORTED: printMsg(L"read logo: unsupported"); break;
      case EFI_NOT_STARTED: printMsg(L"read logo: not started"); break;
      case EFI_NO_MAPPING: printMsg(L"read logo: no mapping"); break;
      case EFI_ALREADY_STARTED: printMsg(L"read logo: already started"); break;
      case EFI_ACCESS_DENIED: printMsg(L"read logo: access denied"); break;
      case EFI_OUT_OF_RESOURCES: printMsg(L"read logo: out of resources"); break;
      case EFI_DEVICE_ERROR: printMsg(L"read logo: device error"); break;
      default: printMsg(L"read logo: unknown error"); break;
    }
    return NULL;
  }
  if(EFI_ERROR(tok.Status))
  {
    switch(tok.Status)
    {
      case EFI_OUT_OF_RESOURCES: printMsg(L"read logo: out of resources"); break;
      case EFI_BUFFER_TOO_SMALL: printMsg(L"read logo: buffer too small"); break;
      case EFI_ABORTED: printMsg(L"read logo: aborted"); break;
      case EFI_NETWORK_UNREACHABLE: printMsg(L"read logo: net unreach"); break;
      case EFI_ICMP_ERROR: printMsg(L"read logo: icmp error"); break;
      case EFI_TIMEOUT: printMsg(L"read logo: timeout"); break;
      case EFI_TFTP_ERROR: printMsg(L"read logo: tftp error"); break;
      case EFI_DEVICE_ERROR: printMsg(L"read logo: device error"); break;
      case EFI_NO_MEDIA: printMsg(L"read logo: no media"); break;
      default: printMsg(L"read logo: unknown error");
    }
    return NULL;
  }
  printMsg(L"logo read over network");

  return fb;
}

EFI_STATUS showLogo()
{

  UINTN fs = 0;
  //EFI_HANDLE *fb = readLogoLocal(&fs);
  EFI_HANDLE *fb = readLogoRemote(&fs);
  if(fb == NULL) return EFI_LOAD_ERROR;

  VOID *gopBlt = NULL;
  UINTN gopBltSz = 0;
  UINTN logoHeight = 0,
        logoWidth = 0;

  UINTN bmp_error;
  EFI_STATUS status = ConvertBmpToGopBlt(
      (VOID*)fb,
      fs,
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
    return status;
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
    return status;
  }

  printMsg(L"Deter QuickJump");
  return EFI_SUCCESS;
}

///
/// Network Functions
///

EFI_STATUS initNetwork()
{
  EFI_STATUS status = initTftp();
  
  return status;
}

EFI_STATUS initTftp()
{
  EFI_STATUS status = gBS->LocateProtocol(
      &gEfiMtftp4ProtocolGuid,
      NULL,
      (VOID**) &tftp);

  if(EFI_ERROR(status))
  {
    switch(status)
    {
      case EFI_NOT_FOUND: printMsg(L"locate tftp: not found"); break;
      case EFI_INVALID_PARAMETER: printMsg(L"locate tftp: invalid param"); break;
      default: printMsg(L"locate tftp: unknown failure");
    }
    return status;
  }

  EFI_MTFTP4_CONFIG_DATA cfg;
  cfg.UseDefaultSetting = TRUE;

  //this needs to be set to a valid ip address for supermicro boards
  //although it really dosen't matter what thet address actually is
  cfg.StationIp.Addr[0] = 10;
  cfg.StationIp.Addr[1] = 0;
  cfg.StationIp.Addr[2] = 0;
  cfg.StationIp.Addr[3] = 1;

  cfg.SubnetMask.Addr[0] = 255;
  cfg.SubnetMask.Addr[1] = 255;
  cfg.SubnetMask.Addr[2] = 255;
  cfg.SubnetMask.Addr[3] = 0;

  cfg.GatewayIp.Addr[0] = 10;
  cfg.GatewayIp.Addr[1] = 0;
  cfg.GatewayIp.Addr[2] = 0;
  cfg.GatewayIp.Addr[3] = 1;

  cfg.ServerIp.Addr[0] = 10;
  cfg.ServerIp.Addr[1] = 0;
  cfg.ServerIp.Addr[2] = 0;
  cfg.ServerIp.Addr[3] = 1;
  cfg.LocalPort = 0;
  cfg.InitialServerPort = 69;
  cfg.TryCount = 4;
  cfg.TimeoutValue = 7;

  status = tftp->Configure(tftp, &cfg);
  if(EFI_ERROR(status))
  {
    switch(status)
    {
      case EFI_INVALID_PARAMETER: printMsg(L"tftp: invalid parameter"); break;
      case EFI_ACCESS_DENIED: printMsg(L"tftp: access denied"); break;
      case EFI_NO_MAPPING: printMsg(L"tftp: no mapping"); break;
      case EFI_UNSUPPORTED: printMsg(L"tftp: unsupported"); break;
      case EFI_OUT_OF_RESOURCES: printMsg(L"tftp: out of resources"); break;
      case EFI_DEVICE_ERROR: printMsg(L"tftp: device error"); break;
      default: printMsg(L"tftp: unknown error");
    }
    return status;
  }

  return EFI_SUCCESS;
}

///
/// Runtime Functions
///

void run(CHAR16 *msg)
{
  if(msg != NULL) printMsg(msg);

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
