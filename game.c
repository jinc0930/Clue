#include "game.h"
#include "items.h"
#include "character.h"
#include "utils.h"
#include "pool.h"
#include <assert.h>

const char* CHARACTERS[] = {"minh","james","ivan","tenzin","edrick","chang","kevin","michael","joey"};
const char* ITEMS[] = {"butter knife","bat","wrench","rope","dagger","rifle","hammer","lead pipe","poison bottle"};
const char* ROOMS[] = {"kitchen","hall","study","bathroom","ballroom","library","lounge","office","bedroom"};
const char* SPECIAL_ITEMS[] = {"key","bread"};

struct Game makeGame() {
    struct Game game = {
        .attempts = 0,
        .map = { NULL },
        .items = { NULL },
        .characters = { NULL},
        .avatar = NULL,
        .okChar = false,
        .okItem = false,
        .okRoom = false,
        .finished = false
    };

    // INIT: ROOMS, CHARACTERS, ITEMS
    struct Pool a = makePool(N_ROOMS), b = makePool(N_ITEMS), c = makePool(N_CHARACTERS);
    for (size_t i = 0; i < N_ROOMS; i++) {
        game.map[poolTake(&a)] = makeroom(ROOMS[i]);
    }
    for (size_t i = 0; i < N_ITEMS; i++) {
        game.items[poolTake(&b)] = makeitem(ITEMS[i]);
    }
    for (size_t i = 0; i < N_CHARACTERS; i++) {
        game.characters[poolTake(&c)] = makeChar(CHARACTERS[i]);
    }
    // making sure pools are consumed
    assert(a.length == 0 && b.length == 0 && c.length == 0);

    // SET: ROOM CONNECTIONS
    for (int p = 0; p < N_ROOMS; p++ ){
        if( p > 2 ){
            setNorth(game.map[p], game.map[p-3]);
        }
        if( p < 6 ){
            setSouth(game.map[p], game.map[p+3]);
        }
        if( p != 0 && p != 3 && p != 6){
            setWest(game.map[p], game.map[p-1]);
        }
        if( p != 2 && p != 5 && p != 8){
            setEast(game.map[p], game.map[p+1]);
        }
    }

    // SET ON MAP: ITEMS
    // pool for distribution of items and chars
    struct Pool poolChar = makePool(N_CHARACTERS), poolItem = makePool(N_ITEMS);
    for (size_t i = 0; i < N_ITEMS; i++) {
        // a random item to a room
        additem(game.map[i], game.items[poolTake(&poolItem)]);
    }
    // SET ON MAP: CHARS
    for (size_t i = 0; i < N_CHARACTERS; i++) {
        // a random character to a room
        struct Character * charChosen = game.characters[poolTake(&poolChar)];
        addChar(game.map[i], charChosen);

        // add reference
        setloc(charChosen, game.map[i]);
    }
    return game;
}

