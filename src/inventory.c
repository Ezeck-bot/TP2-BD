#include "inventory.h"
#include "database.h"

#include "ui_style.h"

#include "raygui.h"
#define NUM_ELEMENTS 6

void ui_inventory(GameData* g){
    Vector2 cursor = {.x=g->win.w * 0.5f,.y = g->win.h * 0.5f - (TEXTBOX_HEIGHT +PADDING) * NUM_ELEMENTS};

    Rectangle bounds_stats = {
        .x=cursor.x - g->win.w * 0.65f * 0.5f,
        .y=60,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    int ggbucks = database_see_ggbucks(g);
    char buffer[20];
    sprintf(buffer, "%d ggbucks", ggbucks); 
    GuiLabel(bounds_stats,buffer);
    bounds_stats.y += TEXTBOX_HEIGHT + PADDING;
    if(GuiButton(bounds_stats,"ADD GGBUCKS")){
        g->state = CreditCardd;
    }

    int count;
    Inventaire* inventaire_bd = (Inventaire*)database_show_inventory(g, &count);

    float itemWidth = g->win.w * 0.20f; // Largeur d'un item (20% de la largeur de la fenêtre)
    float spacing = PADDING; // Espacement horizontal entre les items
    float startX = cursor.x - (itemWidth * 4 + spacing * 3) / 2.0f; // Position de départ centrée pour 4 items

    for (size_t i = 0; i < count; i++) {
        int row = i / 4;
        int col = i % 4;

        float posX = startX + col * (itemWidth + spacing);
        float posY = 90 + TEXTBOX_HEIGHT + PADDING + row * (2 * TEXTBOX_HEIGHT + 2 * PADDING);
        if (row >= 1) {
            posY += PADDING + 30;
        }

        Rectangle bounds_see = {
            .x = posX,
            .y = posY,
            .width = itemWidth,
            .height = TEXTBOX_HEIGHT
        };

        char info_items[20];
        sprintf(info_items, "%s - price : %d ggbucks", inventaire_bd[i].name, inventaire_bd[i].value); 
        GuiLabel(bounds_see, info_items);

        bounds_see.y += TEXTBOX_HEIGHT + PADDING;
        if (GuiButton(bounds_see, "SEE")) {

        }
    }

    Rectangle bounds = {
        .x=550 - g->win.w * 0.25f * 0.5f,
        .y=60,
        .width=g->win.w * 0.25f,.height=TEXTBOX_HEIGHT
    };
    bounds.y += TEXTBOX_HEIGHT + PADDING;
    char* butt_label = "Return to menu";
    if(GuiButton(bounds,butt_label)){
        g->state = MainMenu;
    }
}