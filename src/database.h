#pragma once
#include "game.h"


void database_start(GameData* g);
void database_init_tables(GameData* g);
void database_insert_tables(GameData* g);
int database_verification_insert_table(GameData* g);
void database_end(GameData* g);

void database_create_user(GameData* g, char* username, char* mdp);

int database_login_user(GameData* g, char* username, char* mdp);

int verification_class_exists(GameData* g, int race_active, int faction_active, int status_active);

void database_save_character(GameData* g, char* username, int classId, int level, int strength, int stamina, int stealth, int charisma);

int database_charactere_race_nombre(GameData* g);

int database_charactere_race(GameData* g);

void database_add_credit_card(GameData* g, int card_number, char* card_expiration, int card_cvc, int number_ggbucks, int is_register_card);

int database_see_ggbucks(GameData* g);

typedef struct {
    int id_inventaire;
    char name[256];
    int value;
} Item;

Item* data_see_items(GameData* g, int* count);

void database_add_inventory(GameData* g, int id_items, int value_items);

int database_inventory_nombre(GameData* g);

typedef struct {
    int id_inventaire;
    char name[256];
    int value;
} Inventaire;

Inventaire* database_show_inventory(GameData* g, int* count);