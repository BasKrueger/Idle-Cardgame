using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class DeckView : MonoBehaviour, IGameView
{
    public event Action<CardView> CardSelected;
    
    [SerializeField]
    private Transform content;
    [SerializeField]
    private CardView viewModel;
    [SerializeField]
    private TargetFollower marker;

    private Dictionary<int, CardView> activeViews = new();
    
    private void Awake()
    {
        foreach(var t in content.GetComponentsInChildren<CardView>())
        {
            Destroy(t.gameObject);
        }
    }
        
    public void OnGameStateUpdate(GameState gameState)
    {
        TrySpawnCards(gameState.player.generic);
        UpdateMarker(gameState.player.generic);
    }
    
    private void TrySpawnCards(CharacterState playerState)
    {
        if (activeViews.Count > 0) return;

        foreach(var card in playerState.autoDeck)
        {
            var instance = Instantiate(viewModel, content, false);
            instance.Content.UpdateTexts(card);
            instance.transform.gameObject.name = card.cardDescription;
            
            instance.Clicked += CardSelected.Invoke;
            instance.ContentChanged += OnCardContentChanged;
            
            instance.Content.ShowAsMedium();
            activeViews.Add(card.id, instance);
        }

        marker.transform.SetAsLastSibling();
    }
    
    private void OnCardContentChanged(CardViewContent oldContent, CardView view)
    {
        if (oldContent != null && activeViews.ContainsKey(oldContent.displayID)) 
        {
            activeViews.Remove(oldContent.displayID);
        }

        activeViews.TryAdd(view.Content.displayID, view);
    }
    
    private void UpdateMarker(CharacterState playerState)
    {
        if(playerState.autoDeckInPlayOrder.Count == 0)
        {
            Debug.LogError("Error: deck is empty");
            return;
        }

        marker.SetTarget(activeViews[playerState.autoDeckInPlayOrder.First().id].transform);
    }
}
