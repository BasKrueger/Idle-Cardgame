
public class IdleEncounter : EncounterModel
{
    public override string encounterName => "Idle";
    public override bool completeCondition => variables["tickCooldown"] <= 0;
    public override bool isStaticEncounter => false;

    public IdleEncounter() : base()
    {
        variables.Add("tickCooldown", 15);
    }

    public override void Tick(PlayerModel player)
    {
        variables["tickCooldown"]--;
        base.Tick(player);
    }
}
