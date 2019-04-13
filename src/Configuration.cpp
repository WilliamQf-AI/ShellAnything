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

#include "shellanything/Configuration.h"

namespace shellanything
{

  Configuration::Configuration() : Node("Configuration"),
    mFileModifiedDate(0)
  {
  }

  Configuration::~Configuration()
  {
  }

  const std::string & Configuration::getFilePath() const
  {
    return mFilePath;
  }

  void Configuration::setFilePath(const std::string & iFilePath)
  {
    mFilePath = iFilePath;
  }

  const uint64_t & Configuration::getFileModifiedDate() const
  {
    return mFileModifiedDate;
  }

  void Configuration::setFileModifiedDate(const uint64_t & iFileModifiedDate)
  {
    mFileModifiedDate = iFileModifiedDate;
  }

  Item * Configuration::findItemByCommandId(const uint32_t & iCommandId)
  {
    //for each child
    Item::ItemPtrList children = getItems();
    for(size_t i=0; i<children.size(); i++)
    {
      Item * child = children[i];
      Item * match = child->findItemByCommandId(iCommandId);
      if (match)
        return match;
    }
 
    return NULL;
  }
 
  uint32_t Configuration::assignCommandIds(const uint32_t & iFirstCommandId)
  {
    uint32_t nextCommandId = iFirstCommandId;

    //for each child
    Item::ItemPtrList children = getItems();
    for(size_t i=0; i<children.size(); i++)
    {
      Item * child = children[i];
      nextCommandId = child->assignCommandIds(nextCommandId);
    }
 
    return nextCommandId;
  }
 
  Item::ItemPtrList Configuration::getItems()
  {
    Item::ItemPtrList sub_items = filterNodes<Item*>(this->findChildren("Item"));
    return sub_items;
  }

} //namespace shellanything
