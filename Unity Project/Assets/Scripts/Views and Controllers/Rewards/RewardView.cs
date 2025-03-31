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
    private (int id, RewardViewBox box) active;
    private bool moving = false;

    private void Awake()
    {
        foreach(RewardViewBox t in content.GetComponentsInChildren<RewardViewBox>())
        {
            Destroy(t.gameObject);
        }

        anim.GetComponent<AnimEventForwarder>().rewardFinished += OnOpenAnimationFinished;
    }

    public void OnGameStateUpdate(GameState gameState)
    {
        gameState.rewards.earnedRewards.ForEach(reward => unclaimedRewards.TryAdd(reward.ID, reward));
        
        UpdateLabels(gameState.rewards);
    }

    private void TrySpawnBox(RewardState state)
    {
        if (active.box == null)
        {
            active.box = SpawnBox(state);
            active.id = state.ID;
        }
    }

    private RewardViewBox SpawnBox(RewardState state)
    {
        var instance = Instantiate(boxTemplate);
        instance.transform.SetParent(content, false);
        instance.allClaimed += OnBoxClaimed;
        instance.SetUp(state, goldParticleTarget, xpParticleTarget, collection);

        if(active.box == null)
        {
            content.transform.localPosition = new Vector3();
            instance.transform.position = content.transform.position;
        }
        else
        {
            instance.transform.localPosition = active.box.transform.localPosition + new Vector3(active.box.GetComponent<RectTransform>().rect.width, 0);
        }

        return instance;
    }

    private async UniTask TryShowNextRewardBox(RewardState state)
    {
        if (moving) return;
        moving = true;

        var nextBox = SpawnBox(state);
        var moveWidth = boxTemplate.GetComponent<RectTransform>().rect.width;
        var targetPos = content.transform.localPosition - new Vector3(moveWidth, 0);

        while (content.transform.localPosition != targetPos)
        {
            content.transform.localPosition = Vector3.MoveTowards(content.transform.localPosition, targetPos, boxMoveSpeed * Time.deltaTime * Screen.width);
            await UniTask.WaitForEndOfFrame();
        }

        Destroy(active.box.gameObject);
        active.box = nextBox;
        active.id = state.ID;

        if (nextBox != null) nextBox.Show();
        moving = false;
    }

    private async void OnBoxClaimed()
    {
        unclaimedRewards.Remove(active.id);

        if(unclaimedRewards.Count == 0)
        {
            OnCancelClicked();
            return;
        }

        await TryShowNextRewardBox(unclaimedRewards.First().Value);
    }

    public void OnRewardModeClicked()
    {
        rewardButton.interactable = false;
        anim.SetBool(rewardModeBool, true);
        this.enabled = true;

        if (active.box != null) Destroy(active.box.gameObject);
        active.box = null;
        active.id = -1;

        TrySpawnBox(unclaimedRewards.First().Value);
    }

    public void OnCancelClicked()
    {
        rewardButton.interactable = true;
        anim.SetBool(rewardModeBool, false);
        this.enabled = false;
    }
    
    private void UpdateLabels(RewardStashState state)
    {
        rewardButtonLabel.text = state.rewardCount == 0
           ? new LocalizedString("Menus", "Rewards_Button_Empty").GetLocalizedString()
           : string.Format(new LocalizedString("Menus", "Rewards_Button").GetLocalizedString(), state.rewardCount);
    }

    private void OnOpenAnimationFinished()
    {
        if (!this.enabled || active.box == null) return;
        active.box.Show();
    }
}
