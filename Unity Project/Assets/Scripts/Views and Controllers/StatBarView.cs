using System;
using UnityEngine;

public class StatBarView : MonoBehaviour, IGameView
{
    [SerializeField]
    private StatView hp;
    [SerializeField]
    private StatView gold;
    [SerializeField]
    private StatView xp;

    public void OnGameStateUpdate(GameState gameState)
    {
        hp.ShowValue(gameState.player.generic.hp, gameState.player.generic.baseHP);
        gold.ShowValue(gameState.player.gold);
        xp.ShowValue(gameState.player.xp);
    }
}
