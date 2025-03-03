using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class LogDetails : MonoBehaviour
{
    public event Action closed;
    
    [SerializeField]
    private Animator anim;
    [SerializeField]
    private TextMeshProUGUI text;
    [SerializeField]
    private LogEntryView entryTemplate;
    private RectTransform rect;

    public static LogDetails Create(List<string> logs)
    {
        var window = Instantiate(Resources.Load("LogDetails") as GameObject).GetComponent<LogDetails>();
        window.rect = window.GetComponent<RectTransform>();

        return window;
    }
    
    public void OnCloseClicked()
    {
        anim.SetTrigger("Hide");
        this.enabled = false;

        closed?.Invoke();
        
        Destroy(this.gameObject, 1);
    }
    
    public void Display(List<string> logs)
    {
        if (!this.enabled) return;

        text.text = "";
        logs.ForEach(log => text.text += $"{log}\n\n");
    }
    
    private void LateUpdate() => LayoutRebuilder.ForceRebuildLayoutImmediate(rect);
}
