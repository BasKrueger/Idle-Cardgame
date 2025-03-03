using System;

public abstract class ActorModel
{
    public Guid id { get; protected set; } = Guid.NewGuid();

    public abstract CharacterModel owner { get; }
    public InteractionResolver interactions;
    public ActorInteractions interactionEvents = new();

    public virtual void AfterRegistration() { }
}

public class ActorInteractions
{
    public InteractionEvent<BeginEncounterInteraction> beginEncounter = new();
    public InteractionEvent<AttackInteraction> attack = new();
    public InteractionEvent<DamageInteraction> damage = new();
    public InteractionEvent<PlayInteraction> play = new();
    public InteractionEvent<DeathInteraction> death = new();
}
