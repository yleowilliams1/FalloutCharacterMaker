#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ui.h"
#include "raylib-nuklear.h"
#include "character.h"
#include "item.h"
static inline struct nk_rect overlay_rect(float offsetX, float offsetY, float scale,
                                           float x, float y, float w, float h){
return nk_rect(offsetX / scale + x, offsetY / scale + y, w, h);
}
typedef struct {

	const char *label;
	float dialX, dialY, dialW, dialH;   
	float boxX,  boxY,  boxW,  boxH;    
} StatRow;
#define DIAL_X 155
#define DIAL_W 113

#define BOX_X 283
#define BOX_W 32
#define ST 0
#define DE 1
#define SO 2
#define IN 3
#define WI 4
#define PE 5
#define LO 6
static StatRow stats[] = {
	{ "ST", DIAL_X,179,DIAL_W,80,  BOX_X,180,BOX_W,75},
	{ "DE", DIAL_X,282,DIAL_W,71,  BOX_X,280,BOX_W,70},
	{ "SO", DIAL_X,382,DIAL_W,68,  BOX_X,382,BOX_W,65},
	{ "IN", DIAL_X,486,DIAL_W,67,  BOX_X,485,BOX_W,64},
	{ "WI", DIAL_X,579,DIAL_W,71,  BOX_X,579,BOX_W,66},
	{ "PE", DIAL_X,680,DIAL_W,68,  BOX_X,681,BOX_W,64},
	{ "LO", DIAL_X,783,DIAL_W,68,  BOX_X,784,BOX_W,65},
};
#define NUM_STATS (sizeof(stats) / sizeof(stats[0]))
#define GOLD_TEXT   nk_rgb(59, 247, 6)
#define GOLD_HOVER  nk_rgb(240, 210, 140)

#define ITEM_X 1100 
#define ITEM_Y 923
#define ITEM_END_X 1100 
#define ITEM_END_Y 154
static void push_arrow_style(struct nk_context *ctx){
	nk_style_push_style_item(ctx, &ctx->style.button.normal, nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.button.hover,  nk_style_item_color(nk_rgba(255,255,255,25)));
	nk_style_push_style_item(ctx, &ctx->style.button.active, nk_style_item_color(nk_rgba(255,255,255,55)));
	nk_style_push_float(ctx, &ctx->style.button.border, 0.0f);
	nk_style_push_float(ctx, &ctx->style.button.rounding, 3.0f);
	nk_style_push_color(ctx, &ctx->style.button.text_normal, GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.button.text_hover,  GOLD_HOVER);
	nk_style_push_color(ctx, &ctx->style.button.text_active, GOLD_HOVER);
}

static void pop_arrow_style(struct nk_context *ctx){
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_float(ctx);
	nk_style_pop_float(ctx);
	nk_style_pop_style_item(ctx);
	nk_style_pop_style_item(ctx);
	nk_style_pop_style_item(ctx);
}
#define HEIGHT 62
static void push_transparent_window(struct nk_context *ctx){
	nk_style_push_style_item(ctx, &ctx->style.window.fixed_background,
	                          nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_float(ctx, &ctx->style.window.border, 0.0f);
	nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(4,4));
}

