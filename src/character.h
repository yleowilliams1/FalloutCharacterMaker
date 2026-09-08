#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "util/util.h"
#define STAT_LIST\
	X(STRENGTH)\
	X(DEXTERITY)\
	X(SOCIAL)\
	X(INTELLIGENCE)\
	X(WILLPOWER)\
	X(PERCEPTION)\
	X(LOOKS)
#define BONUS_LIST\
	X(Investigate)\
	X(Appeal)\
	X(Speech)\
	X(Smarts)\
	X(ActionPoints)\
	X(HealthPerLevel)\
	X(DamageBonus)\
	X(AttackBonus)\
	X(RangedBonus)
enum StatEnum{
	#define X(name) name,
	STAT_LIST
	#undef X
	STAT_COUNT,
};
enum BonusEnum{
	#define X(name) name,
	BONUS_LIST
	#undef X
	BONUS_COUNT,
};
static inline const char *statstr(enum StatEnum type){
	switch(type){
		#define X(name) case name: return #name;
		STAT_LIST
		#undef X
		default: return NULL;
	}
}
static inline const char *bonusstr(enum BonusEnum bonus){
	switch(bonus){
		#define X(name) case name: return #name;
		BONUS_LIST
		#undef X
		default: return NULL;
	}
}
#pragma pack(push, 1)
struct Header{
	uint64_t magic_number;
	uint32_t time_stamp;
	uint16_t endian_check;
	uint16_t version;
};
#pragma pack(pop)
struct Data{
	uint32_t value;
	uint32_t min;
	uint32_t max;
};

#define NAME_SIZE 19
#define AGE_SIZE 4
#define PRONOUN_SIZE 16
struct Stats{
	struct Data d[STAT_COUNT];
	uint32_t points_used;
	uint32_t age;
	char ageStr[AGE_SIZE];
	char name[NAME_SIZE];
	char pronouns[PRONOUN_SIZE];
};
void swap_stats(struct Stats *s);
void write_stat(struct Stats stats);
struct Stats read_stat();
