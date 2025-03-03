using System;

public partial class EncounterModelManager 
{
    public event Action StateUpdated;
    public event Action NewEncounterBegan;
    
    private EncounterModel currentEncounter;
    
    public EncounterModelManager(PlayerModel player, InteractionResolver interactions)
    {
        BeginNextEncounter(player, interactions); 
    }
    
    public void Tick(PlayerModel player, InteractionResolver interactions)
    {
        if(currentEncounter.completeCondition)
        {
            BeginNextEncounter(player, interactions);
        }
        
        currentEncounter.Tick(player);
    }
    
    private EncounterModel GenerateRandomEncounter(InteractionResolver interactions)
    {
        if(currentEncounter != null && currentEncounter is CombatEncounter)
        {
            return interactions.CreateInstance<IdleEncounter>();
        }

        return interactions.CreateInstance<CombatEncounter>();
    }
    
    private void BeginNextEncounter(PlayerModel player, InteractionResolver interactions, EncounterModel encounter = null)
    {
        if(currentEncounter != null)
        {
            currentEncounter.EndEncounter(player);
            currentEncounter.StateUpdated -= OnEncounterUpdated;
        }
        
        currentEncounter = encounter ?? GenerateRandomEncounter(interactions);
        currentEncounter.StateUpdated += OnEncounterUpdated;

        interactions.AddNow(new BeginEncounterInteraction(player, currentEncounter));
        
        NewEncounterBegan?.Invoke();
    }

    private void OnEncounterUpdated() => StateUpdated?.Invoke();

    public EncounterModel.State GetCurrentEncounterState()
    {
        return currentEncounter.GetState();
    }
}
