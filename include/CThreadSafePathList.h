/*
 * Copyright (c) 2016 Seven Bridges Genomics
 * Author: Berke Cagkan Toptas
 */

//TODO: Read/Write lock for multithread support will be added

#ifndef _C_THREAD_SAFE_PATH_STACK_H_
#define _C_THREAD_SAFE_PATH_STACK_H_

#include "CPath.h"
#include <set>

class CThreadSafePathList
{
  public:

    CThreadSafePathList();
    
    int Size() const;

    void Clear();

    void Add(const CPath& item);

    void Erase(const CPath& item);
    
    // Get the least advanced path
    CPath GetLeastAdvanced();

    bool Empty();

    bool Contains(const CPath& item) const;

    CPath floor(const CPath& a_rObj);

  private:
    
    std::set<CPath> m_set;

};

#endif //_C_THREAD_SAFE_PATH_STACK_H_
