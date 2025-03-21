using Cysharp.Threading.Tasks;
using JetBrains.Annotations;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class RewardCardView : CardView
{
    private static readonly int discoverTrigger = Animator.StringToHash("DiscoverReward");
    private static readonly int hideTrigger = Animator.StringToHash("HideReward");

    private static readonly int tapIdleTrigger = Animator.StringToHash("TriggerIdle");

    private Transform ogParent;
    private Vector3 ogPos;

    [SerializeField]
    private Animator tapIcon;
    [SerializeField]
    private UIParticle revealParticle;
    [SerializeField]
    private RectTransform revealPos;

    private bool discovered = false;

    public UniTaskCompletionSource animationDoneSource { get; private set; }

    public async void DiscoverCard(float delay)
    {
        if (discovered) return;
        discovered = true;

        ogParent = transform.parent;
        ogPos = transform.position;

        transform.SetParent(ParticleUI.instance.transform, true);
        transform.position = ogPos;
        
        animationDoneSource = new UniTaskCompletionSource();

        await UniTask.WaitForSeconds(delay);
        anim.SetTrigger(discoverTrigger);
        TapIconIdleLoop();
    }

    public void HideCard()
    {
        anim.SetTrigger(hideTrigger);
    }

    public void OnDiscoverEndTrigger()
    {
        animationDoneSource?.TrySetResult();
    }

    public void ReturnToPreAnimationParent()
    {
        transform.SetParent(ogParent);
    }

    public async void OnDiscoverVFXTrigger()
    {
        var particles = await ParticleUI.Splatter(revealParticle, 10, revealPos.transform.position, 0.75f, 0.65f);

        foreach(var particle in particles)
        {
            var _ = particle.FadeOut(0.25f).ContinueWith(() => Destroy(particle.gameObject));
        }
    }

    private async void TapIconIdleLoop()
    {
        while (true)
        {
            await UniTask.WaitForSeconds(Random.Range(5, 15));
            if (tapIcon == null) break;
            tapIcon.SetTrigger(tapIdleTrigger);
        }
    }
}
