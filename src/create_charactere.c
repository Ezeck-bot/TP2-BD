#include "create_charactere.h"
#include "database.h"

#include "ui_style.h"

#include "raygui.h"

#define USERNAME_MAX 32
#define STAT_MAX 3
#define NUM_ELEMENTS 6

const char *races = "Nord;Dark_Elf;Redguard;Orc";
const char *factions = "Dawnguard;Forsworn;BrotherhoodOfSteel;Imperials";
const char *status = "Citizen;Merchant;Soldier";

// Indices des sélections actives
int race_active = 0;
int faction_active = 0;
int status_active = 0;

// États d'édition des menus déroulants
bool race_edit = false;
bool faction_edit = false;
bool status_edit = false;

void ui_create_char(GameData* g){
    Vector2 cursor = {.x=g->win.w * 0.5f,.y = g->win.h * 0.5f - (TEXTBOX_HEIGHT +PADDING) * NUM_ELEMENTS};
    Rectangle box = {.x= cursor.x,.y=cursor.y,.width=200,.height=50};

    Rectangle bounds_stats = {
        .x=cursor.x - g->win.w * 0.65f * 0.5f,
        .y=60,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(bounds_stats,"+20 stats");
    bounds_stats.y += TEXTBOX_HEIGHT + PADDING;

    static bool username_edit = false;
    static char username[USERNAME_MAX] = {0};
    Rectangle bounds = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(bounds,"Name Charactere");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(bounds,username,USERNAME_MAX,username_edit)){
        //Do logic on Enter
        username_edit = !username_edit;
    }
    
    //Début classe
    Rectangle bounds_2 = {
        .x=165 - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 1.95f * 0.7f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds_2.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds_2,"Choose race");
    bounds_2.y += TEXTBOX_HEIGHT + PADDING;
    if (GuiDropdownBox(bounds_2, races, &race_active, race_edit)) {
            race_edit = !race_edit;
    }

    Rectangle bounds_4 = {
        .x=550 - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 1.95f * 0.7f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds_4.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds_4,"Choose social");
    bounds_4.y += TEXTBOX_HEIGHT + PADDING;
    if (GuiDropdownBox(bounds_4, status, &status_active, status_edit)) {
            status_edit = !status_edit;
    }
    //FIN CLASSE

    static bool level_edit = false;
    static char level[STAT_MAX] = {0};
    Rectangle level_5 = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 2.5f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(level_5,"Level");
    level_5.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(level_5,level,STAT_MAX,level_edit)){
        //Do logic on Enter
        level_edit = !level_edit;
    }

    static bool strength_edit = false;
    static char strength[STAT_MAX] = {0};
    Rectangle strength_5 = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 3.1f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(strength_5,"Strength");
    strength_5.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(strength_5,strength,STAT_MAX,strength_edit)){
        //Do logic on Enter
        strength_edit = !strength_edit;
    }

    static bool stamina_edit = false;
    static char stamina[STAT_MAX] = {0};
    Rectangle stamina_5 = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 3.6f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(stamina_5,"Stamina");
    stamina_5.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(stamina_5,stamina,STAT_MAX,stamina_edit)){
        //Do logic on Enter
        stamina_edit = !stamina_edit;
    }

    static bool stealth_edit = false;
    static char stealth[STAT_MAX] = {0};
    Rectangle stealth_5 = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 4.1f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(stealth_5,"Stealth");
    stealth_5.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(stealth_5,stealth,STAT_MAX,stealth_edit)){
        //Do logic on Enter
        stealth_edit = !stealth_edit;
    }

    static bool charisma_edit = false;
    static char charisma[STAT_MAX] = {0};
    Rectangle charisma_5 = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 4.6f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(charisma_5,"Charisma");
    charisma_5.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(charisma_5,charisma,STAT_MAX,charisma_edit)){
        //Do logic on Enter
        charisma_edit = !charisma_edit;
    }

    Rectangle bounds_3 = {
        .x=358 - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 1.95f * 0.7f + TEXTBOX_HEIGHT + PADDING,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(bounds_3,"Choose faction");
    bounds_3.y += TEXTBOX_HEIGHT + PADDING;
    if (GuiDropdownBox(bounds_3, factions, &faction_active, faction_edit)) {
            faction_edit = !faction_edit;
    }

    Rectangle bounds_menu = {
        .x=550 - g->win.w * 0.25f * 0.5f,
        .y=60,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds_menu.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label_menu = "Return to menu";
    if(GuiButton(bounds_menu,butt_label_menu)){
        g->state = MainMenu;
    }
    
    Rectangle save_bounds = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 5.3f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label = "Validate";
    if(GuiButton(save_bounds,butt_label)){

        int level_int = atoi(level);
        int strength_int = atoi(strength);
        int stamina_int = atoi(stamina);
        int stealth_int = atoi(stealth);
        int charisma_int = atoi(charisma);

        int points_restants = level_int + strength_int + stamina_int + stealth_int + charisma_int;

        if ((faction_active == 1 && status_active == 0) || // Forsworn avec Citizen
            (faction_active == 2 && status_active == 0)) { // BrotherhoodOfSteel avec Citizen
            knob_log(KNOB_INFO, "Erreur : Combinaison invalide (Forsworn/BrotherhoodOfSteel avec Citizen");
        } else {
            //vérification si la classe existe

            int classId = verification_class_exists(g, race_active, faction_active, status_active);
            if(classId != 0){
                if (points_restants > 20)
                {
                    knob_log(KNOB_INFO, "Nombre de point de stat superieur a 20.");
                } else {
                    database_save_character(g, username, classId, level_int, strength_int, stamina_int, stealth_int, charisma_int);

                    g->state = MainMenu;
                }
            } else {
                knob_log(KNOB_INFO, "Classe inexistante dans la base de donnee");
            }
        }
    }

    WinRect sub = {
        .x= cursor.x + bounds.width,
        .y=cursor.y * 3.1f * 0.9f,
        .w=g->win.w - bounds.width - cursor.x,
        .h=g->win.h - cursor.y
    };
    game_preview_player(sub,g, race_active);
}
