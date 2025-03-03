using System;

public partial class GameModel 
{
    public const float TICK_COOLDOWN = 1f;
    
    private PlayerModel player;
    private EncounterModelManager encounters;
    private LogModel log;
    private InteractionResolver interactions;
    private bool generateStates = false;
    
    public GameModel()
    {
        log = new LogModel();
        interactions = new InteractionResolver(log);

        player = interactions.CreateInstance<PlayerModel>();
        player.StateUpdated += PushState;

        encounters = new EncounterModelManager(player, interactions);
        encounters.StateUpdated += PushState;

        generateStates = true;
    }
        
    public void Tick()
    {
        if(player.phase == CharacterPhase.Dead) return;
        
        encounters.Tick(player, interactions);
        PushState();
    }

    public void Skip(float seconds)
    {
        generateStates = false;
        for (int i = 0; i < seconds / TICK_COOLDOWN; i++) Tick();
        generateStates = true;
        PushState();  
    }

    public void SwapCards(Guid collection, Guid deck)
    {
        player.SwapCards(collection, deck);
        PushState();
    }
}
