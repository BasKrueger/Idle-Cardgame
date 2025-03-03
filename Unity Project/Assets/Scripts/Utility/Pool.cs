using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class Pool<T> where T : IPoolable
{
    private List<T> pooled = new List<T>();

    private Transform content;
    private T toPool;

    public Pool(T toPool, Transform lifeTimeDependency, int instances)
    {
        content = new GameObject().transform;
        content.transform.SetParent(lifeTimeDependency);
        content.gameObject.name = $"{toPool.gameObject.name} Pool";

        this.toPool = toPool;

        for (int i = 0; i < instances; i++)
        {
            CreateInstance();
        }
    }

    public T GetInstance()
    {
        if (pooled.Count == 0) CreateInstance();

        var instance = pooled.First();
        instance.gameObject.SetActive(true);
        pooled.Remove(instance);
        return instance;
    }

    public async void Return(T toReturn, float delay = 0)
    {
        if (delay > 0) await UniTask.WaitForSeconds(delay);

        toReturn.OnPoolEntered();
        toReturn.gameObject.SetActive(false);
        toReturn.transform.SetParent(content);
        pooled.Add(toReturn);
    }

    public void CreateInstance()
    {
        var instance = GameObject.Instantiate(toPool.gameObject).GetComponent<T>();
        instance.transform.SetParent(content);
        pooled.Add(instance);
    }
}

public interface IPoolable
{
    public void OnPoolEntered();
    GameObject gameObject { get; }
    Transform transform { get; }
}