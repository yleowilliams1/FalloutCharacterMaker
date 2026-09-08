#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "util/util.h"
#include "character.h"
#include "item.h"
#define MAGIC_NUMBER "KokomoBB"
#define ENDIAN_CHECK 0x0102
#define VERSION 1
void change_data(struct Stats *s, enum StatEnum stat, uint32_t value, uint32_t min, uint32_t max){if(s){s->d[stat].value = value; s->d[stat].min = min; s->d[stat].max = max;}}
void change_name(struct Stats *s, char *name){if(sizeof(name) != NAME_SIZE){return;}if(!s){return;}strcpy(s->name, name);}
void change_age(struct Stats *s, uint32_t age){if(s){s->age = age;}}
void update_points(struct Stats *s){if(s){s->points_used = 0;for(int i = 0; i < STAT_COUNT; i++){s->points_used += s->d[i].value;}}}

void swap_stats(struct Stats *s){
	if(!s){return;}
	for(int i = 0; i < STAT_COUNT; i++){
		s->d[i].value = swap32(s->d[i].value);
		s->d[i].min   = swap32(s->d[i].min);
		s->d[i].max   = swap32(s->d[i].max);
	}
	s->points_used = swap32(s->points_used);
	s->age         = swap32(s->age);
	/* ageStr, name, pronouns are char buffers - no swap needed */
}

void write_stat(struct Stats stats){
	FILE *f = fopen("character.SAVE", "wb");
	if(!f){LOG(NO_FILE, "Failed to open file for writing"); return;}

	struct Header h = {0};
	memcpy(&h.magic_number, MAGIC_NUMBER, sizeof(h.magic_number));	
	h.time_stamp = (uint32_t)time(NULL);
	h.endian_check = ENDIAN_CHECK;
	h.version = VERSION;
	
	bool failed = false;
	if(!failed && fwrite(&h, sizeof(h), 1, f) != 1){failed = true;}
	if(!failed && fwrite(&stats, sizeof(stats), 1, f) != 1){failed = true;}
	fclose(f);
	if(failed){LOG(NO_FILE, "Failed to fwrite");}
}
struct Stats read_stat(){
	FILE *f = fopen("character.SAVE", "rb");
	struct Stats s = {0};
	if(!f){LOG(NO_FILE, "Failed to open"); return s;}
	
	struct Header h = {0};
	if(fread(&h, sizeof(h), 1, f) != 1){fclose(f); LOG(READ, "Failed to read header"); return s;}
	
	uint64_t expected_magic = 0;
	memcpy(&expected_magic, MAGIC_NUMBER, sizeof(expected_magic));

	if(h.magic_number != expected_magic){LOG(PARSE, "Magic number was invalid");fclose(f);return s;}
	
	bool need_swap = false;
	if(h.endian_check == ENDIAN_CHECK){need_swap = false;}
	else if(swap16(h.endian_check) == ENDIAN_CHECK){need_swap = true; h.version = swap16(h.version); h.time_stamp = swap32(h.time_stamp);}
	else{LOG(PARSE, "File is corrupted");fclose(f); return s;}	
	if(h.version != VERSION){LOG(PARSE, "Version may not be supported");}
	
	if(fread(&s, sizeof(s), 1, f) != 1){LOG(NO_FILE, "Failed to read file");fclose(f); return (struct Stats){0};}	
	if(need_swap){swap_stats(&s);}
	
	fclose(f);
	return s;
}
