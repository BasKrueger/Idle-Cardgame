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

    public async UniTask DisplayPercent(float percent)
    {
        targetFill = percent;
        UpdateTargetFill();

        await UniTask.WaitUntil(() => !updatingFill);
    }

    public async UniTask DisplayText(string key, params object[] formatValues)
    {
        var locString = new LocalizedString("Menus", key);
        label.text = string.Format(locString.GetLocalizedString(), formatValues);

        await UniTask.WaitForEndOfFrame();
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
