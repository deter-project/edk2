#pragma once

/*=============================================================================
 *
 *
 *  This header provides an interface to internal library code that has been
 *  shamelessly stolen from other portions of the edk2 library. For each
 *  stolen component I provide reference to the site of the burglary
 *
 *
 *===========================================================================*/

/**
 * stolen from IntelFrameworkModulePkg/Library/GenericBdsLib/BdsConsole.c
 */

EFI_STATUS
ConvertBmpToGopBlt (
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
     OUT UINTN     *PixelHeight,
     OUT UINTN     *PixelWidth
  );