// intialize everything
int initGame(struct Game * game, const char * name) {
    bool already_exists = false;
    int avatarIdx = 0;

    for (size_t i = 0; i < N_CHARACTERS; i++) {
        if (strcasecmp(name, getcharname(game->characters[i])) == 0) {
            already_exists = true;
            avatarIdx = i;
            break;
        }
    };

    if (already_exists == false) {
        avatarIdx = rand()%N_CHARACTERS;
    }

    // AVATAR
    game->avatar = game->characters[avatarIdx];
    game->avatar->name = name;
    game->avatar->id = "avatar";
    game->avatar->location->visited = true;

    // HINTS
    struct Pool poolHintsChar = makePool(N_CHARACTERS),
    poolHintsRoom = makePool(N_ROOMS),
    poolHintsItem = makePool(N_ITEMS);

    // TAKE INDEX FOR ANSWERS
    int murderIdx = poolTakeExcluding(&poolHintsChar, avatarIdx); 
    int roomIdx = poolTake(&poolHintsRoom);
    int itemIdx = poolTake(&poolHintsItem);
    game->characters[murderIdx]->id = "murderer";

    // SET TARGET ANSWERS
    game->targetChar = game->characters[murderIdx]->name;
    game->targetRoom = game->map[roomIdx]->name;
    game->targetItem = game->items[itemIdx]->name;

    // IDS DISTRIBUTION
    int excludes[] = {murderIdx, avatarIdx};
    struct Pool poolChars = makePoolExcluding(N_CHARACTERS, excludes, sizeof(excludes)/sizeof(excludes[0]));
    
    // 4 HINT GIVERS
    for (size_t i = 0; i < 4; i++) {
        int idx = poolTake(&poolChars);
        game->characters[idx]->id = "hint giver";

        // set hints
        struct Item * itemHint = game->items[poolTake(&poolHintsItem)];
        struct Room * roomHint = game->map[poolTake(&poolHintsRoom)];
        struct Character * charHint = game->characters[poolTakeExcluding(&poolHintsChar, idx)];
        set_item_hint(game->characters[idx], itemHint);
        set_room_hint(game->characters[idx], roomHint);
        set_char_hint(game->characters[idx], charHint);
    }

    // 3 ACCUSERS
    int probs[] = {0, 0, 0, 0, 5, 10, 25, 55, 80};
    bool ok_item = false;
    bool ok_char = false;
    bool ok_room = false;

    for (size_t i = 0; i < 3; i++){
        int idx = poolTake(&poolChars);
        game->characters[idx]->id = "accuser";

        struct Character * accused = NULL;
        struct Item * accused_item = NULL;
        struct Room * accused_room = NULL;

        // accuse correct item?
        if ((rand() % 100 < probs[i*3])) {
            if (!ok_item) { // no previous correct accusations
                accused_item = game->items[itemIdx];
                ok_item = true;
            } else {
                accused_item = game->items[poolTake(&poolHintsItem)];
            }
        } else {
            accused_item = game->items[poolTake(&poolHintsItem)];
        }
        // accuse correct room?
        if ((rand() % 100 < probs[i*3+1])) {
            if (!ok_room) { // no previous correct accusations
                accused_room = game->map[roomIdx];
                ok_room = true;
            } else {
                accused_room = game->map[poolTake(&poolHintsRoom)];
            }
        } else {
            accused_room = game->map[poolTake(&poolHintsRoom)];
        }

        // accuse correct char?
        if ((rand() % 100) < probs[i*3+2]) {
            if (!ok_char) { // no previous correct accusations
                accused = game->characters[murderIdx];
                ok_char = true;
            } else {
                // select a random one excluding avatar and self
                int excludes[] = {avatarIdx, idx};
                struct Pool notMe = makePoolExcluding(N_CHARACTERS, excludes, sizeof(excludes)/sizeof(excludes[0]));
                accused = game->characters[poolTake(&notMe)];
            }
        } else {
            // select a random one excluding avatar and self
            int excludes[] = {avatarIdx, idx};
            struct Pool notMe = makePoolExcluding(N_CHARACTERS, excludes, sizeof(excludes)/sizeof(excludes[0]));
            accused = game->characters[poolTake(&notMe)];
        }

        set_item_hint(game->characters[idx], accused_item);
        set_room_hint(game->characters[idx], accused_room);
        set_char_hint(game->characters[idx], accused);
    }

    // 1 MURDERER
    struct Item * itemHint = game->items[poolTake(&poolHintsItem)];
    struct Room * roomHint = game->map[poolTake(&poolHintsRoom)];
    set_item_hint(game->characters[murderIdx], itemHint);
    set_room_hint(game->characters[murderIdx], roomHint);

    int excludesMurd[] = {avatarIdx, murderIdx};
    struct Pool notMeAndAvatar = makePoolExcluding(N_CHARACTERS, excludesMurd, sizeof(excludesMurd)/sizeof(excludesMurd[0]));
    struct Character * accused = game->characters[poolTake(&notMeAndAvatar)];
    set_char_hint(game->characters[murderIdx], accused);

    return avatarIdx;
}


// helper moving the player
//-1 if is full/invalid
//-2 if is locked
// >= 0 if is ok
int move(struct Game * game, enum direction dir) {
    struct Room * current = game->avatar->location;
    struct Room * target = NULL;
    switch(dir) {
        case North:

        target = getNorth(current);
        if (target == NULL) return -1;
        else return moveChar(current, target, game->avatar);
        
        case East:

        target = getEast(current);
        if (target == NULL) return -1;
        else return moveChar(current, target, game->avatar);

        case West:

        target = getWest(current);
        if (target == NULL) return -1;
        else return moveChar(current, target, game->avatar);

        case South:

        target = getSouth(current);
        if (target == NULL) return -1;
        else return moveChar(current, target, game->avatar);

    }

    return -1;
}

