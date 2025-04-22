using Cysharp.Threading.Tasks;
using System;
using System.Threading.Tasks;
using UnityEngine;

public class RewardViewBox : MonoBehaviour
{
    public event Action allClaimed;

    [SerializeField]
    private CardRewardView cards;
    [SerializeField]
    private BonusRewardView bonus;

    private RewardState reward;

    private void Awake()
    {
        cards.claimStarted += OnClaimStarted;
        cards.claimEnded += OnClaimEnded;
    }

    public void SetUp(RewardState reward, StatView gold, StatView xp, CollectionView collection)
    {
        this.reward = reward;

        bonus.SetUp(reward, gold, xp);
        cards.SetUp(reward, collection);
    }

    private void OnClaimStarted(int cardID)
    {
        bonus.ClaimBonus();
        GameDLL.ClaimReward(reward.ID, cardID);
        SaveManager.Save();
    }

    private async void OnClaimEnded()
    {
        await UniTask.WaitForSeconds(1.2f);

        allClaimed?.Invoke();
    }
}
