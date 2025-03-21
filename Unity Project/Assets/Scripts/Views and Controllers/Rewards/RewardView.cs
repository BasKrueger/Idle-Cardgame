using Cysharp.Threading.Tasks;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.Localization;
using UnityEngine.UI;

public class RewardView : MonoBehaviour, IGameView
{
    private static int rewardModeBool = Animator.StringToHash("RewardMode");
    private const float boxMoveSpeed = 6;

    [Header("PRefabs")]
    [SerializeField]
    private RewardViewBox boxTemplate;

    [Header("Scene references")]
    [SerializeField]
    private Transform content;
    [SerializeField]
    private Animator anim;
    [SerializeField]
    private Button rewardButton;
    [SerializeField]
    private TextMeshProUGUI rewardButtonLabel;

    [SerializeField]
    private StatView goldParticleTarget;
    [SerializeField]
    private StatView xpParticleTarget;
    [SerializeField]
    private CollectionView collection;

    private Dictionary<int, RewardState> unclaimedRewards = new();
    private HashSet<int> claimedRewards = new();

    private RewardViewBox activeBox;

    private void Awake()
    {
        foreach(RewardViewBox t in content.GetComponentsInChildren<RewardViewBox>())
        {
            Destroy(t.gameObject);
        }

        anim.GetComponent<AnimEventForwarder>().rewardFinished += OnOpenAnimationFinished;
    }

    public async void OnGameStateUpdate(GameState gameState)
    {
        for (int i = 0; i < gameState.rewards.earnedRewards.Count; i++)
        {
            var state = gameState.rewards.earnedRewards[i];
            if (claimedRewards.Contains(state.ID) || unclaimedRewards.ContainsKey(state.ID)) continue;
            unclaimedRewards.TryAdd(state.ID, state);

            if(activeBox == null)
            {
                activeBox = await SpawnBox(state);
            }
        }
       
        rewardButtonLabel.text = gameState.rewards.earnedRewards.Count == 0
            ? new LocalizedString("Menus", "Rewards_Button_Empty").GetLocalizedString()
            : string.Format(new LocalizedString("Menus", "Rewards_Button").GetLocalizedString(), gameState.rewards.earnedRewards.Count);
    }

    public void OnRewardModeClicked()
    {
        rewardButton.interactable = false;
        anim.SetBool(rewardModeBool, true);
        this.enabled = true;
    }

    public void OnCancelClicked()
    {
        rewardButton.interactable = true;
        anim.SetBool(rewardModeBool, false);
        this.enabled = false;
    }

    private void OnOpenAnimationFinished()
    {
        if (!this.enabled || activeBox == null) return;
        activeBox.Show();
    }

    private async UniTask<RewardViewBox> SpawnBox(RewardState state)
    {
        var instance = Instantiate(boxTemplate);
        instance.transform.SetParent(content, false);
        instance.allClaimed += OnBoxClaimed;
        instance.SetUp(state, goldParticleTarget, xpParticleTarget, collection);

        await UniTask.WaitForEndOfFrame();

        if(activeBox == null)
        {
            content.transform.localPosition = new Vector3();
            instance.transform.position = content.transform.position;
        }
        else
        {
            instance.transform.localPosition = activeBox.transform.localPosition + new Vector3(activeBox.GetComponent<RectTransform>().rect.width, 0);
        }

        return instance;
    }

    private async UniTask TryShowNextRewardBox(RewardState state)
    {
        var nextBox = state != null ? await SpawnBox(state) : null;

        var moveWidth = activeBox.GetComponent<RectTransform>().rect.width;
        var targetPos = content.transform.localPosition - new Vector3(moveWidth, 0);

        while (content.transform.localPosition != targetPos)
        {
            content.transform.localPosition = Vector3.MoveTowards(content.transform.localPosition, targetPos, boxMoveSpeed * Time.deltaTime * Screen.width);
            await UniTask.WaitForEndOfFrame();
        }

        Destroy(activeBox.gameObject);
        activeBox = nextBox;

        if (nextBox != null) nextBox.Show(); 
    }

    private async void OnBoxClaimed()
    {
        claimedRewards.Add(unclaimedRewards.First().Key);
        unclaimedRewards.Remove(unclaimedRewards.First().Key);

        await TryShowNextRewardBox(unclaimedRewards.FirstOrDefault().Value);
        if (unclaimedRewards.Count == 0) OnCancelClicked();
    }
}
