#include "game.h"

#include <assert.h>
#include <string.h>

#include "raylib.h"
#include "raygui.h"

#include "ui_style.h"
#include "login.h"
#include "create_charactere.h"
#include "credit_card.h"
#include "inventory.h"
#include "store.h"

#include "database.h"


void game_preview_player(WinRect sub_rect,GameData* g, int race){
    #define BUMP 16
    Rectangle src ={
        .x = 0,
        .y=race * BUMP + race * 1,
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

    database_start(g);
    database_init_tables(g);
    if(database_verification_insert_table(g) == 0){
        
    
        database_insert_tables(g);
    }

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
        case Inventoryy:{
            ui_inventory(g);
            break;
        }
        case CreditCardd:{
            ui_add_credit_card(g);
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

    database_end(g);
}

#define NUM_ELEMENTS 4
void ui_main_menu(GameData* g){
    //cherchons si'il a déja un personnage
    int nombre_race = database_charactere_race_nombre(g);
    int nombre_iventory = database_inventory_nombre(g);


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
    if (nombre_race)
    {
        GuiDisable();
    } else {
        GuiEnable();
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Créer Personnage")){
        g->state = CreateCharacter;
    }

    if (nombre_iventory != 0)
    {
        GuiEnable();
    } else {
        GuiDisable();
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Inventory")){
        //@TODO: Save to db
        g->state = Inventoryy;
    }

    GuiEnable();
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Magasin")){
        //@TODO: Save to db
        g->state = Store;
    }

    
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds,"Quitter")){
        g->state = Exit;
    }

    
    if (nombre_race != 0)
    {
        int race = database_charactere_race(g);

        WinRect sub = {
            .x= cursor.x + bounds.width,
            .y=cursor.y,
            .w=g->win.w - bounds.width - cursor.x,
            .h=g->win.h - cursor.y
        };
        game_preview_player(sub,g, race);
        

    }
}