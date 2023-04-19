// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();
int map_count = 0;

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    // int cast_spell;
    // You can add other properties for the player here
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define INV_BUTTON 7
int get_action(GameInputs inputs)
{
    //pc.printf("%0.3f, ", inputs.ax);
    //pc.printf("%0.3f, ", inputs.ay);
    //pc.printf("%0.3f\n", inputs.az);
    //pc.printf("run\n");
    if (inputs.b1 == 0) {
        return ACTION_BUTTON;
    } else if (inputs.b2 == 0) {
        return MENU_BUTTON;
    } else if (inputs.b3 == 0) {
        return INV_BUTTON;
    }
    if (inputs.ax != 0) {
        if (inputs.ax < - 0.3f) {
            //pc.printf("left");
            return GO_LEFT;
        } else if (inputs.ax > 0.3f) {
            //pc.printf("right");
            return GO_RIGHT;
        }
    }
    if (inputs.ay != 0) {
        if (inputs.ay > 0.3f) {
            //pc.printf("up");
            return GO_UP;
        } else if (inputs.ay < -0.3f) {
            //pc.printf("down");
            return GO_DOWN;
        }
    }
    return NO_ACTION;
}
// forward declarations
void kill_boss();
void kill_tiger();
void portal_spawn();
//void alt_spawn();
void grave_spawn(int x, int y);
void tomb_spawn();
//void animation();
//void boss_move(int x, int y); 
//void winner();
// potential needed ints
int portal_travel = 0;
int encounter = 0;
int fight = 0;
int inv_mode = 0;
int lives = 3;
int boss_health = 3;
int foes = 5;
//int tbar = 5;
int tutorial = 0;
int talk = 0;
//int boss_spawn = 0;
//int boss_px = 24;
//int boss_dir = 0;
/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define VICTORY   3
//#define DEFEAT    4
//#define WIN_2     5
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    MapItem* move;
    uLCD.locate(0, 0);
    uLCD.printf("%3d, %3d, %3d", Player.x, Player.y, lives); 
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        // For each case, stop if you hit a wall, door, NPC, or boss
        case GO_UP: {    
                    // if tile is walkable, move character down(boolean)
                    // if tile is not walkable, do nothing
                    // walkable is an int in graphics?
                    // get_north(map option, coordinate);
                    if (inv_mode == 1) {
                        Player.y--;
                    } else {
                        move = get_north(Player.x, Player.y);
                        if ((bool) move->walkable == 1){
                            Player.y --;
                            pc.printf("%d\n", Player.y);
                        }
                    }
                    break;}
        // rest of GO_DIRECTION are same
        case GO_LEFT:   {
                        move = get_west(Player.x, Player.y);
                        if (inv_mode == 1) {
                            Player.x--;
                        } else {
                            if ((bool) move->walkable == 1) {
                                Player.x --;
                                pc.printf("%d\n", Player.x);
                            }
                        }
                        break;}            
        case GO_DOWN:   {
                        move = get_south(Player.x, Player.y);
                        if (inv_mode == 1) {
                            Player.y++;
                        } else {
                            if ((bool) move->walkable == 1) {
                                Player.y ++;
                                pc.printf("%d\n", Player.y);
                            }
                        }
                        break;}
        case GO_RIGHT: { 
                        move = get_east(Player.x, Player.y);
                        bool t = (bool) move->walkable;
                        pc.printf("%d\n", t);
                        if (inv_mode == 1) {
                            Player.x++;
                        } else {
                            if (t == 1) {
                                Player.x ++;
                                pc.printf("%d\n", Player.x);
                            }
                        }
                        break;}
        // Triggers various actions depending on where you are
        case ACTION_BUTTON: { 
                    // trigger interaction, add case INV_MODE
                    pc.printf("press a\n");
                    MapItem* nor = get_north(Player.x, Player.y);
                    MapItem* sou = get_south(Player.x, Player.y);
                    MapItem* eas = get_east(Player.x, Player.y);
                    MapItem* wes = get_west(Player.x, Player.y);
                    MapItem* her = get_here(Player.x, Player.y);
                    //pc.printf("%d, %d, %d, %d, %d\n",nor->type, sou->type, eas->type, wes->type, her->type);
                    //pc.printf("%d\n",eas->walkable);
                    if (sou->type == 5) {
                        pc.printf("gate hit\n");
                        if (Player.has_key == 1) {
                            pc.printf("key\n");
                            // don't know what to do now
                            //winner();
                            return VICTORY;
                        }
                        pc.printf("no key\n");
                        speech("No key...", "... no entry.");
                    }
                    if ((nor->type == 3) || (sou->type == 3) || (wes->type == 3) || (eas->type == 3)) {
                        pc.printf("npc\n");
                        //animation();
                        if (portal_travel == 0 && encounter == 0) {
                            pc.printf("scenario 1\n");
                            speech("Go kill u(sic)ga", "& you get a key!");
                            portal_spawn();
                            encounter++;
                        } else if (portal_travel >= 1 && Player.has_key == 0) {
                            pc.printf("scenario 2\n");
                            speech("You did it!", "Here's the key!");
                            Player.has_key = 1;
                            pc.printf("Key obtained!\n");
                        } else if (portal_travel == 0) {
                            pc.printf("scenario 1.5\n");
                            speech("Find the portal!", "Go kill u(sic)ga!");
                        } else if (Player.has_key == 1) {
                            pc.printf("scenario 2.5\n");
                            speech("Unlock the gate &", "your journey ends.");
                        }
                    }
                    if (her->type == 2) {
                        pc.printf("portal\n");
                        pc.printf("%d\n", map_count);
                        pc.printf("Changing maps\n");
                        if (map_count == 0) {
                            map_count = 1;
                            set_active_map(1);
                        } else if (map_count == 1) {
                            map_count = 0;
                            set_active_map(0);
                        } pc.printf("Map changed!\n");
                        portal_travel++;
                    }
                    if ((nor->type == 7) || (her->type == 7) || (sou->type == 7) || (wes->type == 7) || (eas->type == 7) && (boss_health != 0)) {
                        pc.printf("life lost\n");
                        lives--;
                        if (lives == 0) {
                            pc.printf("halt\n");
                            speech("Wow. Just wow.", "You didn't listen.");
                            speech("I can't even.. WOW", "You are dumb.");
                            speech("You will not be", "returning to the");
                            speech("Georgia Institute", "of Technology");
                            speech("next semester.", "You are worse than");
                            speech("the 2021 football", "team. Goodbye.");
                            //orator("Wow. Just wow.   You didn't listen."); //check this
                            return GAME_OVER;
                        }
                        speech("Wrong Spell!", "Try again.");
                        speech_adv("Lives left: ", lives);
                    }
                    if ((nor->type == 6) || (her->type == 6) || (sou->type == 6) || (wes->type == 6) || (eas->type == 6)) {
                        pc.printf("spell\n");
                        if (fight == 0) {
                            boss_health--;
                            if (boss_health == 0) {
                                speech("Water Spell!", "u(sic)ga slain!");
                                kill_boss();
                                fight++;
                                foes--;
                                if (foes == 0) {
                                    speech("All enemies dead!", "Get back to Sims!");
                                    portal_spawn();
                                }
                            } else if (boss_health != 0) {
                                speech("Water Spell!", "Keep it up!");
                                if (boss_health == 1) {
                                    speech("QUIT IT NOW!", "WE RUN THIS STATE!");
                                    lives = lives - 2;
                                    Player.x = Player.x - 2;
                                    Player.y = Player.y + 2;
                                    if (lives <= 0) {
                                        speech("WE RUN THIS STATE!", "WE RUN THIS STATE!");
                                        return GAME_OVER;
                                    }
                                }
                            }
                        } else {
                            speech("Find the portal", " & find Sims.");
                        //Player.has_key = 1;
                        }
                    }
                    if ((nor->type == 8) || (her->type == 8) || (sou->type == 8) || (wes->type == 8) || (eas->type == 8)) {
                        pc.printf("dawg maulin'\n");
                        lives--;
                        if (lives == 0) {
                            pc.printf("dawg call\n");
                            speech("GOOOOO DAWGS!", "SIC 'EM!");
                            speech("WOOF WOOF WOOF", "WOOF WOOF WOOF!");
                            return GAME_OVER;
                        }
                        speech("The dawgs bite!", "Hit 'em!");
                        speech_adv("Lives left: ", lives); 
                    }
                    //if ((nor->type == 11) || (sou->type == 11) || (wes->type == 11) || (eas->type == 11)) {
//                        speech("Shoo little bee, ", "I rule the ACC.");
//                        lives--;
//                        speech_adv("Lives left: ", lives);
//                        if (lives == 0) {
//                            speech("Are you sure you", "belong here?");
//                            return DEFEAT;
//                        }
//                        speech("Don't talk to him!", "Sting him!");
//                    }
                    //if ((nor->type == 13) || (sou->type == 13) || (wes->type == 13) || (eas->type == 13)) {
//                        if (tutorial == 0 && talk == 0) {
//                            pc.printf("clem 1\n");
//                            speech("Defeat Clemson &", "you enter the ...");
//                            speech("Georgia Tech Hall", "of Fame.");
//                            portal_spawn();
//                            talk++;
//                        } else if (tutorial >= 1 && tbar <= 0) {
//                            pc.printf("scenario 2\n");
//                            speech("Is this real?", "You really did it?");
//                            speech("Well, good job.", "You deserve this.");
//                            return WIN_2;
                    //    } else if (tutorial == 0) {
//                            pc.printf("scenario 1.5\n");
//                            speech("Enter my portal &", "sting the tiger.");
//                        }
//                    }
                    break; }
        case INV_BUTTON:  {
                    pc.printf("press i\n");
                    //MapItem* nor = get_north(Player.x, Player.y);
//                    MapItem* sou = get_south(Player.x, Player.y);
//                    MapItem* eas = get_east(Player.x, Player.y);
//                    MapItem* wes = get_west(Player.x, Player.y);
//                    MapItem* her = get_here(Player.x, Player.y);
//                    pc.printf("%d\n",eas->walkable);
                    if (inv_mode == 0) {
                        inv_mode = 1;
                    } else {
                        inv_mode = 0;
                    }
                    // method to make all walkable = 0, last attempt killed program; 
                    break;}
        case MENU_BUTTON: {
                    pc.printf("press m\n");
                    // repurposed for stinger due to complicatedness of menu requirements
                    MapItem* nor = get_north(Player.x, Player.y);
                    MapItem* sou = get_south(Player.x, Player.y);
                    MapItem* eas = get_east(Player.x, Player.y);
                    MapItem* wes = get_west(Player.x, Player.y);
                    int kill = 0;
                    if (nor->type == 8) {
                        pc.printf("sting\n");
                        speech("GOOOOO JACKETS!", "STING 'EM! BZZZ!");
                        kill = Player.y - 1;
                        grave_spawn(Player.x, kill);
                        foes--;
                    } else if (sou->type == 8) {
                        pc.printf("sting\n");
                        speech("GOOOOO JACKETS!", "STING 'EM! BZZZ!");
                        kill = Player.y + 1;
                        grave_spawn(Player.x, kill);
                        foes--;
                    } else if (eas->type == 8) {
                        pc.printf("sting\n");
                        speech("GOOOOO JACKETS!", "STING 'EM! BZZZ!");
                        kill = Player.x + 1;
                        grave_spawn(kill, Player.y);
                        foes--;
                    } else if (wes->type == 8) {
                        pc.printf("sting\n");
                        speech("GOOOOO JACKETS!", "STING 'EM! BZZZ!");
                        kill = Player.x - 1;
                        grave_spawn(kill, Player.y);
                        foes--;
                    }  else if ((nor->type == 4) || (sou->type == 4) || (wes->type == 4) || (eas->type == 4)) {
                        pc.printf("stung boss\n");
                        speech("u(sic)ga stung!", "He's not happy...");
                        boss_health--;
                        speech("I EAT JACKETS FOR", "BREAKFAST!!");
                        Player.x = Player.x - 10;
                        Player.y = Player.y - 10;
                        lives = lives - 2;
                        if (lives <= 0) {
                            speech("I told you he", "wasn't happy.");
                            return GAME_OVER;
                        }
                    //} else if ((nor->type == 11) || (sou->type == 11) || (wes->type == 11) || (eas->type == 11)) {
//                        pc.printf("stung tiger\n");
//                        speech("CLEMSON STUNG!", "Keep it up!");
//                        tbar = tbar - 2;
//                        if (tbar <= 0) {
//                            speech("Clemson defeated!", "Return to fan!");
//                            portal_spawn();
//                            kill_tiger();
//                            lives = 3;
//                            tutorial++;
                        //} else {
//                            speech("ROOOOAAR!", "STAY BACK!");
//                            lives--;
//                            speech_adv("Lives left: ", lives);
//                            if (lives == 0) {
//                                speech("C-L-E-M-S-O-N", "TIGERRRS!");
//                                return DEFEAT;
//                            }
                    }
                    if (foes == 0) {
                        speech("All enemies dead.", "Enter the portal.");
                        portal_spawn();
                    }
                    break;}
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    
   // add_door(25, 25);
