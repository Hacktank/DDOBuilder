// ObserverSubject.cpp
//
// Implementation of the Observer-Subject pattern
#include "stdafx.h"
#include "ObserverSubject.h"
#include "Utilities.h"

//////////////////////////////////////////////////////////////////////

ObserverBase::ObserverBase()
{
}

ObserverBase::~ObserverBase()
{
    DetachAll();
}

// a copied observer DOES observe all the original's subjects
ObserverBase::ObserverBase(const ObserverBase & copy)
{
    AttachAll(copy);
}

ObserverBase & ObserverBase::operator =(const ObserverBase & copy)
{
    DetachAll();
    AttachAll(copy);
    return *this;
}

bool ObserverBase::InsertSubjectBase(SubjectBase * subject)
{
    CriticalSectionLock lock(&m_critsec);

    bool alreadyPresent = false;

    // If it is not already present, add to the beginning of the list
    for (SubjectList::iterator it = m_subjects.begin();
            it != m_subjects.end() && !alreadyPresent;
            it++)
    {
        if (*it == subject) 
        { 
            alreadyPresent = true;
        }
    }

    if (!alreadyPresent)
    {
        m_subjects.push_back(subject);
    }

    // Turn the logic upside down.
    bool hasBeenAdded = !alreadyPresent;
    return hasBeenAdded;
}

bool ObserverBase::RemoveSubjectBase(SubjectBase * subject)
{
    CriticalSectionLock lock(&m_critsec);

    bool wasRemoved = false;
    SubjectList::iterator it = m_subjects.begin();
    while (!wasRemoved
            && it != m_subjects.end()) 
    {
        if (*it == subject) 
        { 
            m_subjects.erase(it); 
            wasRemoved = true;
        }
        else
        {
            it++;
        }
    }

    return wasRemoved;
}

// drop all observed subjects
void ObserverBase::DetachAll()
{
    CriticalSectionLock lock(&m_critsec);
    for (SubjectList::iterator it = m_subjects.begin(); it != m_subjects.end(); it++)
    {
        (*it)->RemoveObserverBase(this);
    }
    m_subjects.clear();
}


void ObserverBase::AttachAll(const ObserverBase & copy)
{
    // stop any other thread messing with the subjects while we are messing
    CriticalSectionLock lock(&m_critsec);

    // keeping a lock on the copy ensures that all the subjects are valid (cannot be removed)
    CriticalSectionLock copyLock(&copy.m_critsec);
    m_subjects = copy.m_subjects;

    for (SubjectList::iterator it = m_subjects.begin(); it != m_subjects.end(); it++)
    {
        (*it)->InsertObserverBase(this);
    }
}


//////////////////////////////////////////////////////////////////////
SubjectBase::SubjectBase() :
    m_notificationLevel(0)
{
}

SubjectBase::~SubjectBase()
{
    DetachAll();
}

// a copied subject does NOT notify the original's observers
SubjectBase::SubjectBase(const SubjectBase & copy) :
    m_notificationLevel(0)
{
    UNREFERENCED_PARAMETER(copy);
}

SubjectBase & SubjectBase::operator=(const SubjectBase & copy)
{
    UNREFERENCED_PARAMETER(copy);
    DetachAll(); // copy over drops all the original observers
    return *this;
}

void SubjectBase::AttachObserverBase(ObserverBase * pObserver)
{
    if (InsertObserverBase(pObserver))
    {
        pObserver->InsertSubjectBase(this);
    }
}

void SubjectBase::DetachObserverBase(ObserverBase * pObserver)
{
    if (RemoveObserverBase(pObserver))
    {
        pObserver->RemoveSubjectBase(this);
    }
}

bool SubjectBase::InsertObserverBase(ObserverBase * pObserver)
{
    CriticalSectionLock lock(&m_critsec);

    // add the new observer to the end of the vector, because if we are currently in a notification cascade it could be missed if its not at the end
    for (auto& obs : m_observers) {
        if (obs == pObserver) {
            // already present
            return false;
        }
    }
    m_observers.push_back(pObserver);
}

bool SubjectBase::RemoveObserverBase(ObserverBase * pObserver)
{
    CriticalSectionLock lock(&m_critsec);

    for (auto& obs : m_observers) {
        if (obs == pObserver) {
            // cannot risk erasing an observer from the vector during a notification cascade, just null it and it will be erased later
            obs = NULL;
            return true;
        }
    }

    return false;
}

void SubjectBase::DetachAll()
{
    CriticalSectionLock lock(&m_critsec);
    ASSERT(m_notificationLevel == 0);
    for (ObserverList::iterator it = m_observers.begin(); it != m_observers.end(); it++)
    {
        (*it)->RemoveSubjectBase(this);
    }
    m_observers.clear();
}

void SubjectBase::BeginNotification() const
{
    CriticalSectionLock lock(&m_critsec);
    ++m_notificationLevel;
}

void SubjectBase::EndNotification() const
{
    CriticalSectionLock lock(&m_critsec);

    if (m_notificationLevel == 1)
    {
        // remove any observers that have been NULLed out during the notification
        m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), [](const auto& item) {return item == NULL; }), m_observers.end());
    }

    --m_notificationLevel;
}

SubjectBase::ObserverList::const_iterator SubjectBase::ObserversBegin() const
{
    return m_observers.begin();
}

SubjectBase::ObserverList::const_iterator SubjectBase::ObserversEnd() const
{
    return m_observers.end();
}
