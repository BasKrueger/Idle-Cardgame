using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class DeckBuildingController : MonoBehaviour
{
    private static readonly int editModeBool = Animator.StringToHash("EditMode");

    [SerializeField]
    private GameModelManager model;

    [SerializeField]
    private Animator anim;
    [SerializeField]
    private RectTransform previewContent;
    [SerializeField]
    private CollectionView collection;
    [SerializeField]
    private DeckView deck;

    [SerializeField]
    private CardSlot previewSlot;

    private CardSlot SlotA = null;
    private CardSlot SlotB = null;

    private void Awake()
    {
        collection.SlotSelected += OnSlotSelected;
        deck.SlotSelected += OnSlotSelected;
    }

    private void OnSlotSelected(CardSlot other)
    {
        if(other == SlotA)
        {
            previewSlot.TrySwapCards(other);
            SlotA = null;

            anim.SetBool(editModeBool, false);
            return;
        }

        if (SlotA == null) SlotA = other;
        else if (SlotB == null) SlotB = other;

        if (previewSlot.card == null) previewSlot.TrySwapCards(other);
        if (!TryPerformSwap()) anim.SetBool(editModeBool, true);
    }

    private bool TryPerformSwap()
    {
        if (SlotA == null || SlotB == null) return false;

        SlotB.TrySwapCards(previewSlot);
        SlotA.TrySwapCards(previewSlot);

        GameDLL.SwapCards(SlotA.card.lastState.id, SlotB.card.lastState.id);
        SaveManager.Save();
        
        SlotA = null;
        SlotB = null;

        anim.SetBool(editModeBool, false);
        
        return true;
    }

    public void OnCancelPressed()
    {
        if(previewSlot != null && SlotA != null)
        {
            previewSlot.TrySwapCards(SlotA);
        }

        SlotA = null;
        SlotB = null;

        anim.SetBool(editModeBool, false);
    }
}