//    pc.printf("portal\r\n");
    
    add_npc(10, 10);
    pc.printf("jeff sims\r\n");
    
    //add_fan(7, 10);
    //pc.printf("faithful\r\n");
        
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

    pc.printf("Adding gate.\r\n");
    add_gate(22, 30, HORIZONTAL, 5);
    pc.printf("Gate done!\r\n");
    
    pc.printf("Adding barricade.\r\n");
    add_wall(21, 30, VERTICAL, 5);
    add_wall(21, 35, HORIZONTAL, 7);
    add_wall(27, 30, VERTICAL, 5);
    pc.printf("Barricade done!\r\n");
    
    //add_spell(9,9);
//    add_fake(11,11);
//    pc.printf("spell & fake test|r|n");
    
    print_map();
}

void init_alt_map()
{
    Map* map = set_active_map(1);
    add_boss(24, 24);
    pc.printf("u(sic)ga\r\n");
    
    add_dawg(24, 21);
    add_dawg(24, 27);
    add_dawg(27, 24);
    add_dawg(21, 24);
    pc.printf("dawgs\r\n");
    
    add_spell(21, 28);
    add_fake(27, 28);
    pc.printf("spells\r\n");
    
    //add_tiger(7, 6);
    //pc.printf("tiger\r\n");
    
    //add_plant(2, 2);
//    add_plant(2, 4);
//    add_plant(6, 2);
//    add_plant(6, 4);
//    pc.printf("plants\r\n");
    
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
     
    
    print_map();
}

