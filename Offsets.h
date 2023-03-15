#pragma once

#define oGameObjectManager 0x36546E0
#define oBaseNetworkable 0x3653550
// Base networkable
//      "Name": "Class$BaseNetworkable",
//      "Signature": "BaseNetworkable_c*"


#define oItemContainer 0x28 //public ItemContainer containerBelt;
#define oListList 0x38 //	public List<Item> contents;

//public class PlayerModel : ListComponent<PlayerModel>, IOnParentDestroying
#define oNewVelocity 0x23C // private Vector3 newVelocity;

//public class BasePlayer : BaseCombatEntity
#define oModel 0x130 //public Model model;
#define oLifeState 0x224// public BaseCombatEntity.LifeState lifestate
#define oHealth 0x22c// private float _health;
#define oSmoothVelocity 0x23C// private Vector3 newVelocity
#define oVisible 0x288// internal bool visible;
#define oPlayerInput 0x4F0// public PlayerInput input;
#define oPlayerModel 0x4D0// public PlayerModel playerModel;
#define oBaseMovement 0x4F8// public BaseMovement movement;
#define oActiveItem 0x5D8 // private uint clActiveItem;
#define oDisplayName 0x6F0// protected string _displayName
#define oPlayerInventory 0x6A0 // public PlayerInventory inventory;
#define oclothingWaterSpeedBonus 0x768 // public float clothingWaterSpeedBonus;
#define oFrozen 0x4D8 // public bool Frozen;
#define oMovement 0x4F8// public BaseMovement movement;
#define oPlayerFlags 0x690// public BasePlayer.PlayerFlags playerFlags;
#define oClothingAccuracyBonus 0x76C// public float clothingAccuracyBonus;

		//*** PlayerInput ***//
#define obodyAngles 0x3C // private Vector3 bodyAngles;
#define oRecoilAngles 0x64 // public Vector3 recoilAngles;


		//*** Player Model ***//


//public class Model : MonoBehaviour, IPrefabPreProcess // TypeDefIndex: 9383
#define oBoneTransforms 0x48 // public Transform[] boneTransforms;



//public class PlayerWalkMovement : BaseMovement
#define oGroundAngle 0xC4// private float groundAngle;
#define oGroundAngleNew 0xC8// private float groundAngleNew;
#define oGravityMultiplier 0x84// public float gravityMultiplier;
//#define oFlying 0x14C// private bool flying;
#define oFlying 0x149// private bool flying;
#define oGrounded 0x140//private bool grounded;

#define oGravityMultiplierSwimming 0x88 // public float gravityMultiplierSwimming;


//public class Item
//#define oHeldEntity 0x98// private EntityRef heldEntity; 
#define oHeldEntity 0xA0// private EntityRef heldEntity; 

//public class BaseMelee : AttackEntity // TypeDefIndex: 8850
#define oAttackRadius 0x29C // public float attackRadius;
#define oisAutomatic 0x2A0//public bool isAutomatic;
#define omaxDistance 0x298 //public float maxDistance;
#define oBlockSprintOnAttack 0x2A1 //public bool blockSprintOnAttack;
#define oonlyThrowAsProjectile 0x282 //public bool onlyThrowAsProjectile;
#define othrowReady 0x2D8 //private bool throwReady;



//public class BaseProjectile : AttackEntity
#define oRecoil 0x2E0 // public RecoilProperties recoil;
#define oAutomatic 0x290 // public bool automatic;
#define oStancePenalty 0x348 //private float stancePenalty;
#define oAimconePenalty 0x34C //private float aimconePenalty;
#define oAimCone 0x2F0 //public float aimCone;
#define oHipAimCone 0x2F4 //public float hipAimCone;
#define oAimconePenaltyPerShot 0x2F8 //public float aimconePenaltyPerShot;


//public class RecoilProperties : ScriptableObject
#define oRecoilYawMin 0x18 // public float recoilYawMin;
#define oRecoilYawMax 0x1C // public float recoilYawMax;
#define oRecoilPitchMin 0x20 // public float recoilPitchMin;
#define oRecoilPitchMax 0x24  // public float recoilPitchMax;
#define oADSScale 0x30 // public float ADSScale; 
#define oMovementPenalty 0x34 // public float movementPenalty;