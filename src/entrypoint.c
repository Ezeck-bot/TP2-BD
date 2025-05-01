#include "entrypoint.h"
#include "sqlite3.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define KNOB_IMPLEMENTATION
#include "knob.h"

#include "game.h"
#include "login.h"


// #define LOG_SQLITE3_ERROR(db) TraceLog(LOG_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

#define HEAP_SIZE 2048 * 2048 * 4
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

#define WINDOW_W 720
#define WINDOW_H 480

GameData main_state = {
    .win={.x=0,.y=0,.w=WINDOW_W,.h=WINDOW_H}
};
void raylib_start(void){
    srand(time(NULL));
    heap = malloc(HEAP_SIZE);
    assert(heap != NULL);

    game_init(&main_state);
    while(!game_update(&main_state)){}
    game_end(&main_state);

    free(heap);
    return;
}