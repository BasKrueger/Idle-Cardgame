using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class CardBuffViewManager : MonoBehaviour
{
    [SerializeField]
    private RectTransform rect;
    [SerializeField]
    private HorizontalLayoutGroup group;
    [SerializeField]
    private BuffView buffViewTemplate;

    [SerializeField]
    private Vector2 sizePercent = new Vector2(0.2f, 0.2f);
    [SerializeField]
    private Vector2 paddingPercent = new Vector2(0.075f, 0.1f);

    private Dictionary<int, BuffView> activeBuffViews = new();

    public void Display(CardState state)
    {
        SpawnMissingViews(state.buffs);
        DestroyExcessViews(state.buffs);
        UpdateViews(state.buffs);
        UpdateIconSize();
    }

    private void SpawnMissingViews(List<BuffState> states)
    {
        foreach (var state in states)
        {
            if (activeBuffViews.ContainsKey(state.id)) continue;

            var view = Instantiate(buffViewTemplate, group.transform);
            activeBuffViews.Add(state.id, view);
        }
    }

    private void DestroyExcessViews(List<BuffState> states)
    {
        var toRemove = new List<int>();

        foreach(var pair in activeBuffViews)
        {
            if (states.Any(state => state.id == pair.Key)) continue;

            toRemove.Add(pair.Key);
            Destroy(pair.Value.gameObject);
        }

        toRemove.ForEach(val => activeBuffViews.Remove(val));
    }

    private void UpdateViews(List<BuffState> states)
    {
        states.ForEach(state => activeBuffViews[state.id].Display(state));
    }

    public void UpdateIconSize()
    {
        var rectSize = new Vector2(rect.rect.width, rect.rect.height);
        var padding = paddingPercent * rectSize;
        var buffSize = rectSize - padding - sizePercent * rectSize;

        group.padding.top = Mathf.RoundToInt(buffSize.y);
        group.padding.bottom = Mathf.RoundToInt(padding.y);
        group.padding.left = Mathf.RoundToInt(rectSize.x - padding.x - rectSize.x * sizePercent.x * transform.childCount);
        group.padding.right = Mathf.RoundToInt(padding.x);

        LayoutRebuilder.ForceRebuildLayoutImmediate(rect);
    }
}
