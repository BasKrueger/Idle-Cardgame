using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class GameUIScreens : MonoBehaviour
{
    private static float MOVE_SCALE = 0.9f;
    private static float SCALE_SPEED = 0.07f;
    private static float MOVE_SPEED = 2500;

    public enum gameScreens {Adventure, Deck}

    [SerializeField]
    private List<RectTransform> screens;
    private RectTransform currentScreen;
    private RectTransform content;

    private void Awake()
    {
        content = GetComponent<RectTransform>();
        currentScreen = screens.First();
        
        foreach(RectTransform t in content)
        {
            t.sizeDelta = content.sizeDelta;
        }
    }
    
    public async UniTask MoveToScreen(gameScreens screen)
    {
        var nextScreen = screens[(int)screen];
        if (nextScreen == currentScreen) return;

        await screens.ToList().ForEachAsync(screen => ScaleScreenTo(screen, Vector3.one * MOVE_SCALE));
        
        while(nextScreen.position != content.position)
        {
            var previousPos = nextScreen.position;
            nextScreen.position = Vector2.MoveTowards(nextScreen.position, content.position, MOVE_SPEED * Time.deltaTime);
            var delta = previousPos - nextScreen.position;
            
            screens.Where(screen => screen != nextScreen).ToList().ForEach(screen => screen.position -= delta);
            
            await UniTask.WaitForEndOfFrame();
        }
        
        await screens.ToList().ForEachAsync(screen => ScaleScreenTo(screen, Vector3.one));
        
        currentScreen = nextScreen;
    }
    
    private async UniTask ScaleScreenTo(RectTransform screen, Vector3 scale)
    {
        while(screen.localScale != scale)
        {
            screen.localScale = Vector3.MoveTowards(screen.localScale, scale, SCALE_SPEED);
            await UniTask.WaitForEndOfFrame();
        }
    }
}

public static partial class Extensions
{
    public static async UniTask ForEachAsync<T>(this List<T> source, Func<T, UniTask> action)
    {
        foreach(var element in source)
        {
            await action(element);
        }
    }
}
