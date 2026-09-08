#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include <sys/stat.h>
#include <stdlib.h>
#include <raylib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "util/util.h"
#include "raylib-nuklear.h"
#include "ui.h"
#include "item.h"
int main(int argc, char *argv[]){
	struct Stats stats = {0}; 
	if(argc >= 2){
		if(check(argv[1], "NEW")){
			for (int i = 0; i < STAT_COUNT; i++){
				stats.d[i].value = 5;
				stats.d[i].min   = 1;
				stats.d[i].max   = 16;
			}
			strcpy(stats.name, "Unnamed");
			stats.age = 18;
			strcpy(stats.ageStr, "18");
			strcpy(stats.pronouns, "he/him");
		}
	} else{	
		stats = read_stat();

		
	}
	struct ItemManager *items = XCALLOC(1, sizeof(struct ItemManager));
	load_items(items);
	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	initalize_pran();
	init_window("Character Builder");
	update_window();
	Texture2D background = LoadTexture("CharacterSheet.png");
	
	Font uiFont = LoadFontEx("fallout.ttf", 72, NULL, 0); // baked large
	SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // smooths scaling either direction
	struct nk_context *ctx = InitNuklearEx(uiFont, 40);
	ctx->style.text.color = nk_rgb(59, 247, 6);	

	while(!WindowShouldClose()){
		update_window();

		// Same fit-to-window math you already use to draw the background
		float scaleX = (float)GetScreenWidth()  / background.width;
		float scaleY = (float)GetScreenHeight() / background.height;
		float scale  = fminf(scaleX, scaleY);

		float destWidth  = background.width  * scale;
		float destHeight = background.height * scale;
		float offsetX = (GetScreenWidth()  - destWidth)  / 2.0f;
		float offsetY = (GetScreenHeight() - destHeight) / 2.0f;

		SetNuklearScaling(ctx, scale);       // scale the whole UI to match the image
		update_ui(ctx, offsetX, offsetY, scale, &stats, items); // now takes the transform
		UpdateNuklear(ctx);

		BeginDrawing();
		ClearBackground(BLACK);

		Rectangle source = { 0, 0, (float)background.width, (float)background.height };
		Rectangle dest   = { offsetX, offsetY, destWidth, destHeight };
		DrawTexturePro(background, source, dest, (Vector2){0,0}, 0.0f, WHITE);

		DrawNuklear(ctx);
		EndDrawing();
	}

	CloseWindow();
	free(items);
	write_stat(stats);
	UnloadNuklear(ctx);
	UnloadTexture(background);
	return 0;
}
