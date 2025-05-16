#include "credit_card.h"
#include "database.h"

#include "ui_style.h"

#include "raygui.h"

#define CARD_NUMBER 17
#define CARD_EXPIRATION 6
#define CARD_NAME 32
#define CARD_CVC 4
#define NUMBER_GGBUCKS 32
#define NUM_ELEMENTS 6
void ui_add_credit_card(GameData* g){
    Vector2 cursor = {.x=g->win.w * 0.5f,.y = g->win.h * 0.5f - (TEXTBOX_HEIGHT +PADDING) * NUM_ELEMENTS};
    static bool is_resgister_card = false;

    static bool card_name_edit = false;
    static char card_name[CARD_NUMBER] = {0};

    Rectangle bounds = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds,"Card number");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    static bool card_number_edit = false;
    static char card_number[CARD_NUMBER] = {0};
    if(GuiTextBox(bounds,card_number,CARD_NUMBER,card_number_edit)){
        //Do logic on Enter
        card_number_edit = !card_number_edit;
    }

    bounds.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds,"Card expiration");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    static bool card_expiration_edit = false;
    static char card_expiration[CARD_EXPIRATION] = {0};
    if(GuiTextBox(bounds,card_expiration,CARD_EXPIRATION,card_expiration_edit)){
        //Do logic on Enter
        card_expiration_edit = !card_expiration_edit;
    }

    bounds.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds,"Card CVC");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    static bool card_cvc_edit = false;
    static char card_cvc[CARD_CVC] = {0};
    if(GuiTextBox(bounds,card_cvc,CARD_CVC,card_cvc_edit)){
        //Do logic on Enter
        card_cvc_edit = !card_cvc_edit;
    }

    bounds.y += TEXTBOX_HEIGHT + PADDING;
    GuiLabel(bounds,"How many ggbucks do you want? (one ggbucks equals $1 CAD)");
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    static bool number_ggbucks_edit = false;
    static char number_ggbucks[NUMBER_GGBUCKS] = {0};
    if(GuiTextBox(bounds,number_ggbucks,NUMBER_GGBUCKS,number_ggbucks_edit)){
        //Do logic on Enter
        number_ggbucks_edit = !number_ggbucks_edit;
    }

    bounds.y += TEXTBOX_HEIGHT + PADDING;
    bounds.width = bounds.height = TEXTBOX_HEIGHT;
    GuiCheckBox(bounds,"Register Card",&is_resgister_card);

    Rectangle bounds_menu = {
        .x=550 - g->win.w * 0.25f * 0.5f,
        .y=60,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds_menu.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label_menu = "Return to store";
    if(GuiButton(bounds_menu,butt_label_menu)){
        g->state = Store;
    }

    Rectangle save_bounds = {
        .x=cursor.x - g->win.w * 0.25f * 0.5f,
        .y=cursor.y * 5.3f * 0.9f,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label = "MAKE PAYEMENT";
    if(GuiButton(save_bounds,butt_label)){

        if (strlen(number_ggbucks) != 0)
        {
            int card_number_int = atoi(card_number);
            int card_cvc_int = atoi(card_cvc);
            int number_ggbucks_int = atoi(number_ggbucks);

            database_add_credit_card(g, card_number_int, card_expiration, card_cvc_int, number_ggbucks_int, is_resgister_card);
            g->state = Store;
        }
        
    }
}