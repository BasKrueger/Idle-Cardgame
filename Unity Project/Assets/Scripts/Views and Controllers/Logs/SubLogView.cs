using System.Collections.Generic;
using Cysharp.Threading.Tasks;
using TMPro;
using UnityEngine;

public class SubLogView : MonoBehaviour
{
    [SerializeField]
    private Animator anim;
    [SerializeField]
    private TextMeshProUGUI current;
    [SerializeField]
    private TextMeshProUGUI next;

    private UniTaskCompletionSource animationCompletionSource;
    private List<string> fullLogs = new List<string>();
    private Queue<string> queuedLogs = new Queue<string>();

    private bool updatingPreview;
    
    public void Display(string text)
    {
        if (!anim.GetBool("ShowSubLog"))
        {
            current.text = text;
            next.text = text;
            anim.SetBool("ShowSubLog", true);
            return;
        }
       
        
        queuedLogs.Enqueue(text);
        UpdatePreview();
    }
    
    private async void UpdatePreview()
    {
        if (updatingPreview) return;

        updatingPreview = true;
        
        while(queuedLogs.Count > 0)
        {
            var text = queuedLogs.Dequeue();
            
            next.text = text;

            animationCompletionSource = new UniTaskCompletionSource();
            anim.SetTrigger("UpdateSubLog");
            await animationCompletionSource.Task;

            current.text = text;
            fullLogs.Add(text);

            await UniTask.WaitForSeconds(0.5f);
        }
        
        updatingPreview = false;
    }    
    
    //called via Animation Event
    public void OnAnimatorUpdated()
    {
        animationCompletionSource.TrySetResult();
    }
}
