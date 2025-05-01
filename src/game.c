#include "game.h"

#include <assert.h>
#include <string.h>

#include "raylib.h"
#include "raygui.h"

#include "ui_style.h"
#include "login.h"
#include "store.h"


void game_preview_player(WinRect sub_rect,GameData* g){
    #define BUMP 16 + 1 
    Rectangle src ={
        .x = 0,
        .y=g->customer.curr_character.character_classe.race * BUMP,
        .width=16,
        .height=16
    };
    Rectangle dst = *(Rectangle*)&sub_rect;
    dst.height = sub_rect.w;
    DrawTexturePro(*g->img,src,dst,(Vector2){0},0,WHITE);
    if(g->debug){
        DrawRectangleLinesEx(*(Rectangle*)&sub_rect,2.0f,RED);   
    }
}
void game_init(GameData* g){
    InitWindow(g->win.w,g->win.h,"GoodGame™");
    SetTargetFPS(60);

    GuiLoadStyleDefault();
    
    g->img = allocate(sizeof(Texture));
    Texture2D temp = LoadTexture("caracteres.png");
    memcpy(g->img,&temp,sizeof(Texture));

    memset(&g->customer,0,sizeof(Client));
    g->state = Login;
}

int game_update(GameData* g){
    if(WindowShouldClose() || g->state == Exit) return 1;
    BeginDrawing();
    ClearBackground(BLACK);
    
    Rectangle check_bounds = {
        .x=0,
        .y=0,
        .width=TEXTBOX_HEIGHT,
        .height=TEXTBOX_HEIGHT,
    };
    #ifdef _DEBUG // Va appaitre seulement lorsque nous sommes en mode debug.
    GuiCheckBox(check_bounds,"Debug UI",(bool*)&g->debug);
    #endif

    switch(g->state){
        case Login:{
            ui_login(g);
            break;
        }
        case MainMenu:{
            ui_main_menu(g);
            break;
        }
        case CreateCharacter: {
            ui_create_char(g);
            break;
        }
        case Store:{
            ui_store(g);
            break;
        }
        default:
            assert(0 && "Unsupported App state...");
    }
    EndDrawing();
    
    return 0;
}
void game_end(GameData* g){
    //Unload everything that needs to be unloaded here
    // All memory that comes from allocate is freed in the entrypoint.c
    UnloadTexture(*g->img);

    CloseWindow();
}

#define NUM_ELEMENTS 4
void ui_main_menu(GameData* g){
    Vector2 cursor = {.x=g->win.w * 0.5f - g->win.w * 0.25f * 0.5f,.y = g->win.h * 0.5f - (TEXTBOX_HEIGHT +PADDING) * NUM_ELEMENTS};
    static bool is_create_account = false;
    Rectangle bounds = {
        .x=cursor.x,
        .y=cursor.y,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };

    GuiDisable();
    if(GuiButton(bounds,"Play game")){
        //@TODO(JS): Add a game maybe :) ? 
    }
    GuiEnable();
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Créer Personnage")){
        g->state = CreateCharacter;
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Magasin")){
        //@TODO: Save to db
        g->state = Store;
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Quitter")){
        g->state = Exit;
    }

    WinRect sub = {
        .x= cursor.x + bounds.width,
        .y=cursor.y,
        .w=g->win.w - bounds.width - cursor.x,
        .h=g->win.h - cursor.y
    };
    game_preview_player(sub,g);
}

void ui_create_char(GameData* g){
    assert(0 && "@TODO: Implemente me");
}