void kill_boss()
{
    pc.printf("Boss hit\r\n");
    add_grave(24,24);
    pc.printf("Boss dead\r\n");
}

void portal_spawn()
{
    add_door(25, 25);
    pc.printf("portal\r\n");
}

//void alt_spawn()
//{
//    add_alt(4, 4);
//    pc.printf("clemson portal\r\n");
//}

void grave_spawn(int x, int y)
{
    add_grave(x, y);
    pc.printf("dawg slain\r\n");
}

//void kill_tiger()
//{
//    add_ctomb(7,6);
//    pc.printf("Tiger dead\r\n");
//}
//void animation()
//{
//    add_ani(9, 9);
//    pc.printf("animation\r\n");
//}
//void boss_move(int x)
//{
//    if (boss_spawn == 0) {
//        boss_spawn++;
//        add_boss(boss_px - 1, 24);
//        map_erase(boss_px, 24);
//        boss_px--;
//        boss_dir--;
//        boss_move(
//   } else if ((boss_dir < ) && (fight == 0)) {
//        x
//    } else if ((boss_dir > ) && (fight == 0)) {
//        x
//    } else if (fight !=0) {
//        add_boss(24, 24);
//        map_erase(boss_px, 24);
//        speech("But, but...", "we run this state.");
//    }   
//}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // uLCD.filled_rectangle(64, 64, 74, 74, RED); //DELETE OR COMMENT THIS LINE
    pc.printf("start\r\n");
    // Start screen
    uLCD.locate(7, 5);
    uLCD.set_font_size(10, 10);
    uLCD.printf("GT");
    uLCD.locate(5, 6);
    uLCD.printf("QUEST");
    uLCD.locate(5, 7);
    uLCD.printf("MMXXI");
    wait_ms(5000);
    uLCD.locate(0,0);
    uLCD.set_font_size(1, 1);
    //uLCD.printf("test");
    // Initialize the maps
    maps_init();
    init_main_map();
    init_alt_map();
    pc.printf("maps made\r\n");
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = 0;

    // Initial drawing
    draw_game(true);
    // Main game loop
    Timer race; race.start();
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        pc.printf("%.2f\n", race.read());
        uLCD.locate(14, 0);
        uLCD.printf("%.1f", race.read());
        if (race.read() >= 250) {
            race.stop();
            pc.printf("killed by dawgs");
            uLCD.cls();
            uLCD.locate(0, 0);
            uLCD.printf("Game Over. georgiawins again.");
            break;
        }
        // Actually do the game update:
        // 1. Read inputs
        GameInputs input = read_inputs();        
        // 2. Determine action (get_action) 
        int stuff = get_action(input);       
        // 3. Update game (update_game)
        //update_game(stuff);
        // 3b. Check for game over
        int upd = update_game(stuff);
        if (upd == 1) {
            race.stop();
            pc.printf("GAME OVER");
            uLCD.background_color(0xe4002b);
            uLCD.cls();
            uLCD.locate(0, 0);
            uLCD.printf("Game Over. WOOF!");
            break;
        } else if (upd == 3) {
            race.stop();
            pc.printf("VICTORY");
            uLCD.background_color(0xeaaa00);
            uLCD.cls();
            uLCD.locate(0, 0);
            uLCD.printf("Battle won!  THWg!GO JACKETS!");
            break;
        } //else if (upd == 4) {
//            race.stop();
//            pc.printf("killed by dawgs");
//            uLCD.cls();
//            uLCD.locate(0, 0);
//            uLCD.printf("Game Over. georgiawins again.");
//            break;
        //} //else if (upd == 5) {
//            pc.printf("WIN_2");
//            uLCD.background_color(0xeaaa00);
//            uLCD.cls();
//            uLCD.locate(0, 0);
//            uLCD.printf("Congratulations.  Welcome to the GT Hall of Fame.     GO TECH!");
//            break;
//        }
        // 4. Draw frame (draw_game)
        draw_game(true);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    } 
}
