using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[System.Serializable]
public class EncounterState
{
    public int id;
    public string encounterName;
    public bool isStaticEncounter;

    public List<CharacterState> NPCs;

    private Dictionary<int, CharacterState> NPCsByID_;
    public Dictionary<int, CharacterState> NPCsByID
    {
        get
        {
            if(NPCsByID_ == default)
            {
                NPCsByID_ = NPCs.ToDictionary(npc => npc.id, npc => npc);
            }

            return NPCsByID_;            
        }
    }
}
