#pragma once 
#include <stdint.h>
struct nk_context;
struct Stats;
struct ItemManager;
void update_ui(struct nk_context *ctx, float offsetX, float offsetY, float scale, struct Stats *d, struct ItemManager *items);
