#ifndef SYSTEM_SERVICE_ENTRY_H
#define SYSTEM_SERVICE_ENTRY_H

#include "system_service_entry_data.h"

class SystemServiceEntry
{
public:
    SystemServiceEntry();
    SystemServiceEntry(const QString &id, const QString &loadState, const QString &activeState,
                       const QString &subState, const QString &state, const QString &unitObjectPath,
                       const QString &description, quint32 mainPID, bool canReload, bool canStart,
                       bool canStop);
    SystemServiceEntry(const SystemServiceEntry &);
    SystemServiceEntry &operator=(const SystemServiceEntry &);
    ~SystemServiceEntry();

    //////////////////////////////////// GET ///////////////////////////////////

    inline QString getId() const { return data->m_id; }
    inline QString getLoadState() const { return data->m_loadState; }
    inline QString getActiveState() const { return data->m_activeState; }
    inline QString getSubState() const { return data->m_subState; }
    inline QString getState() const { return data->m_state; }
    inline QString getUnitObjectPath() const { return data->m_unitObjectPath; }
    inline QString getDescription() const { return data->m_description; }
    inline quint32 getMainPID() const { return data->m_mainPID; }
    inline bool getCanReload() const { return data->m_canReload; }
    inline bool getCanStart() const { return data->m_canStart; }
    inline bool getCanStop() const { return data->m_canStop; }

    //////////////////////////////////// SET ///////////////////////////////////

    inline void setId(const QString &id) { data->m_id = id; }
    inline void setLoadState(const QString &loadState) { data->m_loadState = loadState; }
    inline void setActiveState(const QString &activeState) { data->m_activeState = activeState; }
    inline void setSubState(const QString &subState) { data->m_subState = subState; }
    inline void setState(const QString &state) { data->m_state = state; }
    inline void setUnitObjectPath(const QString &unitObjectPath)
    {
        data->m_unitObjectPath = unitObjectPath;
    }
    inline void setDescription(const QString &description) { data->m_description = description; }
    inline void setMainPID(quint32 mainPID) { data->m_mainPID = mainPID; }
    inline void setCanReload(bool canReload) { data->m_canReload = canReload; }
    inline void setCanStart(bool canStart) { data->m_canStart = canStart; }
    inline void setCanStop(bool canStop) { data->m_canStop = canStop; }

private:
    QExplicitlySharedDataPointer<SystemServiceEntryData> data;
};

#endif  // SYSTEM_SERVICE_ENTRY_H
