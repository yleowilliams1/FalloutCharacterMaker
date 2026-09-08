#pragma once
#include <stdio.h>
#include "character.h"
#define ITEM_COUNT 20

#define WEAPON_TYPE\
	X(Bludgeoning)\
	X(Piercing)\
	X(Ranged)\
	X(Slashing)\
	X(Magic)
enum WeaponTypes{
	#define X(name) name,
	WEAPON_TYPE
	#undef X
	WEAPON_TYPE_COUNT,
};
static inline const char *wtypestr(enum WeaponTypes type){
	switch(type){
		#define X(name) case name: return #name;
		WEAPON_TYPE
		#undef X
		default: return NULL;
	}
}
#define ITEM_NAME_SIZE 32
struct Armor{
	char name[ITEM_NAME_SIZE];
	int armor_class;
	int damage_reduction;
	enum StatEnum stat;
	int amount;
};
struct Weapon{
	char name[ITEM_NAME_SIZE];
	int damage_dice_count[WEAPON_TYPE_COUNT];
	int damage_dice_faces[WEAPON_TYPE_COUNT];
	int ap_cost;
	enum StatEnum stat[WEAPON_TYPE_COUNT];
	int amount[WEAPON_TYPE_COUNT];
};
struct ItemManager{
	struct Armor armors[ITEM_COUNT];
	struct Weapon weapons[ITEM_COUNT];
};

void load_items(struct ItemManager *man);
