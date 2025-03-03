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
    
    private CardView selectedCard;

    private void Awake()
    {
        collection.CardSelected += ActivateEditMode;
        deck.CardSelected += SwapSelectedCardWith;
    }

    private void ActivateEditMode(CardView view)
    {
        if (selectedCard != null) return;

        selectedCard = view;
        anim.SetBool(editModeBool, true);

        view.Content.ShowAsMega();
        view.Content.transform.SetParent(previewContent, true);
        view.Content.OverrideGlobalPosition(true, view.transform.position);
        view.Content.SetTarget(previewContent.transform);
    }

    private async void SwapSelectedCardWith(CardView other)
    {
        if (selectedCard == null) return;

        GameDLL.SwapCards(selectedCard.Content.displayID, other.Content.displayID);
        GameDLL.GenerateGameState();

        anim.SetBool(editModeBool, false);

        selectedCard.Content.ShowAsMedium();
        other.Content.ShowAsDefault();
        
        await UniTask.WhenAll(
            MoveCardContent(selectedCard, other),
            MoveCardContent(other, selectedCard)
        );
        
        var selectedContent = selectedCard.Content;
        selectedCard.Content = other.Content;
        other.Content = selectedContent;

        selectedCard = null;
    }

    private async UniTask MoveCardContent(CardView cardA, CardView cardB)
    {
        cardA.Content.OverrideGlobalPosition(true, cardA.Content.transform.position);
        await cardA.Content.SetTarget(cardB.transform);
        cardA.Content.transform.SetParent(cardB.transform, true);
        cardA.Content.OverrideGlobalPosition(false);
    }

    public async void OnCancelPressed()
    {
        if (selectedCard == null) return;

        anim.SetBool(editModeBool, false);
        
        await selectedCard.Content.SetTarget(selectedCard.transform);

        selectedCard.Content.ShowAsDefault();
        selectedCard.Content.transform.SetParent(selectedCard.transform, true);
        selectedCard.Content.OverrideGlobalPosition(false);

        selectedCard = null;
    }
}
