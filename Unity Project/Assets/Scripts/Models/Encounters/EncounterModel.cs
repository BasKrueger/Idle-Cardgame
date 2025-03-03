using System;
using System.Collections.Generic;

public abstract partial class EncounterModel : ActorModel
{
    public event Action StateUpdated;
    public event Action Ended;
    
    public abstract string encounterName { get; }
    public abstract bool completeCondition { get; }
    public abstract bool isStaticEncounter { get; }

    protected Dictionary<string, float> variables = new Dictionary<string, float>();
    private List<CharacterModel> encounterNPCs = new List<CharacterModel>();

    private CharacterModel _owner;
    public override CharacterModel owner => _owner;
    protected List<CharacterModel> encounterNPCListCopy => new List<CharacterModel>(encounterNPCs);

    
    public virtual void BeginEncounter(PlayerModel player)
    {
        _owner = player;
    }
    
    public virtual void Tick(PlayerModel player)
    {
        player.Tick();
        encounterNPCs.ForEach(npc => npc.Tick());
    }
    
    public virtual void EndEncounter(PlayerModel player)
    {
        Ended?.Invoke();
    }
    
    protected void AddNPC(CharacterModel NPC)
    {
        encounterNPCs.Add(NPC);
        NPC.StateUpdated += OnNPCStateUpdate;

        StateUpdated?.Invoke();
    }
    
    protected void RemoveNPC(CharacterModel NPC)
    {
        encounterNPCs.Remove(NPC);
        NPC.StateUpdated -= OnNPCStateUpdate;

        StateUpdated?.Invoke();
    }

    private void OnNPCStateUpdate() => StateUpdated?.Invoke();
}
