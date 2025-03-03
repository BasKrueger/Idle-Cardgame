using System;
using UnityEngine;

public class InteractionEvent<T> : BaseInteractionEvent where T : Interaction
{
    public InteractionEvent()
    {
        base.before = (obj) => Before?.Invoke(obj as T);
        base.after = (obj) => After?.Invoke(obj as T);
    }

    public Action<T> Before;
    public Action<T> After;

    protected override bool Validate(object obj) => obj is T;
}

public abstract class BaseInteractionEvent
{
    protected Action<object> before;
    protected Action<object> after;

    public void OnBefore(object t)
    {
        if (Validate(t))
        {
            before?.Invoke(t);
        }
    }

    public void OnAfter(object t)
    {
        if (Validate(t))
        {
            after?.Invoke(t);
        }
    }

    protected abstract bool Validate(object obj);
}