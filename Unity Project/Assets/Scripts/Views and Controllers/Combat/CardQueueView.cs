using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEditor;
using UnityEngine;

public class CardQueueView : MonoBehaviour
{
    [SerializeField]
    private CardView mikroViewTemplate;
    [SerializeField]
    private Transform content;
    [SerializeField]
    private List<Vector3> previewPositions;

    private Pool<CardView> viewPool;
    private Dictionary<int, CardView> activeViews = new Dictionary<int, CardView>();

    private void Awake()
    {
        viewPool = new Pool<CardView>(mikroViewTemplate, this.transform, previewPositions.Count + 1);
    }
    
    public void OnGameStateUpdate(CharacterState characterState)
    {
        var statesToDisplay = characterState.autoDeck.Take(previewPositions.Count).ToDictionary(card => card.id, card=> card);

        DestroyRemovedCards(statesToDisplay);
        SpawnNewCards(statesToDisplay);
        MoveExistingCards(statesToDisplay);
        UpdateCardSize(characterState);
        UpdateCardContent(statesToDisplay);
        UpdateSiblingOrder(statesToDisplay);
    }
    
    private void DestroyRemovedCards(Dictionary<int, CardState> validStates)
    {
        var copy = new Dictionary<int, CardView>(activeViews);
        
        foreach(var activePair in copy)
        {
            if(!validStates.ContainsKey(activePair.Key))
            {
                activeViews[activePair.Key].ShowPlayed(viewPool);
                activeViews.Remove(activePair.Key);
            }
        }
    }
    
    private void SpawnNewCards(Dictionary<int, CardState> validStates)
    {
        var missingStates = validStates.Where(pair => !activeViews.ContainsKey(pair.Key)).ToArray();
        
        for(int i =0;i < missingStates.Length; i++)
        {
            var view = viewPool.GetInstance();
            
            view.transform.SetParent(content);
            view.transform.localPosition = previewPositions[i];
            view.transform.SetAsLastSibling();

            activeViews.Add(missingStates[i].Key, view);
        }
    }
    
    private void MoveExistingCards(Dictionary<int, CardState> validStates)
    {
        int i = 0;
        foreach(var pair in validStates)
        {
            MoveCardOverTime(activeViews[pair.Key], previewPositions[i], 0.25f);
            i++;
        }
    }
    
    private void UpdateCardSize(CharacterState characterState)
    {
        activeViews[characterState.autoDeck.First().id].Content.ShowAsMini();
        
        activeViews.Where(pair => pair.Key != characterState.autoDeck.First().id)
        .ToList().ForEach(pair => pair.Value.Content.ShowAsMikro());
    }

    private void UpdateCardContent(Dictionary<int, CardState> validStates)
    {
        foreach (var pair in validStates)
        {
            activeViews[pair.Key].Content.UpdateFill(pair.Value);
            activeViews[pair.Key].Content.UpdateTexts(pair.Value);
        }
    }
    
    private void UpdateSiblingOrder(Dictionary<int, CardState> validStates)
    {
        activeViews[validStates.Last().Key].transform.SetAsFirstSibling();
    }
    
    private async void MoveCardOverTime(CardView card, Vector3 target, float time)
    {
        if (card.transform.position == target) return;
        
        var startPosition = card.transform.localPosition;
        var timer = 0f;
        
        while(timer < time)
        {
            if (card == null) return;            
            
            await UniTask.WaitForEndOfFrame();
            card.transform.localPosition = Vector3.Lerp(startPosition, target, timer / time);
            timer += Time.deltaTime;
        }

        card.transform.localPosition = target;
    }
}
