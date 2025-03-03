using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using Unity.Mathematics;
using UnityEngine;

public class EncounterView : MonoBehaviour
{
    [SerializeField]
    private CharacterView characterViewTemplate;

    [SerializeField]
    private Transform NpcPos;

    private Dictionary<int, CharacterView> activeNPCs = new Dictionary<int, CharacterView>();

    public async UniTask OnEncounterStateUpdateAsync(EncounterState encounterState)
    {
        await SpawnMissingNPCs(encounterState);
        await DestroyRemovedNPCs(encounterState);
        await UpdateNPCsAsync(encounterState);
    }
    
    public void OnEncounterStateUpdate(EncounterState encounterState)
    {
        UpdateNPCs(encounterState);
    }

    private async UniTask SpawnMissingNPCs(EncounterState encounterState)
    {
        foreach(var npc in encounterState.NPCs)
        {
            if(!activeNPCs.ContainsKey(npc.id))
            {
                var spawnPosition = NpcPos.transform.position += new Vector3(UnityEngine.Random.Range(-0.1f, 0.1f), UnityEngine.Random.Range(-0.1f, 0.1f));
                var view = Instantiate(characterViewTemplate, spawnPosition, quaternion.identity, transform);
                activeNPCs.Add(npc.id, view);

                await UniTask.WaitForSeconds(0.1f);
            }
        }
    }
    
    private async UniTask DestroyRemovedNPCs(EncounterState encounterState)
    {
        var keysToRemove = activeNPCs.Where(pair => !encounterState.NPCsByID.ContainsKey(pair.Key)).Select(pair => pair.Key).ToList();
        if (keysToRemove == null) return;
        
        foreach(var key in keysToRemove)
        {
            Destroy(activeNPCs[key].gameObject);
            
            activeNPCs.Remove(key);

            await UniTask.WaitForSeconds(0.05f);
        }
    }
    
    private async UniTask UpdateNPCsAsync(EncounterState encounterState)
    {
        foreach(var NPC in encounterState.NPCs)
        {
            await activeNPCs[NPC.id].OnCharacterStateUpdateAsync(NPC);
        }
    }
    
    private void UpdateNPCs(EncounterState encounterState)
    {
        encounterState.NPCs.ToList().Where(encounter => activeNPCs.ContainsKey(encounter.id)).ToList()
        .ForEach(NPC => activeNPCs[NPC.id].OnCharacterStateUpdate(NPC));
    }
}
