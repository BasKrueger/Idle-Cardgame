using Cysharp.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.Localization;
using UnityEngine.UI;

public class RewardModeController : MonoBehaviour, IGameView
{
    private static int rewardModeBool = Animator.StringToHash("RewardMode");

    [SerializeField]
    private Animator anim;

    [SerializeField]
    private RewardView content;
    [SerializeField]
    private Button rewardButton;
    [SerializeField]
    private TextMeshProUGUI rewardButtonLabel;
    [SerializeField]
    private CollectionView collection;

    private void Awake()
    {
        content.LastRewardClaimed += OnCancelClicked;
        content.RewardClaimed += OnRewardClaimed;
    }

    public void OnRewardModeClicked()
    {
        anim.SetBool(rewardModeBool, true);
    }
    public void OnCancelClicked()
    {
        anim.SetBool(rewardModeBool, false);
    }

    private async void OnRewardClaimed(int rewardID, CardView card)
    {
        GameDLL.ClaimReward(rewardID, card.Content.displayID);

        card.Content.transform.SetParent(transform.parent, true);
        
        var emptyCard = collection.SpawnEmptyCard();
        await MoveCardContent(card, emptyCard);
        emptyCard.Content = card.Content;
        emptyCard.Content.ShowAsMedium();
    }

    public void OnGameStateUpdate(GameState gameState)
    {
        content.Display(gameState.rewards);

        rewardButton.interactable = gameState.rewards.earnedRewards.Count > 0;
        rewardButtonLabel.text = gameState.rewards.earnedRewards.Count == 0
            ? new LocalizedString("Menus", "Rewards_Button_Empty").GetLocalizedString()
            : string.Format(new LocalizedString("Menus", "Rewards_Button").GetLocalizedString(), gameState.rewards.earnedRewards.Count);
    }

    private async UniTask MoveCardContent(CardView cardA, CardView cardB)
    {
        cardA.Content.OverrideGlobalPosition(true, cardA.Content.transform.position);
        await cardA.Content.SetTarget(cardB.transform);
        cardA.Content.transform.SetParent(cardB.transform, true);
        cardA.Content.OverrideGlobalPosition(false);
    }
}
