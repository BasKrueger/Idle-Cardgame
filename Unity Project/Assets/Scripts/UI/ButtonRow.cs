using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class ButtonRow : MonoBehaviour
{
    [SerializeField]
    private GameUIScreens screens;

    [SerializeField]
    private List<Button> buttons;
    
    private void Awake()
    {
        buttons.ForEach(b => b.onClick.AddListener(() => OnButtonPressed(b)));
    }
    
    public async void OnButtonPressed(Button button)
    {
        buttons.ForEach(b => b.interactable = false);
        await screens.MoveToScreen((GameUIScreens.gameScreens)buttons.IndexOf(button));
        buttons.Where(b => b != button).ToList().ForEach(b => b.interactable = true);
    }
}
