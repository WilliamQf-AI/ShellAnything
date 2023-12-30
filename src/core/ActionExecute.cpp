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

#include "ActionExecute.h"
#include "rapidassist/process_utf8.h"
#include "rapidassist/unicode.h"
#include "rapidassist/filesystem_utf8.h"
#include "rapidassist/timing.h"
#include "PropertyManager.h"
#include "ObjectFactory.h"
#include "LoggerHelper.h"

#include <windows.h>

#include "tinyxml2.h"
using namespace tinyxml2;


namespace shellanything
{
  const std::string ActionExecute::XML_ELEMENT_NAME = "exec";

  class ActionExecuteFactory : public virtual IActionFactory
  {
  public:
    ActionExecuteFactory()
    {
    }
    virtual ~ActionExecuteFactory()
    {
    }

    virtual const std::string& GetName() const
    {
      return ActionExecute::XML_ELEMENT_NAME;
    }

    virtual IAction* ParseFromXml(const std::string& xml, std::string& error) const
    {
      tinyxml2::XMLDocument doc;
      XMLError result = doc.Parse(xml.c_str());
      if (result != XML_SUCCESS)
      {
        if (doc.ErrorStr())
        {
          error = doc.ErrorStr();
          return NULL;
        }
        else
        {
          error = "Unknown error reported by XML library.";
          return NULL;
        }
      }
      XMLElement* element = doc.FirstChildElement(GetName().c_str());

      ActionExecute* action = new ActionExecute();
      std::string tmp_str;

      //parse path
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "path", false, true, tmp_str, error))
      {
        action->SetPath(tmp_str);
      }

