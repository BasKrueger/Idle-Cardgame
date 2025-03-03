using Cysharp.Threading.Tasks;
using UnityEngine;

public interface IGameViewAsync
{
    public UniTask OnGameStateUpdate(GameState gameState);
    
    GameObject gameObject { get ; } 
}
