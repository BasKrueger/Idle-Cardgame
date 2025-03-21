using Cysharp.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class StatView : MonoBehaviour
{
    private const float updateTime = 0.65f;

    [SerializeField]
    private RectTransform minMaxContent;
    [SerializeField]
    private RectTransform absoluteContent;

    [SerializeField]
    private TextMeshProUGUI currentValue;
    [SerializeField]
    private TextMeshProUGUI maxValue;
    [SerializeField]
    private TextMeshProUGUI absoluteValue;

    [field: SerializeField]
    public Image icon { get; private set; }

    public bool updatesEnabled = true;

    private UniTaskCompletionSource cancelUpdate;

    public void ShowValue(int value, bool instant = false)
    {
        if (!updatesEnabled) return;

        minMaxContent.gameObject.SetActive(false);
        absoluteContent.gameObject.SetActive(true);

        if (instant)
        {
            absoluteValue.text = value.ToString();
        }
        else
        {
            UpdateValue(absoluteValue, value);
        }
    }

    public void ShowValue(int value, int baseValue, bool instant = false)
    {
        if (!updatesEnabled) return;

        minMaxContent.gameObject.SetActive(true);
        absoluteContent.gameObject.SetActive(false);

        if (instant)
        {
            currentValue.text = value.ToString();
            maxValue.text = baseValue.ToString();
        }
        else
        {
            UpdateValue(currentValue, value);
            UpdateValue(maxValue, baseValue);
        }
    }

    private async void UpdateValue(TextMeshProUGUI label, int targetValue)
    {
        if(!int.TryParse(label.text, out var startValue))
        {
            return;
        }

        cancelUpdate?.TrySetResult();
        cancelUpdate = new UniTaskCompletionSource();

        float time = 0;
        while(time < updateTime)
        {
            var value = Mathf.Lerp(startValue, targetValue, time / updateTime);

            label.text = Mathf.RoundToInt(value).ToString();

            time += Time.deltaTime;
            await UniTask.WaitForEndOfFrame();
        }

        label.text = targetValue.ToString();

        cancelUpdate?.TrySetResult();
    }
}
