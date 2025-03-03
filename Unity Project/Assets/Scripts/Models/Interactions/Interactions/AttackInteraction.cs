using UnityEngine;

public class AttackInteraction : Interaction
{
    public int damage;
    
    public AttackInteraction(ActorModel source, ActorModel target, int damage) : base(source, target)
    {
        this.damage = damage;
    }

    public override BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions) => interactions.attack;

    public override void Perform(LogModel log)
    {
        source.interactions.AddNow(new DamageInteraction(source, target.owner, damage));
    }
}
