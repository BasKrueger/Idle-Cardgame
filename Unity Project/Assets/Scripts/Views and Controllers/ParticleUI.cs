using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using static UnityEngine.UI.Image;

public class ParticleUI : MonoBehaviour
{
    private static ParticleUI instance_;
    public static ParticleUI instance
    {
        get
        {
            if(instance_ == null)
            {
                instance_ = FindFirstObjectByType<ParticleUI>();
            }

            return instance_;
        }
    }

    public static async UniTask<List<UIParticle>> Splatter (UIParticle particleTemplate, int count, Vector3 position, float force = 1, float rad = 1, SortedDictionary<float, float> speedCurve = null)
    {
        if (speedCurve == null)
        {
            speedCurve = new SortedDictionary<float, float>
            {
                { 0.25f, 1500 * force },
                { 0.85f, 100 * force },
                { 1, 25 * force}
            };
        }

        float minDist = 75 * rad;
        float maxDist = 150 * rad;

        var particles = new List<UIParticle>();
        var tasks = new List<UniTask.Awaiter>();

        for(int i = 0;i < count; i++)
        {
            var particle = Instantiate(particleTemplate);
            particle.transform.SetParent(instance.transform);
            particle.transform.position = position;

            var dir = new Vector3(UnityEngine.Random.Range(-1f, 1f), UnityEngine.Random.Range(-1f, 1f));
            var dist = new Vector3(UnityEngine.Random.Range(minDist, maxDist), UnityEngine.Random.Range(minDist, maxDist));
            var splatterPos = position + Vector3.Scale(dir, dist);

            tasks.Add(particle.MoveWithCustomEase(splatterPos, speedCurve).GetAwaiter());

            particles.Add(particle);

            if (i % 2 == 0)
            {
                await UniTask.WaitForEndOfFrame();
            }
        }

        while (tasks.Any(awaiter => !awaiter.IsCompleted)) await UniTask.WaitForEndOfFrame();

        return particles;
    }
}