// helper take an item in the room
enum actionResult take(struct Game * game, const char * itemName) {
    // is valid?
    bool invalid = true;
    for (int t=0;t<N_ITEMS;t++){
        if (strcmp(itemName, getItemName(game->items[t])) == 0) {
            invalid = false;
            break;
        };
    }
    if (invalid == true) return Invalid;

    int result = moveToInventory(game->avatar, itemName);

    if (result == 0) {
        return NotFound;
    } else if (result == -1) {
        return Full;
    } else {
        return Ok;
    }
}

// helper drop an item in the room
enum actionResult drop(struct Game * game, const char * itemName) {
    // is valid?
    bool invalid = true;
    for (int t=0;t<N_ITEMS;t++){
        if (strcmp(itemName, getItemName(game->items[t])) == 0) {
            invalid = false;
            break;
        };
    }
    if (invalid == true) return Invalid;

    int result = moveItemToRoom(game->avatar, itemName);

    if (result == 0) {
        return NotFound;
    } else {
        return Ok;
    }
}

// helper drop an item in the room
enum actionResult clue(struct Game * game, const char * murderer) {
    struct Room * currentRoom = game->avatar->location;
    struct Character * clueChar = NULL;
    // check if is valid
    for(int r = 0; r<N_CHARACTERS;r++){
        if(strcmp(game->characters[r]->name, murderer) == 0 && strcmp(game->characters[r]->id, game->avatar->id) != 0){
            clueChar = game->characters[r];
            break;
        }
    }
    if (clueChar == NULL) return Invalid;
    else if(roomCharLength(currentRoom) >= MAX_CHARACTER) return Full;
    
    // everything is valid now let's check
    // update state
    game->attempts++;
    game->okChar = false;
    game->okItem = false;
    game->okRoom = false;

    //check if room matches the target room
    if(strcmp(currentRoom->name, game->targetRoom)==0){
        game->okRoom = true;
    }

    //check if item matches (in the room or on the character)
    struct Item * inv = game->avatar->inventory;
    struct Item * roominv = currentRoom->itemList; 
    //set boolean (find item or not)
    bool temp = false;
    //loop through user inventory and see if item is there
    while(inv!=NULL){
        if(strcmp(inv->name,game->targetItem)==0){
            temp=true;
            break;
        }
        inv = inv->next;
    }
    //loop through room itemList and see if item is there
    while(roominv!=NULL){
        if(strcmp(roominv->name,game->targetItem)==0){
            temp=true;
        }
        roominv = roominv->next;
    }
    //if item is in inventory
    if(temp == true){
        game->okItem = true;
    }

    // move char
    moveChar(clueChar->location, currentRoom, clueChar);
    
    // is the target char
    if (strcmp(clueChar->name, game->targetChar) == 0) {
        game->okChar = true;
    }
    // is inside the room?
    else if (isCharInside(currentRoom, game->targetChar)) {
        game->okChar = true;
    }

    return Ok;
}

int lockRandomRoom(struct Game * game) {
    int idx = rand() % N_ROOMS;
    game->map[idx]->isLocked = true;
    return idx;
}
// used for tests
void teleport(struct Game * game, int roomIdx) {
    int map_idx = 0;
    int slot_idx = 0;
    for (size_t i = 0; i < N_CHARACTERS; i++) {
        for (size_t j = 0; j < MAX_CHARACTER; j++) {
            struct Character *slot = game->map[i]->chara[j];
            if (slot != NULL && strcmp(game->avatar->name, slot->name) == 0) {
                map_idx = i;
                slot_idx = j;
                break;
            }
        }
    }
    for (size_t i = 0; i < MAX_CHARACTER; i++) {
        if (game->map[roomIdx]->chara[i] == NULL) {
            game->map[roomIdx]->chara[i] = game->map[map_idx]->chara[slot_idx];
            game->avatar->location = game->map[roomIdx];
            break;
        }
    }
    game->map[map_idx]->chara[slot_idx] = NULL;
}

void freeGame(struct Game * game) {
    //free rooms
    for(int k =0;k<N_ROOMS;k++){
        free(game->map[k]);
    }
    //free items
    for(int l=0;l<N_ITEMS;l++){
        free(game->items[l]);
    }
    //free characters
    for(int j=0;j<N_CHARACTERS;j++){
        for (size_t i = 0; i < 3; i++) {
            free(game->characters[j]->hints[i]);
            free(game->characters[j]->prefix[i]);
        }
        free(game->characters[j]);
    }
}