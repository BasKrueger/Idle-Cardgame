using System.Collections.Generic;
using UnityEngine;

public static class ResourceSpriteCache 
{
    private static Dictionary<string, Sprite> cache = new();

    public static Sprite Get(string path)
    {
        if (cache.ContainsKey(path))
        {
            return cache[path];
        }

        var sprite = Resources.Load<Sprite>(path);
        cache.Add(path, sprite);
        return sprite;
    }
}
