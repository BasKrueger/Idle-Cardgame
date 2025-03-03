using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class GameState
{
    public PlayerState player;
    public EncounterState encounter;
    public LogState log;

    public static GameState Parse(string json)
    {
        var result = JsonUtility.FromJson<GameState>(json);

        var cards = new List<CardState>(result.player.generic.autoDeck);
        result.encounter.NPCs.ForEach(npc => cards.AddRange(npc.autoDeck));

        return result;
    }
}