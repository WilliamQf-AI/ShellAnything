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

#ifndef SA_CONSOLE_LOGGER_SERVICE_H
#define SA_CONSOLE_LOGGER_SERVICE_H

#include "ILoggerService.h"

namespace shellanything
{
  /// <summary>
  /// Abstract logger class.
  /// </summary>
  class SHELLANYTHING_EXPORT ConsoleLoggerService : public virtual ILoggerService
  {
  public:
    ConsoleLoggerService();
    virtual ~ConsoleLoggerService();

  private:
    // Disable and copy constructor, dtor and copy operator
    ConsoleLoggerService(const ConsoleLoggerService&);
    ConsoleLoggerService& operator=(const ConsoleLoggerService&);
  public:

    /// <summary>
    /// Send a message to this logger.
    /// </summary>
    /// <param name="filename">The originating source code file name.</param>
    /// <param name="line">The line number that producing this message.</param>
    /// <param name="level">The log level of the message.</param>
    /// <param name="message">The actual message.</param>
    virtual void LogMessage(const char* filename, int line, const ILoggerService::LOG_LEVEL & level, const char* message);

    /// <summary>
    /// Send a message to this logger.
    /// </summary>
    /// <param name="level">The log level of the message.</param>
    /// <param name="message">The actual message.</param>
    virtual void LogMessage(const ILoggerService::LOG_LEVEL & level, const char* message);

  };

} //namespace shellanything

#endif //SA_CONSOLE_LOGGER_SERVICE_H
