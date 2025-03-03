using System.Collections.Generic;
using System.Linq;
using TMPro;
using UnityEngine;

public class LogEntryView : MonoBehaviour
{
    [SerializeField]
    private TextMeshProUGUI text;
    [SerializeField]
    private SubLogView subLogView;
    private LogDetails detailsWindow;
    
    private HashSet<int> displayedSubEntryIDs = new();
    private List<string> displayedTexts = new();
    
    public void Display(LogStateEntry entry)
    {
        if(detailsWindow != null) detailsWindow.Display(entry.subEntries.Select(sub => sub.content).ToList());

        foreach (var subEntry in entry.subEntries)
        {
            if (displayedSubEntryIDs.Contains(subEntry.ID)) continue;

            if (text.text == "")
            {
                text.text = subEntry.content;
            }
            else
            {
                subLogView.Display(subEntry.content);
            }

            displayedSubEntryIDs.Add(subEntry.ID);
            displayedTexts.Add(subEntry.content);
        }
    }

    public void OnClick()
    {
        if(displayedSubEntryIDs.Count > 1)
        {
            detailsWindow = LogDetails.Create(displayedTexts);
            detailsWindow.closed += () => detailsWindow = null;
        }
    }

    public void OnSubLogUpdateAnimationFinished() => subLogView.OnAnimatorUpdated();
}
