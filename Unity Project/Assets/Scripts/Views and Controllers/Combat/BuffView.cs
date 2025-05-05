using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class BuffView : MonoBehaviour
{
    [SerializeField]
    private Image image;
    [SerializeField]
    private TextMeshProUGUI durationCounter;

    public void Display(BuffState state)
    {
        image.sprite = ResourceSpriteCache.Get($"CardArts/{state.icon}");
        durationCounter.text = state.duration.ToString();
    }
}
