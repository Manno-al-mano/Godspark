#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Weapon UMETA(DisplayName = "Equipped Weapon")
};
UENUM(BlueprintType)
enum class EActionState : uint8 
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Drawing UMETA(DisplayName = "Drawing"),
	EAS_Blocking UMETA(DisplayName = "Blocking"),
	//EAS_DodgeStart UMETA(DisplayName = "Dodge Start"),
	EAS_AttackingStart UMETA(DisplayName = "Attacking Start"),
	EAS_AttackingEnd UMETA(DisplayName = "Attacking End"),
	EAS_Staggered UMETA(DisplayName = "Staggered"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dialogue UMETA(DisplayName = "In Dialogue"),
	EAS_Dead UMETA(DisplayName = "Dead")
}; 
UENUM(BlueprintType)
enum EDeathPose{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	
	EDP_MAX UMETA(DisplayName = "DefaultMAX")
};
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Stateless UMETA(DisplayName = "No State"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Staggered UMETA(DisplayName = "Staggered")
	
};

