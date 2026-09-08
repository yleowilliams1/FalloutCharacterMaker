#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "item.h"
#include "character.h"
#define WEAPON_PATH "data/weapon/"
#define ARMOR_PATH "data/armor/"

static void parse_weapon(struct config_pack p, void *ptr);
static void parse_armor(struct config_pack p, void *ptr);

void load_items(struct ItemManager *man){
	for(int i = 0; i < ITEM_COUNT; i++){
		char *weapon_path = format_path(WEAPON_PATH, "ini", i);
		char *armor_path  = format_path(ARMOR_PATH,  "ini", i);
		snprintf(man->weapons[i].name, ITEM_NAME_SIZE, "Weapon %02d", i + 1);
		snprintf(man->armors[i].name,  ITEM_NAME_SIZE, "Armor %02d",  i + 1);
		config(&man->weapons[i], weapon_path, parse_weapon);
		config(&man->armors[i], armor_path, parse_armor);	
		free(weapon_path);
		free(armor_path);
	}
}

static void parse_weapon(struct config_pack p, void *ptr){
	if(!ptr){return;}
	struct Weapon *w = (struct Weapon *)ptr;
	if(check(p.key, "Name")){
		strncpy(w->name, p.value, ITEM_NAME_SIZE - 1);
		w->name[ITEM_NAME_SIZE - 1] = '\0';
	}
	if(check(p.key, "ApCost")){
		t_atoi(p.value, &w->ap_cost);
	}
	for(int j = 0; j < WEAPON_TYPE_COUNT; j++){
		if(!check(p.current_section, (char *)wtypestr(j))){continue;}
		for(int i = 0; i < STAT_COUNT; i++){
			if(!check(p.key, (char *)statstr(i))){continue;}
			w->stat[j] = i;
		}	
		if(check(p.key, "Amount")){
			t_atoi(p.value, &w->amount[j]);
		}
		if(check(p.key, "DiceCount")){
			t_atoi(p.value, &w->damage_dice_count[j]);
		}
		if(check(p.key, "DiceFaces")){
			t_atoi(p.value, &w->damage_dice_faces[j]);
		}
	}
}
static void parse_armor(struct config_pack p, void *ptr){
	if(!ptr){return;}
	struct Armor *a = (struct Armor *)ptr;
	if(check(p.key, "Name")){
		strncpy(a->name, p.value, ITEM_NAME_SIZE - 1);
		a->name[ITEM_NAME_SIZE - 1] = '\0';
	}
	if(check(p.key, "ArmorClass")){
		t_atoi(p.value, &a->armor_class);
	}	
	if(check(p.key, "DamageReduction")){
		t_atoi(p.value, &a->damage_reduction);
	}
	for(int i = 0; i < STAT_COUNT; i++){
		if(!check(p.key, (char *)statstr(i))){continue;}
		a->stat = i;
	}	
	if(check(p.key, "Amount")){
		t_atoi(p.value, &a->amount);
	}
}

