using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;

public class RewardView : MonoBehaviour
{
    public event Action<int, CardView> RewardClaimed;
    public event Action LastRewardClaimed;

    private const float boxMoveSpeed = 6;

    [SerializeField]
    private RewardViewBox boxTemplate;
    [SerializeField]
    private Transform content;

    private Dictionary<int, RewardViewBox> activeBoxes = new Dictionary<int, RewardViewBox>();
    private HashSet<int> claimedRewards = new HashSet<int>();

    private void Awake()
    {
        foreach(Transform t in content)
        {
            Destroy(t.gameObject);
        }
    }

    public void Display(RewardStashState rewardState)
    {
        for(int i = 0;i < rewardState.earnedRewards.Count;i++)
        {
            var state = rewardState.earnedRewards[i];

            if (activeBoxes.ContainsKey(state.ID) || claimedRewards.Contains(state.ID)) continue;

            SpawnBox(state);
        }

        var values = activeBoxes.Values.ToList();
        for(int i = 0; i < activeBoxes.Count; i++)
        {
            values[activeBoxes.Count - 1 - i].SetRewardsLeftLabel(i);
        }
    }

    private void SpawnBox(RewardState state)
    {
        var posY = content.transform.localPosition.y;
        var posX = activeBoxes.Count == 0 ? 0 : activeBoxes.Last().Value.transform.localPosition.x + activeBoxes.Last().Value.GetComponent<RectTransform>().rect.width;

        var box = Instantiate(boxTemplate, content);
        box.Display(state);
        box.RewardSelected += OnRewardSelected;

        box.transform.SetParent(content, false);
        box.transform.localPosition = new Vector3(posX, posY);

        activeBoxes.Add(state.ID, box);
    }

    private async void OnRewardSelected(int rewardID, CardView selectedCard)
    {
        RewardClaimed?.Invoke(rewardID, selectedCard);
        await MoveToNextReward();

        Destroy(activeBoxes[rewardID].gameObject);
        activeBoxes.Remove(rewardID);
        claimedRewards.Add(rewardID);

        if(activeBoxes.Count == 0)
        {
            content.transform.localPosition -= new Vector3(content.transform.localPosition.x,0);
            LastRewardClaimed?.Invoke();
        }
    }

    private async UniTask MoveToNextReward()
    {
        var posY = content.transform.localPosition.y;
        var posX = content.transform.localPosition.x - activeBoxes.First().Value.GetComponent<RectTransform>().rect.width;

        var targetPos = new Vector3(posX, posY);

        while (content.transform.localPosition != targetPos)
        {
            content.transform.localPosition = Vector3.MoveTowards(content.transform.localPosition, targetPos, boxMoveSpeed * Time.deltaTime * Screen.width);
            await UniTask.WaitForEndOfFrame();
        }
    }
}
