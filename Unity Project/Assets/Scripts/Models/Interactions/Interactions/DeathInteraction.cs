using UnityEngine;

public class DeathInteraction : Interaction
{
    public DeathInteraction(ActorModel source, CharacterModel target) : base(source, target)
    {
    }

    public override BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions) => interactions.death;

    public override void Perform(LogModel log)
    {
        log.AddSubEntry(new LegacyLocalizedString(LegacyLocalizedString.Table.AdventureLog, "Death", 
        new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, target.owner.characterNameKey)));

        target.owner.Die(source.owner);
    }
}
