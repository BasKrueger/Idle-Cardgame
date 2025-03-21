using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class UIParticle : MonoBehaviour
{
    [SerializeField]
    private CanvasGroup group;

    public async UniTask MoveToPosition(Vector3 targetPos, float speed)
    {
        while (transform.position != targetPos)
        {
            transform.position = Vector3.MoveTowards(targetPos, targetPos, speed * Time.deltaTime);
            await UniTask.WaitForEndOfFrame();
        }
    }

    public async UniTask MoveWithEase(Vector3 targetPos, float duration)
    {
        var startPos = transform.position;
        var t = 0f;

        while (t < duration)
        {
            transform.position = Vector3.Lerp(startPos, targetPos, Mathf.SmoothStep(0, 1, t / duration));
            t += Time.deltaTime;

            await UniTask.WaitForEndOfFrame();
        }

        transform.position = targetPos;
    }

    public async UniTask MoveWithCustomEase(Vector3 targetPos, SortedDictionary<float, float> speedAtPercent, (float percent, Action<UIParticle> action) performAtPercent)
            => await MoveWithCustomEase(targetPos, speedAtPercent, new SortedDictionary<float, Action<UIParticle>>() { { performAtPercent.percent, performAtPercent.action }, });
    public async UniTask MoveWithCustomEase(Vector3 targetPos, SortedDictionary<float, float> speedAtPercent, SortedDictionary<float, Action<UIParticle>> performAtPercent = null)
    {
        if (performAtPercent == null) performAtPercent = new();
        speedAtPercent.TryAdd(0, speedAtPercent.First().Value);
        speedAtPercent.TryAdd(1, speedAtPercent.Last().Value);
        speedAtPercent.TryAdd(1.1f, speedAtPercent.Last().Value);

        var startPos = transform.position;

        while(Vector3.Distance(transform.position, targetPos) > 0.01f)
        {
            var distPercent = 1 - Vector3.Distance(transform.position, targetPos) / Vector3.Distance(startPos, targetPos);
            var moveSpeed = CalculateMoveSpeed(speedAtPercent, distPercent);
            transform.position = Vector3.MoveTowards(transform.position, targetPos, moveSpeed * Time.deltaTime); ;
            
            if (performAtPercent.Count > 0 && performAtPercent.First().Key < distPercent)
            {
                performAtPercent.First().Value?.Invoke(this);
                performAtPercent.Remove(performAtPercent.First().Key);
            }

            await UniTask.WaitForEndOfFrame();
        }

        transform.position = targetPos;
    }

    public async UniTask FadeOut(float fadeTime)
    {
        var t = 0f;

        while(t < fadeTime)
        {
            group.alpha = Mathf.Lerp(1, 0, t / fadeTime);
            t += Time.deltaTime;
            await UniTask.WaitForEndOfFrame();
        }

        group.alpha = 0;
    }

    public async UniTask FadeIn(float fadeTime)
    {
        var t = 0f;

        while (t < fadeTime)
        {
            group.alpha = Mathf.Lerp(0, 1, t / fadeTime);
            t += Time.deltaTime;
            await UniTask.WaitForEndOfFrame();
        }

        group.alpha = 1;
    }

    private float CalculateMoveSpeed(SortedDictionary<float, float> dict, float totalPercent)
    {
        var nextPair = dict.First(pair => pair.Key > totalPercent);
        var lastPair = dict.Last(pair => pair.Key <= totalPercent);
        var transitionPercent = (totalPercent - lastPair.Key) / (nextPair.Key - lastPair.Key);

        return Mathf.Lerp(lastPair.Value, nextPair.Value, transitionPercent);
    }
}
