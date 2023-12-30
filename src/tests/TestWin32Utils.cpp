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

#pragma warning( push )
#pragma warning( disable: 4018 ) //googletest\install\include\gtest/gtest.h(1528): warning C4018: '>=' : signed/unsigned mismatch
#include <gtest/gtest.h>
#pragma warning( pop )

#include "TestWin32Utils.h"
#include "Win32Utils.h"
#include "ArgumentsHandler.h"

#include "rapidassist/undef_windows_macros.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/testing.h"
#include "rapidassist/cli.h"
#include "rapidassist/errors.h"

namespace ra
{
  namespace strings
  {
    std::string ToString(DWORD& value)
    {
      return ra::strings::ToString((size_t)value);
    }
  }
}
namespace shellanything
{
  namespace test
  {
    std::string zero_padding(std::string& input, size_t final_length)
    {
      std::string output = input;
      while (output.length() < final_length)
      {
        output.insert(0, 1, '0');
      }
      return output;
    }
    std::string ToString(SIZE s)
    {
      return ra::strings::ToString(s.cx) + "x" + ra::strings::ToString(s.cy);
    }

    //--------------------------------------------------------------------------------------------------
    void TestWin32Utils::SetUp()
    {
    }
    //--------------------------------------------------------------------------------------------------
    void TestWin32Utils::TearDown()
    {
    }
    //--------------------------------------------------------------------------------------------------
    TEST_F(TestWin32Utils, testIconToBitmapConversion)
    {
      for (int i = 0; i < 305; i++)
      {
        HICON hIconLarge = NULL;
        HICON hIconSmall = NULL;

        const char* icon_path = "c:\\windows\\system32\\shell32.dll";
        UINT numIconLoaded = ExtractIconEx(icon_path, i, &hIconLarge, &hIconSmall, 1);
        ASSERT_GE(numIconLoaded, 1); //at least 1 icon loaded
        ASSERT_TRUE(hIconLarge != NULL);

        //Convert the icon to a bitmap (with invisible background)
        SIZE icon_size = Win32Utils::GetIconSize(hIconLarge);
        HBITMAP hBitmap = Win32Utils::CopyAsBitmap(hIconLarge, icon_size.cx, icon_size.cy);

        DestroyIcon(hIconLarge);
        DestroyIcon(hIconSmall);

        // build output files
        std::string test_dir = ra::filesystem::GetTemporaryDirectory() + "\\" + ra::testing::GetTestQualifiedName();
        ASSERT_TRUE(ra::filesystem::CreateDirectory(test_dir.c_str()));
        std::string dll_filename = ra::filesystem::GetFilename(icon_path);
        std::string output_path_v1 = test_dir + "\\" + dll_filename + "." + zero_padding(ra::strings::ToString(i), 3) + ".v1.bmp";
        std::string output_path_v3 = test_dir + "\\" + dll_filename + "." + zero_padding(ra::strings::ToString(i), 3) + ".v3.bmp";

        //save to a file
        ASSERT_TRUE(Win32Utils::SaveAs32BppBitmapV1File(output_path_v1.c_str(), hBitmap));
        ASSERT_TRUE(Win32Utils::SaveAs32BppBitmapV3File(output_path_v3.c_str(), hBitmap));

        //delete the bitmap
        DeleteObject(hBitmap);
      }
    }
    //--------------------------------------------------------------------------------------------------
    TEST_F(TestWin32Utils, testSymbolicLinksIconTransparencyIssue12)
    {
      //https://github.com/end2endzone/ShellAnything/issues/12

      struct QUICK_ICON
      {
        const char* path;
        int index;
      };
      static const QUICK_ICON icons[] = {
        {"C:\\Windows\\System32\\imageres.dll", 154},
        {"C:\\Windows\\System32\\shell32.dll",   29},
        {"C:\\Windows\\System32\\imageres.dll", 160},
        {"C:\\Windows\\System32\\mmcndmgr.dll",  69},
      };
      static const size_t num_icons = sizeof(icons) / sizeof(icons[0]);

      for (int i = 0; i < num_icons; i++)
      {
        const char* icon_path = icons[i].path;
        const int& index = icons[i].index;

        HICON hIconLarge = NULL;
        HICON hIconSmall = NULL;

        UINT numIconLoaded = ExtractIconEx(icon_path, index, &hIconLarge, &hIconSmall, 1);
        ASSERT_GE(numIconLoaded, 1); //at least 1 icon loaded
        ASSERT_TRUE(hIconLarge != NULL);

        //Convert the icon to a bitmap (with invisible background)
        SIZE icon_size = Win32Utils::GetIconSize(hIconLarge);
        HBITMAP hBitmap = Win32Utils::CopyAsBitmap(hIconLarge, icon_size.cx, icon_size.cy);

        DestroyIcon(hIconLarge);
        DestroyIcon(hIconSmall);

        // build output files
        std::string test_dir = ra::filesystem::GetTemporaryDirectory() + "\\" + ra::testing::GetTestQualifiedName();
        ASSERT_TRUE(ra::filesystem::CreateDirectory(test_dir.c_str()));
        std::string dll_filename = ra::filesystem::GetFilename(icon_path);
        std::string output_path = test_dir + "\\" + dll_filename + "." + zero_padding(ra::strings::ToString(index), 3) + ".bmp";

        //save to a file
        ASSERT_TRUE(Win32Utils::SaveAs32BppBitmapFile(output_path.c_str(), hBitmap));

        //delete the bitmap
        DeleteObject(hBitmap);
      }
    }
    //--------------------------------------------------------------------------------------------------
    int PrintMonitorInformation(int argc, char** argv)
    {
      // Set current process as monitor dpi aware...
      Win32Utils::EnableMonitorDpiAwareness();
      bool monitor_dpi_aware = Win32Utils::IsMonitorDpiAwarenessEnabled();
      printf("monitor_dpi_aware=%d\n", monitor_dpi_aware);
      if (!monitor_dpi_aware)
        return 1;

      // Print system metrics
      std::cout << "System metrics:\n";
      std::cout << "SM_CXSCREEN    : " << GetSystemMetrics(SM_CXSCREEN) << "\n";
      std::cout << "SM_CYSCREEN    : " << GetSystemMetrics(SM_CYSCREEN) << "\n";
      std::cout << "SM_CXSMICON    : " << GetSystemMetrics(SM_CXSMICON) << "\n";
      std::cout << "SM_CYSMICON    : " << GetSystemMetrics(SM_CYSMICON) << "\n";
      std::cout << "SM_CXICON      : " << GetSystemMetrics(SM_CXICON) << "\n";
      std::cout << "SM_CYICON      : " << GetSystemMetrics(SM_CYICON) << "\n";
      std::cout << "System DPI     : " << Win32Utils::GetSystemDPI() << "\n";
      std::cout << "System Scaling : " << Win32Utils::GetSystemScalingPercent() << "%" << "\n";

      // Print information about monitor and their scaling
      int count = Win32Utils::GetMonitorCount();
      printf("Found %d monitors.\n", count);
      for (int i = 0; i < count; i++)
      {
        int dpi = Win32Utils::GetMonitorDPI(i);
        int scaling = Win32Utils::GetMonitorScalingPercent(i);
        std::cout << "Monitor " << i << " is scaled to " << scaling << "% (dpi " << dpi << ")\n";
      }

      return 0; // success
    }
    COMMAND_LINE_ENTRY_POINT * PrintMonitorInformationEntryPoint = shellanything::RegisterCommandLineEntryPoint("PrintMonitorInformation", PrintMonitorInformation);
    TEST_F(TestWin32Utils, testMonitorInfo)
    {
      int exit_code = shellanything::InvokeCommandLineEntryPoint("PrintMonitorInformation", 0, NULL);
      ASSERT_EQ(0, exit_code);
    }
    //--------------------------------------------------------------------------------------------------
    int ProcessIconAsBitmap(HICON hIcon, const std::string& outout_file_path, SIZE & icon_size)
    {
      if (hIcon == NULL)
        return 16;

      //Convert the icon to a bitmap (with invisible background)
      icon_size = Win32Utils::GetIconSize(hIcon);
      HBITMAP hBitmap = Win32Utils::CopyAsBitmap(hIcon, icon_size.cx, icon_size.cy);

      //save to a file
      bool created = Win32Utils::SaveAs32BppBitmapFile(outout_file_path.c_str(), hBitmap);
      if (!created)
      {
        printf("Failed to create bitmap: '%s'\n", outout_file_path.c_str());
        return 15;
      }

      //delete the bitmap
      DeleteObject(hBitmap);

      return 0;
    }
    int ExtractLargeAndSmallIcons(int argc, char** argv)
    {
      //MessageBox(NULL, "ATTACH NOW!", "ATTACH NOW!", MB_OK);

      // Set current process as monitor dpi aware...
      Win32Utils::EnableMonitorDpiAwareness();
      bool monitor_dpi_aware = Win32Utils::IsMonitorDpiAwarenessEnabled();
      if (!monitor_dpi_aware)
        return 5;

      const char* argument_name;

      argument_name = "icon_path";
      std::string icon_path;
      bool has_icon_path = ra::cli::ParseArgument(argument_name, icon_path, argc, argv);
      if (!has_icon_path)
      {
        printf("Missing argument: '%s'\n", argument_name);
        return 10;
      }

      argument_name = "output_large_path";
      std::string output_large_path;
      bool has_output_large_path = ra::cli::ParseArgument(argument_name, output_large_path, argc, argv);
      if (!has_output_large_path)
      {
        printf("Missing argument: '%s'\n", argument_name);
        return 11;
      }

      argument_name = "output_small_path";
      std::string output_small_path;
      bool has_output_small_path = ra::cli::ParseArgument(argument_name, output_small_path, argc, argv);
      if (!has_output_small_path)
      {
        printf("Missing argument: '%s'\n", argument_name);
        return 12;
      }

      if (!ra::filesystem::FileExists(icon_path.c_str()))
      {
        printf("File not found: '%s'\n", icon_path.c_str());
        return 13;
      }

      static const HICON INVALID_ICON_HANDLE_VALUE = NULL;

      HICON hIconLarge = INVALID_ICON_HANDLE_VALUE;
      HICON hIconSmall = INVALID_ICON_HANDLE_VALUE;

      static const int index = 0;
      UINT numIconLoaded = ExtractIconEx(icon_path.c_str(), index, &hIconLarge, &hIconSmall, 1);
      if (numIconLoaded != 1)
      {
        printf("No icons in file: '%s'\n", icon_path.c_str());
        return 14;
      }

      SIZE icon_size_large;
      SIZE icon_size_small;

      if (hIconLarge != INVALID_ICON_HANDLE_VALUE)
      {
        int exit_code = ProcessIconAsBitmap(hIconLarge, output_large_path, icon_size_large);
        DestroyIcon(hIconLarge);
        if (exit_code != 0)
          return 100 + exit_code;
      }
      if (hIconSmall != INVALID_ICON_HANDLE_VALUE)
      {
        int exit_code = ProcessIconAsBitmap(hIconSmall, output_small_path, icon_size_small);
        DestroyIcon(hIconSmall);
        if (exit_code != 0)
          return 1000 + exit_code;
      }

      // print stats about icons
      std::string icon_file_name = ra::filesystem::GetFilename(icon_path.c_str());
      std::cout << "icon_path=" << icon_file_name << "  -->  ";
      if (ra::filesystem::FileExists(output_large_path.c_str()))
        std::cout << " large:" << ToString(icon_size_large);
      if (ra::filesystem::FileExists(output_small_path.c_str()))
        std::cout << " small:" << ToString(icon_size_small);
      std::cout << "\n";

      return 0;
    }
    COMMAND_LINE_ENTRY_POINT* ExtractLargeAndSmallIconsEntryPoint = shellanything::RegisterCommandLineEntryPoint("ExtractLargeAndSmallIcons", ExtractLargeAndSmallIcons);
    TEST_F(TestWin32Utils, testExtractLargeAndSmallIconsIssue117)
    {
      //https://github.com/end2endzone/ShellAnything/issues/117

      static const char * icons[] = {
        "test_files\\test-res-16-20-24-32-40-48-60-64-72-80-96-120-128-144-160-192-256.ico",
        "test_files\\test-res-16-32-48-64-96-128-256.ico",
        "test_files\\issue117\\alarm_clock-256-32bpp-8ba-compressed.ico",
        "test_files\\issue117\\alarm_clock-16-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-20-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-24-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-32-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-40-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-48-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-64-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-96-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-128-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-all-compressed.ico",
      };
      static const size_t num_icons = sizeof(icons) / sizeof(icons[0]);

      for (int i = 0; i < num_icons; i++)
      {
        const std::string icon_path = ra::filesystem::GetPathBasedOnCurrentProcess(icons[i]);
        const int index = 0;

        ASSERT_TRUE(ra::filesystem::FileExists(icon_path.c_str())) << "File does not exists: " << icon_path;

        // build output files
        std::string test_dir = ra::filesystem::GetTemporaryDirectory() + "\\" + ra::testing::GetTestQualifiedName();
        ASSERT_TRUE(ra::filesystem::CreateDirectory(test_dir.c_str()));
        std::string icon_filename = ra::filesystem::GetFilename(icon_path.c_str());
        std::string large_bmp_path = test_dir + "\\" + icon_filename + ".large.bmp";
        std::string small_bmp_path = test_dir + "\\" + icon_filename + ".small.bmp";

        // remove data from previous runs
        ra::filesystem::DeleteFile(large_bmp_path.c_str());
        ra::filesystem::DeleteFile(small_bmp_path.c_str());

        // invoke extraction function
        const int argc = 3;
        static const std::string DOUBLE_QUOTES = "\"";
        std::string arg0 = std::string("--icon_path=") + DOUBLE_QUOTES + icon_path + DOUBLE_QUOTES;
        std::string arg1 = std::string("--output_large_path=") + DOUBLE_QUOTES + large_bmp_path + DOUBLE_QUOTES;
        std::string arg2 = std::string("--output_small_path=") + DOUBLE_QUOTES + small_bmp_path + DOUBLE_QUOTES;
        char* argv[argc] = {
          (char*)arg0.c_str(),
          (char*)arg1.c_str(),
          (char*)arg2.c_str(),
        };
        int exit_code = shellanything::InvokeCommandLineEntryPoint("ExtractLargeAndSmallIcons", argc, argv);
        ASSERT_EQ(0, exit_code);
      }
    }
    //--------------------------------------------------------------------------------------------------
    int ExtractSmallIcon(int argc, char** argv)
    {
      //MessageBox(NULL, "ATTACH NOW!", "ATTACH NOW!", MB_OK);

      // Set current process as monitor dpi aware...
      Win32Utils::EnableMonitorDpiAwareness();
      bool monitor_dpi_aware = Win32Utils::IsMonitorDpiAwarenessEnabled();
      if (!monitor_dpi_aware)
        return 5;

      const char* argument_name;

      argument_name = "icon_path";
      std::string icon_path;
      bool has_icon_path = ra::cli::ParseArgument(argument_name, icon_path, argc, argv);
      if (!has_icon_path)
      {
        printf("Missing argument: '%s'\n", argument_name);
        return 10;
      }

      argument_name = "output_path";
      std::string output_path;
      bool has_output_path = ra::cli::ParseArgument(argument_name, output_path, argc, argv);
      if (!has_output_path)
      {
        printf("Missing argument: '%s'\n", argument_name);
        return 11;
      }

      if (!ra::filesystem::FileExists(icon_path.c_str()))
      {
        printf("File not found: '%s'\n", icon_path.c_str());
        return 13;
      }

      static const HICON INVALID_ICON_HANDLE_VALUE = NULL;

      HICON hIconSmall = INVALID_ICON_HANDLE_VALUE;

      static const int index = 0;
      UINT numIconLoaded = ExtractIconEx(icon_path.c_str(), index, NULL, &hIconSmall, 1);
      if (numIconLoaded != 1)
      {
        printf("No icons in file: '%s'\n", icon_path.c_str());
        return 14;
      }

      SIZE icon_size_small;

      if (hIconSmall != INVALID_ICON_HANDLE_VALUE)
      {
        int exit_code = ProcessIconAsBitmap(hIconSmall, output_path, icon_size_small);
        DestroyIcon(hIconSmall);
        if (exit_code != 0)
          return 1000 + exit_code;
      }

      // print stats about icons
      std::string icon_file_name = ra::filesystem::GetFilename(icon_path.c_str());
      std::cout << "icon_path=" << icon_file_name << "  -->  ";
      if (ra::filesystem::FileExists(output_path.c_str()))
        std::cout << " small:" << ToString(icon_size_small);
      std::cout << "\n";

      return 0;
    }
    COMMAND_LINE_ENTRY_POINT* ExtractSmallIconEntryPoint = shellanything::RegisterCommandLineEntryPoint("ExtractSmallIcon", ExtractSmallIcon);
    TEST_F(TestWin32Utils, testExtractSmallIconIssue117)
    {
      //https://github.com/end2endzone/ShellAnything/issues/117

      static const char* icons[] = {
        "test_files\\test-res-16-20-24-32-40-48-60-64-72-80-96-120-128-144-160-192-256.ico",
        "test_files\\test-res-16-32-48-64-96-128-256.ico",
        "test_files\\issue117\\alarm_clock-256-32bpp-8ba-compressed.ico",
        "test_files\\issue117\\alarm_clock-16-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-20-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-24-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-32-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-40-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-48-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-64-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-96-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-128-32bpp-8ba-uncompressed.ico",
        "test_files\\issue117\\alarm_clock-all-compressed.ico",
      };
      static const size_t num_icons = sizeof(icons) / sizeof(icons[0]);

      for (int i = 0; i < num_icons; i++)
      {
        const std::string icon_path = ra::filesystem::GetPathBasedOnCurrentProcess(icons[i]);
        const int index = 0;

        ASSERT_TRUE(ra::filesystem::FileExists(icon_path.c_str())) << "File does not exists: " << icon_path;

        // build output files
        std::string test_dir = ra::filesystem::GetTemporaryDirectory() + "\\" + ra::testing::GetTestQualifiedName();
        ASSERT_TRUE(ra::filesystem::CreateDirectory(test_dir.c_str()));
        std::string icon_filename = ra::filesystem::GetFilename(icon_path.c_str());
        std::string small_bmp_path = test_dir + "\\" + icon_filename + ".small.bmp";

        // remove data from previous runs
        ra::filesystem::DeleteFile(small_bmp_path.c_str());

        // invoke extraction function
        const int argc = 2;
        static const std::string DOUBLE_QUOTES = "\"";
        std::string arg0 = std::string("--icon_path=") + DOUBLE_QUOTES + icon_path + DOUBLE_QUOTES;
        std::string arg1 = std::string("--output_path=") + DOUBLE_QUOTES + small_bmp_path + DOUBLE_QUOTES;
        char* argv[argc] = {
          (char*)arg0.c_str(),
          (char*)arg1.c_str(),
        };
        int exit_code = shellanything::InvokeCommandLineEntryPoint("ExtractSmallIcon", argc, argv);
        ASSERT_EQ(0, exit_code);
      }
    }
    //--------------------------------------------------------------------------------------------------
#define TYPE_STRING(t) "  " #t ":" + ra::strings::ToString(t) + "\n"
    std::string ToString(BITMAPFILEHEADER& b)
    {
      std::string s;
      s += "BITMAPFILEHEADER {\n";
      //s += "  bfType:" + ra::strings::ToString(b.bfType) + "\n";
      s += TYPE_STRING(b.bfType);
      s += TYPE_STRING(b.bfSize);
      s += TYPE_STRING(b.bfReserved1);
      s += TYPE_STRING(b.bfReserved2);
      s += TYPE_STRING(b.bfOffBits);
      s += "}\n";
      return s;
    }
    std::string ToString(BITMAPINFOHEADER& b)
    {
      std::string s;
      s += "BITMAPINFOHEADER {\n";
      s += TYPE_STRING(b.biSize);
      s += TYPE_STRING(b.biWidth);
      s += TYPE_STRING(b.biHeight);
      s += TYPE_STRING(b.biPlanes);
      s += TYPE_STRING(b.biBitCount);
      s += TYPE_STRING(b.biCompression);
      s += TYPE_STRING(b.biSizeImage);
      s += TYPE_STRING(b.biXPelsPerMeter);
      s += TYPE_STRING(b.biYPelsPerMeter);
      s += TYPE_STRING(b.biClrUsed);
      s += TYPE_STRING(b.biClrImportant);
      s += "}\n";
      return s;
    }
    std::string GetBitmapFileDescription(const char* path)
    {
      std::string desc;

      FILE* f = fopen(path, "rb");
      if (!f) return desc;

      #pragma pack(push, 1)
      struct FULL_BMP_HEADER
      {
        BITMAPFILEHEADER file_header;
        BITMAPINFOHEADER info_header;
      };
      #pragma pack(pop)

      FULL_BMP_HEADER fbh;
      size_t read_size = fread(&fbh, 1, sizeof(fbh), f);
      if (read_size != sizeof(fbh))
      {
        fclose(f);
        return desc;
      }

      desc += ToString(fbh.file_header);
      desc += ToString(fbh.info_header);

      fclose(f);
      return desc;
    }
    HBITMAP Create32bppBitmapFromIcon(const char * path, int nIconIndex)
    {
      std::string file_ext = ra::strings::Uppercase( ra::filesystem::GetFileExtention(path) );

      // Create a 32bpp bitmap with transparency using an icon from the shell.
      HICON hIconLarge = NULL;
      if (file_ext == "DLL")
      {
        UINT numIconLoaded = ExtractIconEx(path, nIconIndex, &hIconLarge, NULL, 1);
        if (numIconLoaded != 1) return NULL;
      }
      else if (file_ext == "ICO")
      {
        hIconLarge = (HICON)LoadImage(NULL, path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
      }
      if (hIconLarge == NULL) return NULL;

      //Convert the icon to a bitmap (with invisible background)
      SIZE icon_size = Win32Utils::GetIconSize(hIconLarge);
      HBITMAP hBitmap = Win32Utils::CopyAsBitmap(hIconLarge, icon_size.cx, icon_size.cy);

      DestroyIcon(hIconLarge);

      return hBitmap;
    }
    TEST_F(TestWin32Utils, testSaveBitmapToFile)
    {
      struct BITMAP_METADATA
      {
        const char* path;
        int bit_per_pixels;
        uint32_t load_flags;
        bool check_integrity;
      };
      static const BITMAP_METADATA bitmaps[] = {
        {"test_files\\Smiley.40x60.1bpp.bmp" ,       1, LR_MONOCHROME        ,    true },
        {"test_files\\Smiley.80x80.1bpp.bmp" ,       1, LR_MONOCHROME        ,    true },
        {"test_files\\Smiley.40x60.4bpp.bmp" ,       4, LR_CREATEDIBSECTION  ,    true },
        {"test_files\\Smiley.80x80.4bpp.bmp" ,       4, LR_CREATEDIBSECTION  ,    true },
        {"test_files\\Smiley.80x80.8bpp.bmp" ,       8, LR_CREATEDIBSECTION  ,    true },
        {"test_files\\Smiley.80x80.24bpp.bmp",      24, LR_CREATEDIBSECTION  ,    true },
        {"test_files\\Smiley.80x80.32bpp.bmp",      32, 0                    ,    false}, // the file was generated by gimp. 72 DPI instead of 96 DPI.
        {"c:\\windows\\system32\\shell32.dll",      32, 0                    ,    false},
        {"test_files\\issue117\\alarm_clock-256-32bpp-8ba-compressed.ico", 32, 0, false},
      };
      static const size_t num_bitmaps = sizeof(bitmaps) / sizeof(bitmaps[0]);

      for (int i = 0; i < num_bitmaps; i++)
      {
        std::string bitmap_path = ra::filesystem::GetPathBasedOnCurrentProcess(bitmaps[i].path);
        std::string file_ext = ra::strings::Uppercase( ra::filesystem::GetFileExtention(bitmap_path) );

        ASSERT_TRUE(ra::filesystem::FileExists(bitmap_path.c_str())) << "File does not exists: " << bitmap_path;

        HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, bitmap_path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | bitmaps[i].load_flags);
        if (hBitmap == NULL)
          hBitmap = Win32Utils::LoadBitmapFromFile(bitmap_path.c_str()); // LoadImage() do not support 32bpp bitmap. Try with LoadBitmapFromFile() and hope we load the image.
        if (hBitmap == NULL && file_ext == "DLL")
        {
          const int nIconIndex = 3;
          hBitmap = Create32bppBitmapFromIcon(bitmap_path.c_str(), nIconIndex);
          bitmap_path += "." + ra::strings::ToString(nIconIndex) + ".bmp"; //fuzz the filename
        }
        if (hBitmap == NULL && file_ext == "ICO")
        {
          hBitmap = Create32bppBitmapFromIcon(bitmap_path.c_str(), 0);
          bitmap_path += ".bmp"; //fuzz the filename
        }
        ASSERT_TRUE(hBitmap != NULL) << "Failed loading file: " << bitmaps[i].path;

        // Check that we loaded the bitmap properly.
        int actual_bits_per_pixels = Win32Utils::GetBitPerPixel(hBitmap);
        int expected_bits_per_pixels = bitmaps[i].bit_per_pixels;
        ASSERT_EQ(expected_bits_per_pixels, actual_bits_per_pixels) << "Incorrect bits per pixels reported: " << bitmaps[i].path;

        // build output files
        std::string test_dir = ra::filesystem::GetTemporaryDirectory() + "\\" + ra::testing::GetTestQualifiedName();
        ASSERT_TRUE(ra::filesystem::CreateDirectory(test_dir.c_str()));
        std::string bitmap_filename = ra::filesystem::GetFilename(bitmap_path.c_str());
        std::string output_path = test_dir + "\\" + bitmap_filename;

        // remove data from previous runs
        ra::filesystem::DeleteFile(output_path.c_str());

        // Save as *.bmp
        bool saved = Win32Utils::SaveBitmapFile(output_path.c_str(), hBitmap);
        ASSERT_TRUE(saved) << "Failed to save bitmap to file: " << output_path;

        // If we compare bitmaps, check that output is identical to the input.
        if (bitmaps[i].check_integrity)
        {
          std::string reason;
          bool equals = ra::testing::IsFileEquals(bitmap_path.c_str(), output_path.c_str(), reason, 0);
          std::string desc1 = GetBitmapFileDescription(bitmap_path.c_str());
          std::string desc2 = GetBitmapFileDescription(output_path.c_str());
          reason += "\n";
          reason += "original:\n";
          reason += desc1;
          reason += "\n";
          reason += "generated:\n";
          reason += desc2;
          ASSERT_TRUE(equals) << reason;
        }

        //cleanup
        DeleteObject(hBitmap);
      }
    }
    //--------------------------------------------------------------------------------------------------

  } //namespace test
} //namespace shellanything
