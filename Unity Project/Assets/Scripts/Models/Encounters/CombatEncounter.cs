using System.Linq;

public class CombatEncounter : EncounterModel
{
    public override string encounterName => "Combat";
    public override bool completeCondition => encounterNPCListCopy.All(NPC => NPC.hp <= 0);
    public override bool isStaticEncounter => true;
    
    public override void AfterRegistration()
    {
        AddNPC(interactions.CreateInstance<EnemyModel>());
    }

    public override void BeginEncounter(PlayerModel player)
    {
        foreach(var npc in encounterNPCListCopy)
        {
            player.EngageInCombat(npc);
            npc.EngageInCombat(player);
        }

        base.BeginEncounter(player);
    }

    public override void EndEncounter(PlayerModel player)
    {
        player.DisEngageInCombat(player);
        encounterNPCListCopy.Where(NPC => NPC.phase != CharacterPhase.Dead).ToList().ForEach(npc => npc.DisEngageInCombat(player));

        base.EndEncounter(player);
    }
}
