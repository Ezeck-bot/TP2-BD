#pragma once

#include "knob.h"

typedef enum {
    Nord,
    Dark_Elf, // Can only wear eleven(white yellow) armor and weapons
    Redguard,
    Orc // Orcs can only wear green cloths or Orchish(blue) armor 
} Race;

typedef enum {
    Dawnguard, // Mix armors, only purple colors for cloths or armor
    Forsworm, //Only wear light armor, no citizens. only colors close to beige for cloths or armor
    BrotherhoodOfSteel, // Only wear heavy armor, no citizens. Colors don't matter.
    Imperials // Mix armors, only red colors for cloths or armor
} Factions;

typedef enum {
    Citizen, // Can only have small axes or bows as weapons
    Merchant, // Pacifistes, so no weapon equipped
    Soldier // Any weapon
} SocialStatus;

typedef struct {
    Race race;
    Factions faction;
    SocialStatus status;
} Classe;

typedef struct {
    char name[16];
    int value;
    int is_weapon;
    int is_armor;
    int is_equipped;
} Items;

typedef struct {
    Items items;
    size_t count;
    size_t capacity;
} Inventory;

typedef struct {
    char name[16];
    int level;
    int strength;
    int stamina;
    int stealth;
    int charisma; // aka rizz
    Classe character_classe;
} Character;

typedef struct {
    float x;
    float y;
    float w;
    float h;
} WinRect;

typedef enum {
    Login,
    MainMenu,
    CreateCharacter,
    Store,
    Exit
}Game_State;

typedef struct {
    Items* items;
    size_t count;
    size_t capacity;
    int ggbucks_amount;
} Wallet;

typedef struct {
    char card_number[17]; // Entre 15 et 16 nombre
    char name_on_card[32]; // Refuser caractere special, les accents dans les noms
    char cvc_number[4]; // 3 chiffres
} CreditCard;

typedef struct {
    char* username;
    char* mdp;
    Character curr_character;
    Wallet bag_of_holding;
    CreditCard my_card;
} Client;

typedef struct Texture Texture;

typedef struct {
    Game_State state;
    Client customer;
    WinRect win;
    Texture* img;
    #ifdef _DEBUG
    int debug;
    #endif
}GameData;

void* allocate(size_t size);

void game_init(GameData* g);
int game_update(GameData* g);
void game_preview_player(WinRect sub_rect,GameData* g);
void game_end(GameData* g);

void ui_main_menu(GameData* g);
void ui_create_char(GameData* g);