using TMPro;
using UnityEngine;

public class StatView : MonoBehaviour
{
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

    private int lastDisplayedValue_ = -1;

    public void ShowValue(int value)
    {
        minMaxContent.gameObject.SetActive(false);
        absoluteContent.gameObject.SetActive(true);

        absoluteValue.text = value.ToString();
        lastDisplayedValue = value;
    }

    public void ShowValue(int value, int baseValue)
    {
        minMaxContent.gameObject.SetActive(true);
        absoluteContent.gameObject.SetActive(false);

        currentValue.text = value.ToString();
        maxValue.text = baseValue.ToString();

        lastDisplayedValue = value;
    }

    private int lastDisplayedValue
    {
        get => lastDisplayedValue_;
        set
        {
            lastDisplayedValue_ = value;
        }
    }
}
