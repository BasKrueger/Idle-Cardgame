using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BonusRewardView : MonoBehaviour
{
    public event Action claimed;

    [Header("Scene References")]
    [SerializeField]
    private StatView xpStat;
    [SerializeField]
    private StatView goldStat;
    [SerializeField]
    private Button claimButton;

    [Header("Template")]
    [SerializeField]
    private UIParticle coinParticleTemplate;
    [SerializeField]
    private UIParticle xpParticleTemplate;

    private StatView goldTarget;
    private StatView xpTarget;

    RewardState reward;

    public void SetUp(RewardState reward, StatView goldTarget, StatView xpTarget)
    {
        if(reward.gold == 0 && reward.xp == 0)
        {
            this.gameObject.SetActive(false);
            claimed?.Invoke();
            return;
        }

        this.reward = reward;
        this.goldTarget = goldTarget;
        this.xpTarget = xpTarget;

        ShowBonusRewards(reward);
    }

    public async void ClaimBonus()
    {
        claimButton.interactable = false;

        GameDLL.ClaimBonusReward(reward.ID);
        GameDLL.CreateGameState();

        await AnimateClaim();

        claimed?.Invoke();
    }

    private async UniTask AnimateClaim()
    {
        goldTarget.updatesEnabled = false;
        xpTarget.updatesEnabled = false;

        var _ = AnimateClaimParticle(coinParticleTemplate, goldStat, goldTarget, reward.gold);
        await UniTask.WaitForSeconds(0.45f);
        _ = AnimateClaimParticle(xpParticleTemplate, xpStat, xpTarget, reward.xp);
    }

    private void ShowBonusRewards(RewardState reward)
    {
        goldStat.gameObject.SetActive(reward.gold > 0);
        goldStat.ShowValue(reward.gold, true);

        xpStat.gameObject.SetActive(reward.xp > 0);
        xpStat.ShowValue(reward.xp, true);
    }

    private async UniTask AnimateClaimParticle(UIParticle template, StatView origin, StatView target, int amount)
    {
        var collectCurve = new SortedDictionary<float, float>
        {
            { 0, 25 },
            { 0.1f, 1500 },
            { 0.6f, 2500 },
            { 1, 0 }
        };

        var particles = await ParticleUI.Splatter(template, amount, origin.transform.position, 1.5f);

        for (int i = 0; i < particles.Count; i++)
        {
            var particle = particles[i];

            var _ = particle.MoveWithCustomEase(target.icon.transform.position, collectCurve, (0.95f, instance => instance.FadeOut(0.2f)))
                    .ContinueWith(() => Destroy(particle.gameObject));

            if (i % 4 == 0)
            {
                await UniTask.WaitForEndOfFrame();
            }
        }

        await UniTask.WaitForSeconds(0.3f);
        target.updatesEnabled = true;
    }
}
