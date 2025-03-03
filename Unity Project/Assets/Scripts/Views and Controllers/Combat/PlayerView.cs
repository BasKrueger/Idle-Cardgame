using Cysharp.Threading.Tasks;

public class PlayerView : CharacterView, IGameView, IGameViewAsync
{
    public UniTask OnGameStateUpdate(GameState gameState)
    {
        return base.OnCharacterStateUpdateAsync(gameState.player.generic);
    }

    void IGameView.OnGameStateUpdate(GameState gameState)
    {
        base.OnCharacterStateUpdate(gameState.player.generic);
    }
}
