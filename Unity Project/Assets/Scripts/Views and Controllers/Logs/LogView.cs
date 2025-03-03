using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class LogView : MonoBehaviour, IGameViewAsync
{
    [SerializeField]
    private RectTransform content;

    [SerializeField]
    private LogEntryView entryTemplate;

    private Dictionary<int, LogEntryView> activeLogs = new();

    private void Awake()
    {
        foreach (Transform t in content)
        {
            Destroy(t.gameObject);
        }
    }

    public async UniTask OnGameStateUpdate(GameState gameState)
    {
        DestroyMissingLogs(gameState.log);
        SpawnMissingLogs(gameState.log);
        UpdateExistingLogs(gameState.log);
    }

    private void SpawnMissingLogs(LogState logState)
    {
        foreach (var entry in logState.entries)
        {
            if (!activeLogs.ContainsKey(entry.ID))
            {
                var view = Instantiate(entryTemplate, content, false);
                activeLogs.Add(entry.ID, view);
            }
        }
    }

    private void UpdateExistingLogs(LogState logState)
    {
        logState.entries.ForEach(entry => activeLogs[entry.ID].Display(entry));
    }
    
    private void DestroyMissingLogs(LogState logState)
    {
        var copy = new Dictionary<int, LogEntryView>(activeLogs);
        
        foreach (var valuePair in copy)
        {
            if (logState.entries.All(entry => entry.ID != valuePair.Key))
            {
                var log = valuePair.Value;
                activeLogs.Remove(valuePair.Key);
                Destroy(log.gameObject);
            }
        }
    }

    private void LateUpdate()
    {
        LayoutRebuilder.ForceRebuildLayoutImmediate(content);
    }
}
