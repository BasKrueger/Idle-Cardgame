using Cysharp.Threading.Tasks;
using System;
using UnityEngine;

public class CardSlot : MonoBehaviour
{
    public event Action<CardSlot> ContentChanged;
    public event Action<CardSlot> Clicked;

    public bool inPlayAnimation => card != null && card.inPlayAnimation;

    [Header("Settings")]

    [SerializeField]
    private float cardMoveSpeed = 1500;
    [SerializeField]
    private float cardScaleSpeed = 1000;
    [SerializeField]
    public ParticleUI preferedUI;
    [SerializeField]
    private bool hideText;
    [SerializeField]
    private bool playAnimationEnabled;

    [Header("References")]
    [SerializeField]
    private RectTransform content;
    [SerializeField]
    private CardView cardTemplate;

    private CardView card_;
    public CardView card
    {
        get => card_;
        private set
        {
            if(card_ != null)
            {
                card_.TryClearAtFrameEnd();
                card_.slot = null;
            }

            if (card_ != value && value != null) 
            {
                value.slot = this;
                value.scaleSpeed = cardScaleSpeed;
                value.moveSpeed = cardMoveSpeed;
                value.hideText = hideText;
                value.preferedUI = preferedUI;
                value.playAnimationEnabled = playAnimationEnabled;
                card_ = value;
            }

            card_ = value;
            ContentChanged?.Invoke(this);
        }
    }

    public void Awake()
    {
        foreach (var t in GetComponentsInChildren<CardView>()) Destroy(t.gameObject);
    }

    public async UniTask<bool> TrySwapCards(CardSlot slotB)
    {
        if (slotB == null) return false;
        var slotA = this;

        var cardA = slotA.card;
        var cardB = slotB.card;

        slotA.card = cardB;
        slotB.card = cardA;

        if(cardA != null) cardA.slot = slotB;
        if(cardB != null) cardB.slot = slotA;

        if (slotA.card != null) { var _ = this.card.MoveTo(slotA.content); }
        if (slotB.card != null) { var _ = slotB.card.MoveTo(slotB.content); }

        slotA.ContentChanged?.Invoke(slotA);
        slotB.ContentChanged?.Invoke(slotB);

        return true;
    }
    
    public async UniTask<bool> TryAddCard(CardView other, bool frameDelay = false)
    {
        if (card != null) return false;

        card = other;

        if (frameDelay) await UniTask.WaitForEndOfFrame();
        await other.MoveTo(content);
        other.transform.SetParent(content);

        return true;
    }

    public void DetachCard()
    {
        card = null;
    }

    public bool TrySpawnCard(CardState state)
    {
        if (card != null) return false;

        var tmp = Instantiate(cardTemplate, content);
        tmp.MoveToInstant(content);
        tmp.Display(state);

        card = tmp;

        return true;
    }

    public bool TryUpdateCard(CardState state, bool strict = false)
    {
        if (card == null) return false;
        if (state == null) return false;
        if (strict)
        {
            if (card.lastState == null || card.lastState.id != state.id) return false;
        }

        card.Display(state);
        return true;
    }

    public void OnClick()
    {
        Clicked?.Invoke(this);
    }
}