static void pop_transparent_window(struct nk_context *ctx){
	nk_style_pop_vec2(ctx);
	nk_style_pop_float(ctx);
	nk_style_pop_style_item(ctx);
}
static void push_transparent_edit(struct nk_context *ctx){
	nk_style_push_style_item(ctx, &ctx->style.edit.normal, nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.edit.hover,  nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.edit.active, nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_float(ctx, &ctx->style.edit.border, 0.0f);
	nk_style_push_color(ctx, &ctx->style.edit.text_normal, GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.edit.text_hover,  GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.edit.text_active, GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.edit.cursor_normal, GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.edit.cursor_hover,  GOLD_TEXT);
}

static void pop_transparent_edit(struct nk_context *ctx){
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_float(ctx);
	nk_style_pop_style_item(ctx);
	nk_style_pop_style_item(ctx);
	nk_style_pop_style_item(ctx);
}
static void update_identity_fields(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct Stats *data){
	struct nk_rect name_rec = overlay_rect(offsetX, offsetY, scale, 47, HEIGHT, 380, 80);
	push_transparent_window(ctx);
	if (nk_begin(ctx, "Name", name_rec, NK_WINDOW_NO_SCROLLBAR)) {
		nk_layout_row_dynamic(ctx, name_rec.h - 8, 1);
		push_transparent_edit(ctx);
		nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, data->name, sizeof(data->name), nk_filter_default);
		pop_transparent_edit(ctx);
	}
	nk_end(ctx);
	pop_transparent_window(ctx);

	struct nk_rect age_rec = overlay_rect(offsetX, offsetY, scale, 480, HEIGHT, 200, 80);
	push_transparent_window(ctx);
	if (nk_begin(ctx, "Age", age_rec, NK_WINDOW_NO_SCROLLBAR)) {
		nk_layout_row_dynamic(ctx, age_rec.h - 8, 1);
		push_transparent_edit(ctx);
		nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, data->ageStr, sizeof(data->ageStr), nk_filter_decimal);
		pop_transparent_edit(ctx);
	}
	nk_end(ctx);
	pop_transparent_window(ctx);

	struct nk_rect pronoun_rec = overlay_rect(offsetX, offsetY, scale, 750, HEIGHT, 500, 80);
	push_transparent_window(ctx);
	if (nk_begin(ctx, "Pronoun", pronoun_rec, NK_WINDOW_NO_SCROLLBAR)) {
		nk_layout_row_dynamic(ctx, pronoun_rec.h - 8, 1);
		push_transparent_edit(ctx);
		nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, data->pronouns, sizeof(data->pronouns), nk_filter_default);
		pop_transparent_edit(ctx);
	}
	nk_end(ctx);
	pop_transparent_window(ctx);
}
#define INV 0
#define APP 1
#define SPE 2
#define SMA 3
#define AP 4
#define HPL 5
#define DMB 6
#define AB 7
#define RAN 8
#define BONUS_Y 513
#define BONUS_H 333 
#define BONUS_W 460
static char *lokup[9] = {"Investigation", "Appeal", "Speech", "Smarts", "Action Points", "Health Per Level", "Damage Bonus", "Attack Bonus", "Ranged Bonus"};
static void update_bonus_fields(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct Stats *data){    
    push_transparent_window(ctx);

    // Number of elements in lokup array (9)
    int count = sizeof(lokup) / sizeof(lokup[0]); 
    float row_h = BONUS_H / (float)count;
    
    data->points_used = 0;
    for(int i = 0; i < STAT_COUNT; i++){
    	data->points_used += data->d[i].value;
    }
    struct nk_rect points_rec = overlay_rect(offsetX, offsetY, scale, 76, 940, 400, 50);
    if(nk_begin(ctx, "Points", points_rec, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)){
    	nk_layout_row_dynamic(ctx, points_rec.h - 4, 1 );	
        char valueStr[128];
	snprintf(valueStr, sizeof(valueStr), "%s: %d", "Used Points", (int)data->points_used);
	
	if(data->points_used >= 64){nk_label_colored(ctx, valueStr, NK_TEXT_LEFT, nk_rgb(255, 0, 0));}
	else{nk_label_colored(ctx, valueStr, NK_TEXT_LEFT, GOLD_TEXT);
    	}
    }
    nk_end(ctx);
    for(int i = 0; i < count; i++){
        // Calculate dynamic y position and row height for each item
        struct nk_rect name_rec = overlay_rect(
            offsetX, 
            offsetY, 
            scale, 
            560,                    // X position
            190+ (row_h * i),      // Y position (staggered per index)
            BONUS_W,                // Width
            row_h                   // Height
        );

        if (nk_begin(ctx, lokup[i], name_rec, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
            nk_layout_row_dynamic(ctx, name_rec.h - 4, 1);
            char valueStr[64];
            int value = 0;
            struct Data *d = data->d;

            switch(i){
                case INV: value = (int)ceilf((d[PE].value + d[IN].value) / 10.0f); break;
                case APP: value = (int)ceilf((d[LO].value + d[SO].value) / 10.0f); break;
                case SPE: value = (int)ceilf((d[PE].value + d[SO].value) / 10.0f); break;
                case SMA: value = (int)ceilf((d[IN].value + d[WI].value) / 10.0f); break;
                case RAN: value = (int)ceilf((d[DE].value + d[PE].value) / 10.0f); break;
                case AP:  value = (int)ceilf((d[IN].value + d[DE].value) / 2.0f);  break;
                case HPL: value = (int)ceilf((d[ST].value + d[WI].value) / 2.0f);  break;
                case DMB: value = (int)ceilf((d[ST].value + d[DE].value) / 2.0f);  break;
                case AB:  value = (int)ceilf((d[WI].value + d[LO].value) / 10.0f); break;
            }

            snprintf(valueStr, sizeof(valueStr), "%s: %d", lokup[i], value);
            nk_label_colored(ctx, valueStr, NK_TEXT_LEFT, GOLD_TEXT);
        }
        nk_end(ctx);
    }
    pop_transparent_window(ctx);
}
void update_base_stats(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct Stats *data){
	for (int i = 0; i < (int)NUM_STATS; i++){
		StatRow *s = &stats[i];

		// --- Number readout, sitting on the dial art ---
		struct nk_rect dialRect = overlay_rect(offsetX, offsetY, scale,
		                                        s->dialX, s->dialY, s->dialW, s->dialH);
		char dialName[16];
		snprintf(dialName, sizeof(dialName), "dial_%d", i);

		nk_style_push_style_item(ctx, &ctx->style.window.fixed_background,
		                          nk_style_item_color(nk_rgba(0,0,0,0)));
		if (nk_begin(ctx, dialName, dialRect, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
			nk_layout_row_dynamic(ctx, dialRect.h, 1);
			char valueStr[8];
			snprintf(valueStr, sizeof(valueStr), "%d", data->d[i].value);
			nk_label_colored(ctx, valueStr, NK_TEXT_CENTERED, nk_rgb(255, 255, 255));
		}
		nk_end(ctx);
		nk_style_pop_style_item(ctx);

		// --- Up / down buttons, stacked vertically on the black square ---
		struct nk_rect boxRect = overlay_rect(offsetX, offsetY, scale,
						       s->boxX, s->boxY, s->boxW, s->boxH);
		char boxName[16];
		snprintf(boxName, sizeof(boxName), "box_%d", i);

		nk_style_push_style_item(ctx, &ctx->style.window.fixed_background,
					  nk_style_item_color(nk_rgba(0,0,0,0)));
		nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(0,0));
		nk_style_push_float(ctx, &ctx->style.window.border, 0.0f);

		if (nk_begin(ctx, boxName, boxRect, NK_WINDOW_NO_SCROLLBAR)) {
			push_arrow_style(ctx);
			nk_layout_row_dynamic(ctx, boxRect.h / 2.0f - 2, 1);
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP)) {
				if (data->d[i].value < data->d[i].max) data->d[i].value++;
			}

			nk_layout_row_dynamic(ctx, boxRect.h / 2.0f - 2, 1);
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN)) {
				if (data->d[i].value > data->d[i].min) data->d[i].value--;
			}
			pop_arrow_style(ctx);
		}
		nk_end(ctx);

		nk_style_pop_float(ctx);
		nk_style_pop_vec2(ctx);
		nk_style_pop_style_item(ctx);
	}
}

