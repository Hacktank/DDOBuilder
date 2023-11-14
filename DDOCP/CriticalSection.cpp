// CriticalSection.cpp
//
// Implementation of critical section wrapper
#include "StdAfx.h"
#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
    InitializeCriticalSection(&m_criticalSection);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection(&m_criticalSection);
}

void CriticalSection::lock() const {
    EnterCriticalSection(&m_criticalSection);
}

void CriticalSection::unlock() const {
    ASSERT(m_criticalSection.OwningThread);

    if (m_criticalSection.OwningThread)
        LeaveCriticalSection(&m_criticalSection);
}

//////////////////////////////////////////////////////////////////////

// Take a lock on the critical section. This will be released by the destructor.
CriticalSectionLock::CriticalSectionLock(const CriticalSection* critSec)
{
    ASSERT(critSec);
    m_pCriticalSection = critSec;
    m_pCriticalSection->lock();
}

// Release a lock on the critical section.
CriticalSectionLock::~CriticalSectionLock()
{
    if (m_pCriticalSection != NULL)
        m_pCriticalSection->unlock();
}

