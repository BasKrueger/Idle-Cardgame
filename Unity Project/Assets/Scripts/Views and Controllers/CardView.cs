using System;
using UnityEngine;

public class CardView : MonoBehaviour, IPoolable
{
    public event Action<CardView> ContentChanged;
    public event Action<CardView> Clicked;

    private readonly static int playKey = Animator.StringToHash("Play");
    private readonly static int cooldownKey = Animator.StringToHash("CooldownPercent");

    [SerializeField]
    private Animator anim;
    [SerializeField]
    private CardViewContent content;
    
    public CardViewContent Content
    {
        get => content;
        set
        {
            content.Clicked -= OnContentClicked;
            content.ChargeUpdated -= SetAnimCharge;
            
            value.Clicked += OnContentClicked;
            value.ChargeUpdated += SetAnimCharge;
            
            content = value;
            ContentChanged?.Invoke(this);
        }
    }
    
    private void Awake()
    {
        Content = content;
    }
    
    public void ShowPlayed(Pool<CardView> returnPool = null)
    {
        anim.SetTrigger(playKey);
        
        if(returnPool == null)
        {
            Destroy(this.gameObject, 2);
        }
        else
        {
            returnPool.Return(this, 2);
        }
    }

    private void SetAnimCharge(float charge) => anim.SetFloat(cooldownKey, charge);
    private void OnContentClicked()
    {
        Clicked?.Invoke(this);
    }

    public void OnPoolEntered()
    {
        anim.Rebind();
        anim.Update(0);
    }
}
