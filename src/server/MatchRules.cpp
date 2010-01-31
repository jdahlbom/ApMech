#include "MatchRules.h"

#include <iostream>

using namespace ap;

MatchRules::MatchRules() :
    GameRules("deathmatch"), RespawnInterval(0), RespawnDelay(0),
    NumberOfTeams(0),
    PlayerExtraLives(0), TeamExtraLives(0), TeamTotalLives(0),
    KillEnemyScore(1), KillTeamScore(-1), KillSelfScore(-1), KilledByEnemyScore(-1), KilledByTeamScore(0),
    VictoryByPersonalKills(0), VictoryByPersonalScore(10), VictoryByTeamKills(0), VictoryByTeamScore(0),
    gameStarted(0)
{}


bool MatchRules::load(std::string rulename, std::string filename)
{
    rulesConfig.load(filename);
    std::string section = "Rules of " + GameRules;
    int32 tmp;

    if (from_string<int32>(tmp, rulesConfig.getSetting("RespawnInterval", section), std::dec)) RespawnInterval = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("RespawnDelay", section), std::dec)) RespawnDelay = tmp;

    if (from_string<int32>(tmp, rulesConfig.getSetting("NumberOfTeams", section), std::dec)) NumberOfTeams = tmp;

    if (from_string<int32>(tmp, rulesConfig.getSetting("PlayerExtraLives", section), std::dec)) PlayerExtraLives = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("TeamExtraLives", section), std::dec)) TeamExtraLives = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("TeamTotalLives", section), std::dec)) TeamTotalLives = tmp;

    if (from_string<int32>(tmp, rulesConfig.getSetting("KillEnemyScore", section), std::dec)) KillEnemyScore = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("KillTeamScore", section), std::dec)) KillTeamScore = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("KillSelfScore", section), std::dec)) KillSelfScore = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("KilledByEnemyScore", section), std::dec)) KilledByEnemyScore = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("KilledByTeamScore", section), std::dec)) KilledByTeamScore = tmp;

    if (from_string<int32>(tmp, rulesConfig.getSetting("VictoryByPersonalKills", section), std::dec)) VictoryByPersonalKills = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("VictoryByPersonalScore", section), std::dec)) VictoryByPersonalScore = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("VictoryByTeamKills", section), std::dec)) VictoryByTeamKills = tmp;
    if (from_string<int32>(tmp, rulesConfig.getSetting("VictoryByTeamScore", section), std::dec)) VictoryByTeamScore = tmp;

    std::cout << "[MATCHRULES] respawnDelay = "<<RespawnDelay<<std::endl;
    return true;
}


void MatchRules::killPlayer(ap::ScoreListing *pScores, uint32 uidDies, uint32 uidKills)
{
    if (uidDies == uidKills) {   // He killed himself!
        ap::ScoreTuple selfKiller;
        selfKiller.uid = uidDies;
        selfKiller.kills = 0;
        selfKiller.deaths = 1;
        selfKiller.score = KillSelfScore;
        pScores->addScore(selfKiller, false);
    } else {                        // update scores normally.
        if (NumberOfTeams == 0) {       // no teams, everybody else is a valid enemy
            ap::ScoreTuple diesTuple, killsTuple;

            diesTuple.uid = uidDies;
            diesTuple.kills = 0; diesTuple.deaths = 1; diesTuple.score = KilledByEnemyScore;
            pScores->addScore(diesTuple, false);

            killsTuple.uid = uidKills;
            killsTuple.kills = 1; killsTuple.deaths = 0; killsTuple.score = KillEnemyScore;
            pScores->addScore(killsTuple, false);
        } else {
            std::cout << "[MATCHRULES] ERROR: Teams not implemented yet!"<<std::endl;
        }

    }

    addSpawner(uidDies);
}


uint32 MatchRules::addSpawner(uint32 uid)
{
    if (RespawnInterval < 1) pendingSpawners.insert(std::make_pair(getTicks() + RespawnDelay, uid));
    else pendingSpawners.insert(std::make_pair( (((getTicks()+RespawnDelay) / RespawnInterval) + 1) * RespawnInterval , uid));
}

/**
 * Get the uid of somebody who's allowed to spawn a mech. Returns 0 when there are no more
 * spawners, otherwise it's best to call this again immediately.
 */

uint32 MatchRules::getSpawner() {
    std::map<ap::uint32, ap::uint32>::iterator i = pendingSpawners.begin();
    while (i != pendingSpawners.end()) {
        if (getTicks() > i->first) {
            ap::uint32 spawner = i->second;
            pendingSpawners.erase(i);
            return spawner;
        } else i++;
    }
    return 0;
}