/* --- Item list (right side big block) & detail readout (middle-bottom box) ---
   These are placeholders - tweak the X/Y/W/H below to match your art. */
#define ITEMLIST_X 1085
#define ITEMLIST_Y 180
#define ITEMLIST_W 750
#define ITEMLIST_H 880

#define DETAIL_X 548
#define DETAIL_Y 600
#define DETAIL_W 400
#define DETAIL_H 460

enum SelectedKind { SEL_NONE, SEL_WEAPON, SEL_ARMOR };
static enum SelectedKind selected_kind = SEL_NONE;
static int selected_index = 0;

static void push_transparent_selectable(struct nk_context *ctx){
	nk_style_push_style_item(ctx, &ctx->style.selectable.normal,        nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.selectable.hover,         nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.selectable.pressed,       nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.selectable.normal_active, nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.selectable.hover_active,  nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_style_item(ctx, &ctx->style.selectable.pressed_active,nk_style_item_color(nk_rgba(0,0,0,0)));
	nk_style_push_color(ctx, &ctx->style.selectable.text_normal,         GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.selectable.text_hover,          nk_rgb(255,255,255));
	nk_style_push_color(ctx, &ctx->style.selectable.text_pressed,        nk_rgb(255,255,255));
	nk_style_push_color(ctx, &ctx->style.selectable.text_normal_active,  GOLD_TEXT);
	nk_style_push_color(ctx, &ctx->style.selectable.text_hover_active,   nk_rgb(255,255,255));
	nk_style_push_color(ctx, &ctx->style.selectable.text_pressed_active, nk_rgb(255,255,255));
}

static void pop_transparent_selectable(struct nk_context *ctx){
	for(int i = 0; i < 6; i++){ nk_style_pop_color(ctx); }
	for(int i = 0; i < 6; i++){ nk_style_pop_style_item(ctx); }
}

static void update_item_lists(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct ItemManager *items){
	if(!items){return;}
	struct nk_rect rec = overlay_rect(offsetX, offsetY, scale, ITEMLIST_X, ITEMLIST_Y, ITEMLIST_W, ITEMLIST_H);
	push_transparent_window(ctx);
	if(nk_begin(ctx, "ItemList", rec, NK_WINDOW_NO_SCROLLBAR)){
		nk_layout_row_dynamic(ctx, rec.h - 8, 2);
		push_transparent_selectable(ctx);

		if(nk_group_begin(ctx, "WeaponColumn", NK_WINDOW_NO_SCROLLBAR)){
			nk_layout_row_dynamic(ctx, 28, 1);
			for(int i = 0; i < ITEM_COUNT; i++){
				nk_bool is_selected = (selected_kind == SEL_WEAPON && selected_index == i);
				if(nk_selectable_label(ctx, items->weapons[i].name, NK_TEXT_LEFT, &is_selected)){
					selected_kind = SEL_WEAPON;
					selected_index = i;
				}
			}
			nk_group_end(ctx);
		}

		if(nk_group_begin(ctx, "ArmorColumn", NK_WINDOW_NO_SCROLLBAR)){
			nk_layout_row_dynamic(ctx, 28, 1);
			for(int i = 0; i < ITEM_COUNT; i++){
				nk_bool is_selected = (selected_kind == SEL_ARMOR && selected_index == i);
				if(nk_selectable_label(ctx, items->armors[i].name, NK_TEXT_LEFT, &is_selected)){
					selected_kind = SEL_ARMOR;
					selected_index = i;
				}
			}
			nk_group_end(ctx);
		}
		pop_transparent_selectable(ctx);
	}
	nk_end(ctx);
	pop_transparent_window(ctx);
}

static const char *stat_abbr[STAT_COUNT] = { "STR","DEX","SOC","INT","WIL","PER","LOK" };
static const char *wtype_abbr[WEAPON_TYPE_COUNT] = { "Blu","Pie","Rng","Sla","Mag" };

static void update_item_details(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct ItemManager *items){
	if(!items){return;}
	struct nk_rect rec = overlay_rect(offsetX, offsetY, scale, DETAIL_X, DETAIL_Y, DETAIL_W, DETAIL_H);
	push_transparent_window(ctx);
	if(nk_begin(ctx, "ItemDetail", rec, NK_WINDOW_NO_SCROLLBAR)){
		nk_style_push_float(ctx, (float *)&ctx->style.font->height, 6.0f);
		nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0, 20));
		nk_layout_row_dynamic(ctx, 12, 1);
		char line[64];

		if(selected_kind == SEL_NONE){
			nk_label_colored(ctx, "Select an item", NK_TEXT_LEFT, GOLD_TEXT);
		}
		else if(selected_kind == SEL_WEAPON){
			struct Weapon *w = &items->weapons[selected_index];
			nk_label_colored(ctx, w->name, NK_TEXT_LEFT, GOLD_TEXT);
			snprintf(line, sizeof(line), "AP:%d", w->ap_cost);
			nk_label_colored(ctx, line, NK_TEXT_LEFT, GOLD_TEXT);
			for(int j = 0; j < WEAPON_TYPE_COUNT; j++){
				snprintf(line, sizeof(line), "%s %dd%d +%d%% %s",
				         wtype_abbr[j], w->damage_dice_count[j], w->damage_dice_faces[j],
				         w->amount[j], stat_abbr[w->stat[j]]);
				nk_label_colored(ctx, line, NK_TEXT_LEFT, GOLD_TEXT);
			}
		}
		else if(selected_kind == SEL_ARMOR){
			struct Armor *a = &items->armors[selected_index];
			nk_label_colored(ctx, a->name, NK_TEXT_LEFT, GOLD_TEXT);
			snprintf(line, sizeof(line), "AC:%d  DR:%d", a->armor_class, a->damage_reduction);
			nk_label_colored(ctx, line, NK_TEXT_LEFT, GOLD_TEXT);
			snprintf(line, sizeof(line), "Bonus +%d %s", a->amount, stat_abbr[a->stat]);
			nk_label_colored(ctx, line, NK_TEXT_LEFT, GOLD_TEXT);
		}
		nk_style_pop_vec2(ctx);
		nk_style_pop_float(ctx);
	}
	nk_end(ctx);
	pop_transparent_window(ctx);
}

void update_ui(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct Stats *data, struct ItemManager *items){
	update_base_stats(ctx, offsetX, offsetY, scale, data);
	update_identity_fields(ctx, offsetX, offsetY, scale, data);
	update_bonus_fields(ctx, offsetX, offsetY, scale, data);
	update_item_lists(ctx, offsetX, offsetY, scale, items);
	update_item_details(ctx, offsetX, offsetY, scale, items);
}
