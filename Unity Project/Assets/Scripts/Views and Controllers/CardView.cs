using System;
using UnityEngine;

public class CardView : MonoBehaviour, IPoolable
{
    public event Action<CardViewContent, CardView> ContentChanged;
    public event Action<CardView> Clicked;

    private readonly static int playKey = Animator.StringToHash("Play");
    private readonly static int cooldownKey = Animator.StringToHash("CooldownPercent");

    [SerializeField]
    protected Animator anim;
    [SerializeField]
    private CardViewContent content;
    
    public CardViewContent Content
    {
        get => content;
        set
        {
            if (value == null) return;
            var previousContent = content;

            if (previousContent != null)
            {
                content.Clicked -= OnContentClicked;
                content.ChargeUpdated -= SetAnimCharge;
            }

            content = value;
            value.Clicked += OnContentClicked;
            value.ChargeUpdated += SetAnimCharge;

            ContentChanged?.Invoke(previousContent, this);
        }
    }
    
    private void Awake()
    {
        Content = content;
    }

    public void Show(CardState state)
    {
        content.Show(state);
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
