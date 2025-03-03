using Cysharp.Threading.Tasks;
using UnityEngine;

public class TargetFollower : MonoBehaviour
{
    [SerializeField]
    private float speed = 500;

    private Vector3 overridePosition;
    private Transform target;

    private UniTaskCompletionSource reachedSource;
    
    public UniTask SetTarget(Transform target)
    {
        this.target = target;
        reachedSource = new UniTaskCompletionSource();
        return reachedSource.Task;
    }
    
    public void OverrideGlobalPosition(bool active, Vector2 pos = default)
    {
        overridePosition = active ? pos : default;
    }
    
    private void LateUpdate()
    {
        if(overridePosition != default)
        {
            transform.position = overridePosition;
        }
        
        if(target != default && transform.position != target.position)
        {
            var newPos = Vector3.MoveTowards(transform.position, target.position, speed * Time.fixedDeltaTime);
            transform.position = newPos;
            overridePosition = overridePosition == default ? default : newPos;
            
            if (newPos == target.position)
            {
                reachedSource.TrySetResult();
            }
        }
    }
}
