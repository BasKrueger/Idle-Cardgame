using Cysharp.Threading.Tasks;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class ParticleUI : MonoBehaviour
{
    private class TrackingData
    {
        public Transform ogParent;
        public Vector3 lastParentPos;
        public Vector3 ogPos;
        public int ogIndex;
    }

    private Canvas canvas_;
    public Canvas canvas
    {
        get
        {
            Transform current = transform;
            while(canvas_ == null)
            {
                canvas_ = current.GetComponent<Canvas>();

                current = current.parent;
            }
            return canvas_;
        }
    }
    public static ParticleUI mainInstance { get; private set; }

    [SerializeField]
    private bool main;
    private Dictionary<Transform, TrackingData> trackedData = new();

    private void Awake()
    {
        if (main)
        {
            if(mainInstance != null) 
            {
                Debug.LogError("Error: Duplicated main particle canvas");
                return;
            }

            mainInstance = this;
        }

        Transform current = transform;
        while (canvas_ == null)
        {
            canvas_ = current.GetComponent<Canvas>();
            current = current.parent;
        }
    }

    public static void AddMain(Transform t, bool trackData = true) => mainInstance.Add(t, trackData);
    public void Add(Transform t, bool recordReturnData = true)
    {
        if (recordReturnData)
        {
            var data = new TrackingData
            {
                ogParent = t.transform.parent,
                lastParentPos = t.transform.parent.position,
                ogPos = t.transform.localPosition,
                ogIndex = t.transform.GetSiblingIndex(),
            };

            if (trackedData.ContainsKey(t)) trackedData[t] = data;
            else trackedData.Add(t, data);
        }

        t.transform.SetParent(transform);
    }

    public static bool ClearDataMain(Transform t) => mainInstance.TryClearData(t);
    public bool TryClearData(Transform t)
    {
        if (!trackedData.ContainsKey(t)) return false;
        trackedData.Remove(t);
        return true;
    }

    public static void TryReturnMain(Transform t) => mainInstance.TryReturn(t);
    public bool TryReturn(Transform t)
    {
        if (!trackedData.ContainsKey(t)) return false;

        t.transform.SetParent(trackedData[t].ogParent);
        t.transform.SetSiblingIndex(trackedData[t].ogIndex);
        t.transform.localPosition = trackedData[t].ogPos;

        trackedData.Remove(t);

        return true;
    }

    public static async UniTask<List<UIParticle>> SplatterMain(UIParticle particleTemplate, int count, Vector3 position, float force = 1, float rad = 1, SortedDictionary<float, float> speedCurve = null) => await mainInstance.Splatter(particleTemplate, count, position, force, rad, speedCurve);
    public async UniTask<List<UIParticle>> Splatter (UIParticle particleTemplate, int count, Vector3 position, float force = 1, float rad = 1, SortedDictionary<float, float> speedCurve = null)
    {
        if (speedCurve == null)
        {
            speedCurve = new SortedDictionary<float, float>
            {
                { 0.25f, 1500 * force },
                { 0.85f, 100 * force },
                { 1, 25 * force}
            };
        }

        float minDist = 75 * rad;
        float maxDist = 150 * rad;

        var particles = new List<UIParticle>();
        var tasks = new List<UniTask.Awaiter>();

        for(int i = 0;i < count; i++)
        {
            var particle = Instantiate(particleTemplate);
            particle.transform.SetParent(transform);
            particle.transform.position = position;

            var dir = new Vector3(UnityEngine.Random.Range(-1f, 1f), UnityEngine.Random.Range(-1f, 1f));
            var dist = new Vector3(UnityEngine.Random.Range(minDist, maxDist), UnityEngine.Random.Range(minDist, maxDist));
            var splatterPos = position + Vector3.Scale(dir, dist);

            tasks.Add(particle.MoveWithCustomEase(splatterPos, speedCurve).GetAwaiter());

            particles.Add(particle);

            if (i % 2 == 0)
            {
                await UniTask.WaitForEndOfFrame();
            }
        }

        while (tasks.Any(awaiter => !awaiter.IsCompleted)) await UniTask.WaitForEndOfFrame();

        return particles;
    }

    private void LateUpdate()
    {
        foreach(var tData in trackedData)
        {
            if (tData.Key == null) continue;
            var delta = tData.Value.ogParent.transform.position - tData.Value.lastParentPos;
            tData.Key.transform.position += delta;

            tData.Value.lastParentPos = tData.Value.ogParent.transform.position;
        }
    }
}
