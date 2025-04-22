using Cysharp.Threading.Tasks;
using UnityEngine;

public class CardAnimationEvents : MonoBehaviour
{
    [SerializeField]
    private RectTransform particlePos;
    [SerializeField]
    private UIParticle sparkleParticle;

    public void OnDisappearAnimationEnd()
    {
        Destroy(this.gameObject);
    }

    public void OnPlayAnimationEnd()
    {
        Destroy(this.gameObject);
    }

    public void OnRevealStart()
    {
        ParticleUI.AddMain(transform);
    }
    public void OnRevealEnd()
    {
        ParticleUI.TryReturnMain(transform);
    }

    public async void TriggerSparkleVFX()
    {
        var particles = await ParticleUI.SplatterMain(sparkleParticle, 10, particlePos.transform.position, 0.75f, 0.65f);

        foreach (var particle in particles)
        {
            var _ = particle.FadeOut(0.25f).ContinueWith(() => Destroy(particle.gameObject));
        }
    }
}
