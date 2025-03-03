using Cysharp.Threading.Tasks;
using UnityEngine;

public class CharacterSpriteView : MonoBehaviour
{
    private readonly int ATTACK_TRIGGER = Animator.StringToHash("Attack");
    private readonly int DAMAGETAKEN_TRIGGER = Animator.StringToHash("TakeDamage");
    private readonly int DEAD_BOOL = Animator.StringToHash("Dead");
    private readonly int COMBAT_BOOL = Animator.StringToHash("Fight");
    private readonly int RUN_BOOL = Animator.StringToHash("Run");

    [SerializeField]
    private Animator anim;

    private UniTaskCompletionSource animationCompletionSource;

    public async UniTask OnCharacterStateUpdateAsync(CharacterState characterState)
    {
        anim.SetBool(COMBAT_BOOL, characterState.phase == CharacterPhase.Combat);
        anim.SetBool(DEAD_BOOL, characterState.phase == CharacterPhase.Dead);

        switch (characterState.animationTrigger)
        {
            case CharacterAnimationTrigger.CardPlayed:
                await AnimationTrigger(ATTACK_TRIGGER);
                break;

            case CharacterAnimationTrigger.Damaged:
                await AnimationTrigger(DAMAGETAKEN_TRIGGER);
                break;
        }
    }
    
    public void SetRunning(bool running) => anim.SetBool(RUN_BOOL, running);
    public void OnAnimationTrigger() => animationCompletionSource?.TrySetResult();

    private UniTask AnimationTrigger(int trigger)
    {
        animationCompletionSource = new UniTaskCompletionSource();
        anim.SetTrigger(trigger);
        return animationCompletionSource.Task;
    }
}