      //parse arguments
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "arguments", true, true, tmp_str, error))
      {
        action->SetArguments(tmp_str);
      }

      //parse basedir
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "basedir", true, true, tmp_str, error))
      {
        action->SetBaseDir(tmp_str);
      }

      //parse verb
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "verb", true, true, tmp_str, error))
      {
        action->SetVerb(tmp_str);
      }

      //parse wait
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "wait", true, true, tmp_str, error))
      {
        action->SetWait(tmp_str);
      }

      //parse timeout
      tmp_str = "";
      if (ObjectFactory::ParseAttribute(element, "timeout", true, true, tmp_str, error))
      {
        action->SetTimeout(tmp_str);
      }

      //done parsing
      return action;
    }

  };

  IActionFactory* ActionExecute::NewFactory()
  {
    return new ActionExecuteFactory();
  }

  ActionExecute::ActionExecute()
  {
  }

  ActionExecute::~ActionExecute()
  {
  }

  bool ActionExecute::Execute(const SelectionContext& context) const
  {
    PropertyManager& pmgr = PropertyManager::GetInstance();
    std::string verb = pmgr.Expand(mVerb);
    std::string timeout = pmgr.Expand(mTimeout);

    // Validate that timeout is valid
    if (!timeout.empty())
    {
      uint32_t tmp = 0;
      bool parsed = ra::strings::Parse(timeout, tmp);
      if (!parsed)
      {
        SA_LOG(ERROR) << "Failed parsing time out value: '" << timeout << "'.";
        return false;
      }
    }

    //If a verb was specified, delegate to VerbExecute(). Otherwise, use ProcessExecute().
    if (verb.empty())
      return ExecuteProcess(context);
    else
      return ExecuteVerb(context);
  }

  bool ActionExecute::ExecuteVerb(const SelectionContext& context) const
  {
    PropertyManager& pmgr = PropertyManager::GetInstance();
    std::string path = pmgr.Expand(mPath);
    std::string basedir = pmgr.Expand(mBaseDir);
    std::string arguments = pmgr.Expand(mArguments);
    std::string verb = pmgr.Expand(mVerb);
    std::string wait = pmgr.Expand(mWait);
    std::string timeout_str = pmgr.Expand(mTimeout);

    std::wstring pathW = ra::unicode::Utf8ToUnicode(path);
    std::wstring argumentsW = ra::unicode::Utf8ToUnicode(arguments);
    std::wstring basedirW = ra::unicode::Utf8ToUnicode(basedir);
    std::wstring verbW = ra::unicode::Utf8ToUnicode(verb);

    SHELLEXECUTEINFOW info = { 0 };

    info.cbSize = sizeof(SHELLEXECUTEINFOW);

    info.fMask |= SEE_MASK_NOCLOSEPROCESS;
    info.fMask |= SEE_MASK_NOASYNC;
    info.fMask |= SEE_MASK_FLAG_DDEWAIT;

    info.hwnd = HWND_DESKTOP;
    info.nShow = SW_SHOWDEFAULT;
    info.lpFile = pathW.c_str();

    //Print execute values in the logs
    SA_LOG(INFO) << "Path: " << path;
    if (!verb.empty())
    {
      info.lpVerb = verbW.c_str(); // Verb
      SA_LOG(INFO) << "Verb: " << verb;
    }
    if (!arguments.empty())
    {
      info.lpParameters = argumentsW.c_str(); // Arguments
      SA_LOG(INFO) << "Arguments: " << arguments;
    }
    if (!basedir.empty())
    {
      info.lpDirectory = basedirW.c_str(); // Default directory
      SA_LOG(INFO) << "Basedir: " << basedir;
    }

    //Execute and get the pid
    bool success = (ShellExecuteExW(&info) == TRUE);
    if (!success)
      return false;
    DWORD pId = GetProcessId(info.hProcess);

    // Check valid process
    success = (pId != ra::process::INVALID_PROCESS_ID);
    if (!success)
    {
      SA_LOG(WARNING) << "Failed to create process.";
      return false;
    }
    SA_LOG(INFO) << "Process created. PID=" << pId;

    // Check for wait exit code
    bool wait_success = WaitForExit(pId);
    if (!wait_success)
    {
      SA_LOG(WARNING) << "Timed out! The process with PID=" << pId << " has failed to exit before the specified timeout.";
      return false;
    }

    return wait_success;
  }

  bool ActionExecute::ExecuteProcess(const SelectionContext& context) const
  {
    PropertyManager& pmgr = PropertyManager::GetInstance();
    std::string path = pmgr.Expand(mPath);
    std::string basedir = pmgr.Expand(mBaseDir);
    std::string arguments = pmgr.Expand(mArguments);
    std::string wait = pmgr.Expand(mWait);
    std::string timeout_str = pmgr.Expand(mTimeout);

    bool basedir_missing = basedir.empty();
    bool arguments_missing = arguments.empty();

    //Note: the startProcess() function requires basedir to be valid.
    //If not specified try to fill basedir with the best option available
    if (basedir.empty())
    {
      const StringList& elements = context.GetElements();
      size_t num_dir = context.GetNumDirectories();
      size_t num_files = context.GetNumFiles();
      if (num_dir == 1 && elements.size() == 1)
      {
        //if a single directory was selected
        basedir = elements[0];
      }
      else if (num_files == 1 && elements.size() == 1)
      {
        //if a single file was selected
        const std::string& selected_file = elements[0];
        const std::string parent_dir = ra::filesystem::GetParentPath(selected_file);
        basedir = parent_dir;
      }
      else if (num_dir == 0 && num_files >= 2 && elements.size() >= 1)
      {
        //if a multiple files was selected
        const std::string& first_selected_file = elements[0];
        const std::string first_parent_dir = ra::filesystem::GetParentPath(first_selected_file);
        basedir = first_parent_dir;
      }
    }

    //warn the user if basedir was modified
    if (basedir_missing && !basedir.empty())
    {
      SA_LOG(INFO) << "Warning: 'basedir' not specified. Setting basedir to '" << basedir << "'.";
    }

    if (basedir.empty())
    {
      SA_LOG(WARNING) << "attribute 'basedir' not specified.";
    }

    //Print execute values in the logs
    SA_LOG(INFO) << "Path: " << path;
    if (!arguments.empty())
    {
      SA_LOG(INFO) << "Arguments: " << arguments;
    }
    if (!basedir.empty())
    {
      SA_LOG(INFO) << "Basedir: " << basedir;
    }

    //Execute and get the pid
    uint32_t pId = ra::process::INVALID_PROCESS_ID;
    if (arguments_missing)
    {
      pId = ra::process::StartProcessUtf8(path, basedir);
    }
    else
    {
      pId = ra::process::StartProcessUtf8(path, basedir, arguments);
    }

    // Check valid process
    bool success = (pId != ra::process::INVALID_PROCESS_ID);
    if (!success)
    {
      SA_LOG(WARNING) << "Failed to create process.";
      return false;
    }
    SA_LOG(INFO) << "Process created. PID=" << pId;

    // Check for wait exit code
    bool wait_success = WaitForExit(pId);
    if (!wait_success)
    {
      SA_LOG(WARNING) << "Timed out! The process with PID=" << pId << " has failed to exit before the specified timeout.";
      return false;
    }

    return wait_success;
  }

  bool ActionExecute::WaitForExit(uint32_t pId) const
  {
    PropertyManager& pmgr = PropertyManager::GetInstance();
    std::string wait = pmgr.Expand(mWait);
    std::string timeout_str = pmgr.Expand(mTimeout);

    if (wait.empty())
      return true; // nothing to do

    bool wait_for_exit_code = Validator::IsTrue(wait);
    if (!wait_for_exit_code)
      return true; // nothing to do

    // Compute the timeout
    static const uint32_t MAX_TIMEOUT_VALUE = (uint32_t)(-1);
    uint32_t timeout_ms = MAX_TIMEOUT_VALUE;
    if (!timeout_str.empty())
    {
      bool parsed = ra::strings::Parse(timeout_str, timeout_ms);
      if (!parsed)
      {
        SA_LOG(ERROR) << "Failed parsing time out value: '" << timeout_str << "'.";
        return false;
      }

      // Convert from seconds to milliseconds
      timeout_ms *= 1000;
    }

    // Define sleep interval
    static const uint32_t DEFAULT_SLEEP_INTERVAL_MS = 250;
    uint32_t sleep_interval_ms = DEFAULT_SLEEP_INTERVAL_MS;
    if (timeout_ms != MAX_TIMEOUT_VALUE)
    {
      if (timeout_ms > 60*1000)
        sleep_interval_ms = 5000;
      else if (timeout_ms > 10*1000)
        sleep_interval_ms = 1000;
      else if (timeout_ms == 1*1000)
        sleep_interval_ms = 50;
    }

    // Wait for the process to complete before returning
    SA_LOG(INFO) << "Waiting for process with PID=" << pId << " to exit.";

    ra::process::processid_t tmp = (ra::process::processid_t)pId;
    int exit_code = 0;
    bool found = false;
    uint64_t start = ra::timing::GetMillisecondsCounterU64();
    uint64_t elapsed = 0;
    while (!found && elapsed < timeout_ms)
    {
      bool success = ra::process::GetExitCode(tmp, exit_code);
      if (success)
        found = true;

      if (!found)
      {
        ra::timing::Millisleep(sleep_interval_ms);
        elapsed = ra::timing::GetMillisecondsCounterU64() - start;
      }
    }

    SA_LOG(INFO) << "The process with PID=" << pId << " has return with exit code " << exit_code;

    // Did we timed out?
    return found;
  }

  const std::string& ActionExecute::GetPath() const
  {
    return mPath;
  }

  void ActionExecute::SetPath(const std::string& path)
  {
    mPath = path;
  }

  const std::string& ActionExecute::GetBaseDir() const
  {
    return mBaseDir;
  }

  void ActionExecute::SetBaseDir(const std::string& base_dir)
  {
    mBaseDir = base_dir;
  }

  const std::string& ActionExecute::GetArguments() const
  {
    return mArguments;
  }

  void ActionExecute::SetArguments(const std::string& arguments)
  {
    mArguments = arguments;
  }

  const std::string& ActionExecute::GetVerb() const
  {
    return mVerb;
  }

  void ActionExecute::SetVerb(const std::string& verb)
  {
    mVerb = verb;
  }

  const std::string& ActionExecute::GetWait() const
  {
    return mWait;
  }

  void ActionExecute::SetWait(const std::string& value)
  {
    mWait = value;
  }

  const std::string& ActionExecute::GetTimeout() const
  {
    return mTimeout;
  }

  void ActionExecute::SetTimeout(const std::string& value)
  {
    mTimeout = value;
  }

} //namespace shellanything
