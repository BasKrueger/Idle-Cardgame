using UnityEngine;

public class DamageInteraction : Interaction
{
    private int damage;
    
    public DamageInteraction(ActorModel source, CharacterModel target, int damage) : base(source, target)
    {
        this.damage = damage;
    }

    public override BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions) => interactions.damage;

    public override void Perform(LogModel log)
    {
        log.AddSubEntry(new LegacyLocalizedString(LegacyLocalizedString.Table.AdventureLog, "DamageTaken",
            new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, source.owner.characterNameKey), damage,
            new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, source.owner.characterNameKey)));

        target.owner.TakeDamage(source.owner, damage);
    }
}
