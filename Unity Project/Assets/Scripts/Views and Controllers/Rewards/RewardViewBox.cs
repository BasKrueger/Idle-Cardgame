using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class RewardViewBox : MonoBehaviour
{
    public event Action<int, CardView> RewardSelected;

    [SerializeField] 
    private Transform content;

    [SerializeField]
    private CardView cardTemplate;
    [SerializeField]
    private TextMeshProUGUI upcomingBoxesLabel;

    private int id;

    public void Display(RewardState reward) 
    {
        id = reward.ID;

        foreach(CardView t in content.GetComponentsInChildren<CardView>())
        {
            Destroy(t.gameObject);
        }

        foreach(var state in reward.cards)
        {
            var instance = Instantiate(cardTemplate);
            instance.Content.Show(state);
            instance.Content.ShowAsMedium();
            instance.Content.SetFill(1);
            instance.Clicked += OnCardSelected;

            instance.transform.SetParent(content);
        }
    }

    private void OnCardSelected(CardView view)
    {
        RewardSelected(id, view);
        view.Clicked -= OnCardSelected;
    }

    public void SetRewardsLeftLabel(int rewardsLeft)
    {
        upcomingBoxesLabel.gameObject.SetActive(rewardsLeft > 0);
        upcomingBoxesLabel.text = string.Format(upcomingBoxesLabel.text, rewardsLeft + 1);
        upcomingBoxesLabel.transform.SetAsLastSibling();
    }
}
