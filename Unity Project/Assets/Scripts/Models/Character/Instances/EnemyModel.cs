public class EnemyModel : CharacterModel
{
    public override string characterNameKey => "GenericEnemy";
    public override CharacterModel owner => this;

    protected override int baseHP => 20;
    protected override int baseDMG => 0;

    public override void AfterRegistration()
    {
        AddCard(interactions.CreateInstance<AttackCardModel>());
        AddCard(interactions.CreateInstance<AttackCardModel>());
        AddCard(interactions.CreateInstance<AttackCardModel>());
        AddCard(interactions.CreateInstance<AttackCardModel>());
    }
}
