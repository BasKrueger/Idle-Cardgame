using UnityEngine;

public class BeginEncounterInteraction : Interaction
{
    public EncounterModel encounter;
    public BeginEncounterInteraction(PlayerModel source, EncounterModel encounter) : base(source, null)
    {
        this.encounter = encounter;
    }

    public override BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions) => interactions.beginEncounter;

    public override void Perform(LogModel log)
    {
        log.BeginNewEntry(new LegacyLocalizedString(LegacyLocalizedString.Table.AdventureLog, $"{encounter.encounterName}Encounter"
            , new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, "Player")));

        encounter.BeginEncounter(source as PlayerModel);
    }
}
