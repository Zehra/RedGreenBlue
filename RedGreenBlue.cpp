// RedGreenBlue.cpp
// ================
// red can kill blue,but red die when they kill green
// blue can kill green,but blue die when they kill red
// green can kill red,but green die when they kill blue
// Classic three team game.

#include "bzfsAPI.h"

class RedGreenBlue : public bz_Plugin
{
public:
    virtual const char* Name(){return "RedGreenBlue";}
    virtual void Init ( const char* /*config*/ );
    virtual void Event(bz_EventData *eventData );
    virtual void Cleanup ( void );
    
    // Utility functions.
    void tkKillPlayer(int killerID) {
        bz_incrementPlayerWins(killerID, -1);
        //bz_incrementPlayerLosses(killerID, 1); // No need for loss increase, since self-kill increases loss by 1.
        bz_killPlayer(killerID, false, killerID);
    }
    
    int checkIfTeams(bz_eTeamType playersTeam, bz_eTeamType killersTeam) {
        if ((killersTeam == eRedTeam) && (playersTeam == eGreenTeam)) {
            return 1;
        } else if ((killersTeam == eBlueTeam) && (playersTeam == eRedTeam)) {
            return 2;
        } else if  ((killersTeam == eGreenTeam) && (playersTeam == eBlueTeam)) {
            return 3;
        } else {
             return 0;
        }
    
    }
    
};

BZ_PLUGIN(RedGreenBlue)

void RedGreenBlue::Init (const char*config) {
    Register(bz_ePlayerDieEvent);
}

void RedGreenBlue::Cleanup (void) {
    Flush();
}

void RedGreenBlue::Event(bz_EventData *eventData ){

    if (eventData->eventType != bz_ePlayerDieEvent)
        return;
  
    bz_PlayerDieEventData_V1 *deathData = (bz_PlayerDieEventData_V1*)eventData;

    if ((deathData->playerID != deathData->killerID) && (deathData->killerID != 253)) {

        bz_eTeamType playerTeam = bz_getPlayerTeam(deathData->playerID);
        bz_eTeamType killerTeam = bz_getPlayerTeam(deathData->killerID);

        int value = checkIfTeams(playerTeam, killerTeam);
    
        if (value != 0) {
            tkKillPlayer(deathData->killerID);
        }
    }
}
