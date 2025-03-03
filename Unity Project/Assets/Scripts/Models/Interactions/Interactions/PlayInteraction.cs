using UnityEngine;

public class PlayInteraction : Interaction
{
    public CardModel card;
    
    public PlayInteraction(ActorModel source, ActorModel target, CardModel card) : base(source, target)
    {
        this.card = card;
    }

    public override BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions) => interactions.play;

    public override void Perform(LogModel log)
    {
         log.AddSubEntry(new LegacyLocalizedString(LegacyLocalizedString.Table.AdventureLog, "CardPlayed",
            new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, source.owner.characterNameKey), card.GetCardName()));

        card.TryPlay(source.owner, target.owner);
        card.owner.TryPlayNextCard();
    }
}
