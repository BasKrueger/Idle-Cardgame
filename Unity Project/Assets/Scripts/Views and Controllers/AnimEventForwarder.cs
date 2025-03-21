using System;
using UnityEngine;

public class AnimEventForwarder : MonoBehaviour
{
    public event Action rewardFinished;

    public void OnRewardFinished() => rewardFinished?.Invoke();
}
