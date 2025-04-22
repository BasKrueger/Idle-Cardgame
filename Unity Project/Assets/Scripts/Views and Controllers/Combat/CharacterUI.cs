using Cysharp.Threading.Tasks;
using System.Linq;
using UnityEngine;

public class CharacterUI : MonoBehaviour
{
    private static readonly int COMBAT_BOOL = Animator.StringToHash("Combat");
    
    [SerializeField]
    private Animator anim;
    
    [SerializeField]
    private HealthView healthbar;
    
    [SerializeField]
    private CardQueueView cardQueue;

    public async UniTask OnCharacterStateUpdateAsync(CharacterState characterState)
    {
        anim.SetBool(COMBAT_BOOL, characterState.phase == CharacterPhase.Combat);

        await cardQueue.OnGameStateUpdateAsync(characterState);
        healthbar.OnCharacterStateUpdate(characterState);
    }
    
    public void OnCharacterStateUpdate(CharacterState characterState)
    {
        cardQueue.OnGameStateUpdate(characterState);
    }
}
