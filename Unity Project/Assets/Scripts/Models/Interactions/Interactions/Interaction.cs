using UnityEngine;

public abstract class Interaction
{
    public ActorModel source;
    public ActorModel target;
    
    public Interaction(ActorModel source, ActorModel target)
    {
        this.source = source;
        this.target = target;
    }
    
    public abstract void Perform(LogModel log);
    public abstract BaseInteractionEvent GetInteractionEvent(ActorInteractions interactions);
}
