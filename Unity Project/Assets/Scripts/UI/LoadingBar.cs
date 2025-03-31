using Cysharp.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.Localization;
using UnityEngine.UI;

public class LoadingBar : MonoBehaviour
{
    private const float fillSpeed = 3f;

    [SerializeField]
    private Image progress;

    [SerializeField]
    private TextMeshProUGUI label;
    private float targetFill;
    public bool updatingFill { get; private set; }

    public void DisplayPercent(float percent)
    {
        targetFill = percent;
        UpdateTargetFill();
    }

    public void DisplayText(string key, params object[] formatValues)
    {
        var locString = new LocalizedString("Menus", key);
        label.text = string.Format(locString.GetLocalizedString(), formatValues);
    }

    private async void UpdateTargetFill()
    {
        if (updatingFill) return;
        updatingFill = true;

        while(progress.fillAmount != targetFill)
        {
            progress.fillAmount = Mathf.MoveTowards(progress.fillAmount, targetFill, fillSpeed * Time.deltaTime);
            await UniTask.WaitForEndOfFrame();
        }

        updatingFill = false;
    }
}
