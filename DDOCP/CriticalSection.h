// CriticalSection.h
//
// This is a wrapper for the operating system critical section
// objects. These allow a resource to be protected so that only
// one object can use it at once (lock the critical section).
// The lock class allows a lock to be taken, and then dropped
// automatically when the lock goes out of scope (is destroyed).
// This dropping of locks is particularly important for
// exceptions handling, so that the lock is dropped if an
// exception occurs and the normal flow of control is not followed.

#pragma once
#include <windows.h>

class CriticalSection
{
    public:
        CriticalSection();
        ~CriticalSection();

        void lock() const;
        void unlock() const;

    private:
        // cannot be copied or assigned
        CriticalSection(const CriticalSection & other) = delete;
        CriticalSection& operator=(const CriticalSection & other) = delete;

        mutable CRITICAL_SECTION m_criticalSection;

        friend class CriticalSectionLock;
};

class CriticalSectionLock
{
    public:
        CriticalSectionLock(const CriticalSection* critSec);
        virtual ~CriticalSectionLock();

        // move constructor
        CriticalSectionLock(CriticalSectionLock&& other) noexcept {
            m_pCriticalSection = other.m_pCriticalSection;
            other.m_pCriticalSection = nullptr;
        }

        // move assignment only
        CriticalSectionLock& operator=(CriticalSectionLock&& other) noexcept {
            m_pCriticalSection = other.m_pCriticalSection;
            other.m_pCriticalSection = nullptr;
        }

    private:
        // no copying
        CriticalSectionLock(CriticalSectionLock& other) = delete;
        CriticalSectionLock& operator=(const CriticalSectionLock& other) = delete;

        mutable const CriticalSection * m_pCriticalSection;
};
