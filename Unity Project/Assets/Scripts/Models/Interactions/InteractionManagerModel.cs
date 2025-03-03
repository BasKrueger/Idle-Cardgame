using System;
using System.Collections.Generic;
using System.Linq;

public class InteractionResolver 
{
    private List<ActorModel> registeredActors = new List<ActorModel>();
    private List<Interaction> queuedInteractions = new List<Interaction>();
    private bool resolving = false;

    private LogModel logs;

    public InteractionResolver(LogModel log) => logs = log;
    
    public T CreateInstance<T>() where T : ActorModel
    {
        var instance = Activator.CreateInstance(typeof(T)) as T;
        instance.interactions = this;
        registeredActors.Add(instance);

        instance.AfterRegistration();
        return instance;
    }
    
    public object CreateInstance(Type type)
    {
        var instance = Activator.CreateInstance(type) as ActorModel;
        instance.interactions = this;
        registeredActors.Add(instance);
        return instance;
    }
    
    public void Add(Interaction interaction)
    {
        queuedInteractions.Add(interaction);
        ResolveQueue();
    }
    
    public void AddWithPriority(Interaction interaction)
    {
        queuedInteractions.Insert(1, interaction);
        ResolveQueue();
    }   
    
    public void AddNow(Interaction interaction)
    {
        ResolveInteraction(interaction);
    } 
    
    public void CancelNextInteraction()
    {
        queuedInteractions.RemoveAt(0);
    }
    
    public void ResolveQueue()
    {
        if (resolving) return;
        
        resolving = true;
        
        while(queuedInteractions.Count > 0)
        {
            var interaction = queuedInteractions[0];
            queuedInteractions.Remove(interaction);
            ResolveInteraction(interaction);
        }

        resolving = false;
    }
    
    private void ResolveInteraction(Interaction interaction)
    {
        var events = registeredActors.Select(actor => interaction.GetInteractionEvent(actor.interactionEvents)).ToList();

        events.ForEach((i) => i.OnBefore(interaction));
        interaction.Perform(logs);
        events.ForEach((i) => i.OnAfter(interaction));
    }
}
