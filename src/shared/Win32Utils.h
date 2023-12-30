/**********************************************************************************
 * MIT License
 *
 * Copyright (c) 2018 Antoine Beauchamp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#ifndef WIN32_UTILS_H
#define WIN32_UTILS_H

#include <Windows.h>
#include <string>

namespace Win32Utils
{
  void GetWindowsVersion(int& major, int& minor);
  std::string GetWindowsProductName();
  bool EnableMonitorDpiAwareness();
  bool IsMonitorDpiAwarenessEnabled();
  int GetSystemDPI();
  float GetSystemScaling();
  int GetSystemScalingPercent();
  int GetSystemDefaultDPI();
  int GetMonitorCount();
  bool GetMousePositionInVirtualScreenCoordinates(int* x, int* y);
  bool GetMousePositionInMonitorCoordinates(int* monitor_index, int* x, int* y);
  int GetMonitorDPI(int monitor_index);
  float GetMonitorScaling(int monitor_index);
  int GetMonitorScalingPercent(int monitor_index);
  SIZE GetIconSize(HICON hIcon);
  HICON GetBestIconForMenu(HICON hIconLarge, HICON hIconSmall);
  RGBQUAD ToRgbQuad(const DWORD& color);
  SIZE GetBitmapSize(HBITMAP hBitmap);
  int GetBitPerPixel(HBITMAP hBitmap);
  int GetBitPerPixel(BITMAP* bmp);
  BOOL FillTransparentPixels(HBITMAP hBitmap, COLORREF background_color);
  HBITMAP CreateBitmapWithAlphaChannel(int width, int height, HDC hDc);
  HBITMAP CopyAsBitmap(HICON hIcon, const int bitmap_width, const int bitmap_height);
  HBITMAP CopyAsBitmap(HICON hIcon);
  bool SaveAs32BppBitmapFile(const char* path, HBITMAP hBitmap);
  bool SaveAs32BppBitmapV1File(const char* path, HBITMAP hBitmap);
  bool SaveAs32BppBitmapV3File(const char* path, HBITMAP hBitmap);
  bool SaveBitmapFile(const char* path, HBITMAP hBitmap);
  HBITMAP LoadBitmapFromFile(const char* path);
  BOOL IsFullyTransparent(HBITMAP hBitmap);
  BOOL IsFullyTransparent(const std::string& buffer);
  std::string GetMenuItemDetails(HMENU hMenu, UINT pos);
  std::string GetMenuTree(HMENU hMenu, int indent);
} //namespace Win32Utils

#endif //WIN32_UTILS_H
