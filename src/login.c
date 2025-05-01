#include "login.h"

#include "ui_style.h"

#include "raygui.h"

#define USERNAME_MAX 32
#define MDP_MAX 32
#define NUM_ELEMENTS 6
// @TODO: Unfinished
void ui_login(GameData* g){
    Vector2 cursor = {.x=g->win.w * 0.5f,.y = g->win.h * 0.5f - (TEXTBOX_HEIGHT +PADDING) * NUM_ELEMENTS};
    static bool is_create_account = false;

    static bool username_edit = false;
    static char username[USERNAME_MAX] = {0};
    Rectangle bounds = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    GuiLabel(bounds,"Nom d'utilisateur");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiTextBox(bounds,username,USERNAME_MAX,username_edit)){
        //Do logic on Enter
        username_edit = !username_edit;
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds,"Mot de passe");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    static bool mdp_edit = false;
    static char mdp[MDP_MAX] = {0};
    if(GuiTextBox(bounds,mdp,MDP_MAX,mdp_edit)){
        //Do logic on Enter
        mdp_edit = !mdp_edit;
    }
    if(is_create_account){
        bounds.y += TEXTBOX_HEIGHT + PADDING;
        GuiLabel(bounds,"Valider mot de passe");
        bounds.y += TEXTBOX_HEIGHT + PADDING;
        static bool mdp_edit = false;
        static char mdp[MDP_MAX] = {0};
        if(GuiTextBox(bounds,mdp,MDP_MAX,mdp_edit)){
            //Do logic on Enter
            mdp_edit = !mdp_edit;
        }
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label =  is_create_account ? "Create account" : "Login";
    if(GuiButton(bounds,butt_label)){
        //@TODO: Save to db
        g->state = MainMenu;
    }
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    bounds.width = bounds.height = TEXTBOX_HEIGHT;
    GuiCheckBox(bounds,"New Account",&is_create_account);

}
void create_account(char* username, char* mdp,GameData* g){

}