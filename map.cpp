#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 * In this file you need to define how the map will be structured. IE how will
 * you put values into the map, pull them from the map. Remember a "Map" variable
 * is a hashtable plus two ints (see below) 
 * You will have more than one map variable, one will be the main map with it's own hashtable.
 * Then you'll have a second map with another hashtable
 * You should store objects into the hashtable with different properties (spells
 * etc)
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
//static Map map;
// make array of maps
// static Map map2;
static Map maps[2]; // make sure this works
//static Map maps[3];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    return (maps[active_map].w * Y + X);
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
    return key % 90;
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table for each map in maps
    // Set width & height
    maps[0].w = 50;
    maps[0].h = 50;
    maps[0].items = createHashTable(map_hash, 90);
    maps[1].w = 50;
    maps[1].h = 50;
    maps[1].items = createHashTable(map_hash, 90);
    //maps[2].w = 25;
//    maps[2].h = 25;
//    maps[2].items = createHashTable(map_hash, 140);
}

Map* get_active_map()
{
    // There's only one map
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m; // sets active_map to int value passed in
    return &maps[active_map];
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'D', 'N', 'B', 'G', 'M', 'F', 'V', 'T','C','R','L'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    // return map width
    return maps[active_map].w;
}

int map_height()
{
   // return map height
   return maps[active_map].h;
   
}

int map_area()
{
    return map_width() * map_height(); // area = width * height
}
// for get_direction, go into hashtable and get items
MapItem* get_north(int x, int y)
{
    int z = y - 1;
    return get_here(x,z);
}

MapItem* get_south(int x, int y)
{
    // modify variable to get pixel below
    int z = y + 1;
    return get_here(x,z);
}

MapItem* get_east(int x, int y)
{
    //modify variable to get pixel to right
    // unsigned int t = y - 1; // as long as the walls prevent you from hitting column1, we're good
    // maps[x].getItem(items, t);
    int z = x + 1;
    return get_here(z,y);
}

MapItem* get_west(int x, int y)
{
    // int z = y + 1;
    // HashTable* pop = maps[x].items;
    // maps[x].getItem(pop, y);
    // modify variable to get pixel to left
    int z = x - 1;
    return get_here(z,y);
}

MapItem* get_here(int x, int y)
{
    // HashTable* tab = maps[x].items;
    // maps[x].getItem(tab, y);
    return (MapItem*) getItem(maps[active_map].items, XY_KEY(x,y));
}


void map_erase(int x, int y)
{
    // only destroys single map item, needed if boss is going to move
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
// add hash table entries for code below (and wall sprite)
void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}
// same as above (use tree sprite)
void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_npc(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_npc;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_boss(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BOSS;
    w1->draw = draw_boss;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_gate(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = GATE;
        w1->draw = draw_gate;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_spell(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SPELL;
    w1->draw = draw_spell;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_fake(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = FAKE;
    w1->draw = draw_fake;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_dawg(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DAWG;
    w1->draw = draw_dawg;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_grave(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GRAVE;
    w1->draw = draw_grave;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

//void add_alt(int x, int y)
//{
// MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//    w1->type = ALT;
//    w1->draw = draw_alt;
//    w1->walkable = true;
//    w1->data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//    if (val) free(val);
//}

//void add_tiger(int x, int y)
//{
//   MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//    w1->type = CLEM;
//    w1->draw = draw_tiger;
//    w1->walkable = false;
//    w1->data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//    if (val) free(val);
//}
//
//void add_ctomb(int x, int y)
//{
//   MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//    w1->type = CLGR;
//    w1->draw = draw_ctomb;
//    w1->walkable = true;
//    w1->data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//    if (val) free(val);
//}
//
//void add_fan(int x, int y)
//{
//   MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//    w1->type = FAN;
//    w1->draw = draw_fan;
//    w1->walkable = false;
//    w1->data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//    if (val) free(val);
//}
//   