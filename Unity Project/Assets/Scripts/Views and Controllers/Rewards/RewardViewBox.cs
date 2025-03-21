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

    private void Awake()
    {
        cards.claimStarted += OnClaimStarted;
        cards.claimEnded += OnClaimEnded;
    }

    public void SetUp(RewardState reward, StatView gold, StatView xp, CollectionView collection)
    {
        bonus.SetUp(reward, gold, xp);
        cards.SetUp(reward, collection);
    }

    public void Show()
    {
        cards.Show();
    }

    private void OnClaimStarted()
    {
        bonus.ClaimBonus();
    }

    private async void OnClaimEnded()
    {
        await UniTask.WaitForSeconds(1.2f);

        allClaimed?.Invoke();
    }
}
