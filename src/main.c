
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "level.h"
#include "draw.h"
#include "state.h"


int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;
    int score = 10;

    InitWindow(screen_width,screen_height,"Presente - the game");
    InitAudioDevice(); // Initialize audio device
    SetTargetFPS(60);

    Music music = LoadMusicStream("resource/The_Apex_of_the_World.ogg");
    Music music2 = LoadMusicStream("resource/God-Shattering_Star.ogg");
    Music death = LoadMusicStream("resource/Death.ogg");
    //Both songs are from Fire Emblem: Three houses and the death sound comes from OldSchoolRunescape
    PlayMusicStream(music); //We start the game with song 1

    bool pause = false;

    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);

    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);

    // Main loop
    while(!WindowShouldClose()){

        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        UpdateMusicStream(music);
        UpdateMusicStream(music2);
        UpdateMusicStream(death);

        if (IsKeyPressed(KEY_U)) //Switch from song 1 to 2
        {
            StopMusicStream(music);
            PlayMusicStream(music2);
        }
        if (IsKeyPressed(KEY_I)) // Switch from song 2 to 1
        {
            StopMusicStream(music2);
            PlayMusicStream(music);
        }
        if (IsKeyPressed(KEY_P)) //Pause song 1
        {
            pause = !pause;

            if (pause) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
        if (IsKeyPressed(KEY_O)) //Pause song 2
        {
            pause = !pause;

            if (pause) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
        if (sta->pla.ent.hp <= 0){ //The player dies, the game ends
            StopMusicStream(music);
	    StopMusicStream(music2);
            PlayMusicStream(death);
            DrawText("You are dead", 300,300,20,BLACK);
            DrawText("Press ESC to get out",300,340,20,BLACK);
            if (get_highscore() <= score){ //got new record??.
                DrawText("New record!",300,320,20,GREEN);
                change_highscore(&score);
            }
            else{
                DrawText("F",300,320,20,BLACK); //For the memes
            }
        }

        // Update the state
        state_update(lvl,sta, &score);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);

            draw_state(lvl, sta);

            DrawText("Presente profe!",190,200,20,LIGHTGRAY);
            DrawText(FormatText("Score: %08i",score),20,20,20,BLUE);
            DrawText(FormatText("Current HighScore: %08i",get_highscore()),20,40,20,RED);
            DrawText("Press U to select song 2 or I to select song 1",20,60,20,LIGHTGRAY);
            DrawText("Press P to pause song 1 or O to pause song 2",20,80,20,LIGHTGRAY);


        EndDrawing();

    }
    //Unload all music and device
    UnloadMusicStream(music); 
    UnloadMusicStream(music2);
    UnloadMusicStream(death);
    CloseAudioDevice();
    // Closer window
    CloseWindow();

    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
