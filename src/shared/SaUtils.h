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

#ifndef SA_UTILS_H
#define SA_UTILS_H

#include <string>

/// <summary>
/// Get the current module path. This file must be included only in DLL and not static libraries.
/// </summary>
/// <returns>Returns the path of the current DLL if successful. Returns an empty string on error.</returns>
std::string GetCurrentModulePath();

/// <summary>
/// Get the current module path encoded in UTF-8. This file must be included only in DLL and not static libraries.
/// </summary>
/// <returns>Returns the path of the current DLL if successful. Returns an empty string on error.</returns>
std::string GetCurrentModulePathUtf8();

/// <summary>
/// Test if a directory has write access.
/// </summary>
/// <returns>Returns true if write access is granted. Returns false otherwise.</returns>
bool HasDirectoryWriteAccess(const std::string& path);

/// <summary>
/// Returns true if the application is run for the first time.
/// Note, for Windows users, the implementation is based on registry keys in HKEY_CURRENT_USER\Software\name\version.
/// </summary>
/// <param name="name">The name of the application.</param>
/// <param name="version">The version of the application.</param>
/// <returns>Returns true if the application is run for the first time. Returns false otherwise.</returns>
bool IsFirstApplicationRun(const std::string& name, const std::string& version);

/// <summary>
/// Check if the given character is printable.
/// </summary>
/// <param name="c">The character to check.</param>
/// <returns>Returns true if the given character is printable. Returns false otherwise.</returns>
bool IsPrintable(const char c);

/// <summary>
/// Check if the given string is printable.
/// </summary>
/// <param name="c">The string to check.</param>
/// <returns>Returns true if the given string is printable. Returns false otherwise.</returns>
bool IsPrintable(const std::string& value);

/// <summary>
/// Check if the given string is printable.
/// </summary>
/// <param name="value">The string to check.</param>
/// <returns>Returns true if the given string is printable. Returns false otherwise.</returns>
bool IsPrintableUtf8(const std::string& value);

/// <summary>
/// Compute the length in bytes of the first code point of a string.
/// If the first character is non-unicode, a length of 1 bytes is returned.
/// If the first character is an UTF8 control character, the length in bytes of the control character is returned.
/// </summary>
/// <param name="value">The string which contains the first code point to check.</param>
/// <returns>Returns the length of the first code point, in bytes. Returns std::string::npos on error.</returns>
size_t GetFirstCodePointLengthUtf8(const char * value);

#endif //SA_UTILS_H
