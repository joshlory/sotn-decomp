// Weapon ID #1. Used by weapons:
// Knuckle duster, Jewel knuckles, Iron Fist, Fist of Tulkas, Unknown#186,
// Unknown#187, Unknown#188
#include "weapon_private.h"
#include "shared.h"

INCLUDE_ASM("weapon/nonmatchings/w_001", EntityWeaponAttack);

INCLUDE_ASM("weapon/nonmatchings/w_001", func_ptr_80170004);

INCLUDE_ASM("weapon/nonmatchings/w_001", func_ptr_80170008);

void func_ptr_8017000C(Entity* self) {}

void func_ptr_80170010(Entity* self) {}

void func_ptr_80170014(Entity* self) {}

int GetWeaponId(void) { return 1; }

void EntityWeaponShieldSpell(Entity* self) {}

void func_ptr_80170024(Entity* self) {}

void func_ptr_80170028(Entity* self) {}

void WeaponUnused2C(void) {}

void WeaponUnused30(void) {}

void WeaponUnused34(void) {}

void WeaponUnused38(void) {}

void WeaponUnused3C(void) {}