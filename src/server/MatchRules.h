#ifndef AP_MATCHRULES_H
#define AP_MATCHRULES_H

#include <OGRE/OgreConfigFile.h>
#include <map>

#include "../functions.h"
#include "../types.h"
#include "../ScoreListing.h"

/**
 * The MatchRules class takes care of loading, storing and applying the Rules of the Match.
 * This also means ruling over when and if players respawn.
 */


class MatchRules
{
    std::string GameRules;
    int RespawnInterval, RespawnDelay;
    int NumberOfTeams;
    int PlayerExtraLives, TeamExtraLives, TeamTotalLives;
    int KillEnemyScore, KillTeamScore, KillSelfScore, KilledByEnemyScore, KilledByTeamScore;
    int VictoryByPersonalKills, VictoryByPersonalScore, VictoryByTeamKills, VictoryByTeamScore;

    Ogre::ConfigFile rulesConfig;

    std::map<ap::uint32, ap::uint32>    pendingSpawners; // ticks value when respawn, uid whom to respawn
    int gameStarted;    // 0 means no game going

 public:
    MatchRules();
    bool load(std::string rulename, std::string filename);

    void killPlayer(ap::ScoreListing *pScores, ap::uint32 uidDies, ap::uint32 uidKills);

    ap::uint32 addSpawner(ap::uint32 uid);
    ap::uint32 getSpawner();

    void startGame();
};




#endif
