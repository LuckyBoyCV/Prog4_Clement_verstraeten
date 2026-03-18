#pragma once
#ifdef USE_STEAMWORKS
#include <steam_api.h>
#include <Windows.h>

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
    int m_eAchievementID;
    const char* m_pchAchievementID;
    char m_rgchName[128];
    char m_rgchDescription[256];
    bool m_bAchieved;
    int m_iIconImage;
};

class CSteamAchievements
{
private:
    int64 m_iAppID;
    Achievement_t* m_pAchievements;
    int m_iNumAchievements;
    bool m_bInitialized;

public:
    CSteamAchievements(Achievement_t* Achievements, int NumAchievements);
    ~CSteamAchievements();
    bool Initialize();
    bool SetAchievement(const char* ID);

    STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
    STEAM_CALLBACK(CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};

#endif // USE_STEAMWORKS