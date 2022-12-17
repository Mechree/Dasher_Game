// Michael McCain
// 12/16/2022
// This is a simple game called Dasher. The character must jump to dodge the obstacles called nebulaes. 

// Libraries
#include "raylib.h"

// Structures

struct AnimData{                                                            // Animation Data
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runTime;
};

// Functions
bool isOnGround(AnimData data, int windowHeight)                            // Ground Check
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)       // Character Animation updates
{
    // update running time
    data.runTime += deltaTime;
    if (data.runTime >= data.updateTime)
    {
        data.runTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

// NOTES 
// - delta time (dT) = time since last frame: FPS up, DT dwn ; FPS dwn, DT up 

int main ()
{
    // Screen Size/Setup
    const int SCREENSZ = 2;
    int windowDimensions [SCREENSZ];
    windowDimensions [0] = 800;
    windowDimensions [1] = 600;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper-Dasher");   // Width, Height, Title window params
    SetTargetFPS(60);

    // Character Movement - accelerate due to grav (pixels/sec/ sec)
    const int gravity {1'000};
    int velocity {0};
    const int jumpV {-600}; // pixels per sec

    // Load Background Image
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX {};
    
    // Load Midground Image
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    // Load Foreground Image
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};


    // Character Textures
    Texture2D character = LoadTexture("textures/scarfy.png");                  // load sprite sheet;
    AnimData charData;
    charData.rec.width = character.width/6;
    charData.rec.height = character.height;
    charData.rec.x = 0;
    charData.rec.y = 0;
    charData.pos.x = windowDimensions[0]/2 - charData.rec.width/2;
    charData.pos.y = windowDimensions[1] - charData.rec.height;
    charData.frame = 0;
    charData.updateTime = 1.0/12.0;
    charData.runTime = 0.0;

    // --Hazard Textures--

    // Nebula
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // Arrays for Nebulas w/ AnimData
    const int NEBSZ = 3;                                                    // Number of generated nebulaes

    AnimData nebulae[NEBSZ]{};
    for (int i = 0; i < NEBSZ; i ++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/ 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runTime = 0.0;
        nebulae[i].updateTime = 0.0;
    }

   

    for (int i = 0; i < NEBSZ; i++)
    {
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }

     float finishLine { nebulae[NEBSZ - 1].pos.x};

    // Nebula animation variables pixels/sec
    int nebVel{-200};                                                       // Nebula velocity

    // Collision detection variable
    bool collision {};

// --Display Game Window--
    while (!WindowShouldClose())
    {

        // delta time
        const float dT {GetFrameTime()};

        // Setup, teardown, and clear canvas
        BeginDrawing();
        ClearBackground(BLACK);                                             // background Color
        bgX -= 20 * dT;
        mgX -= 40 * dT;
        fgX -= 80* dT; 

        // Scroll background
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        // Scroll midground
        if (mgX <= - midground.width * 2)
        {
            mgX = 0.0;
        }
        // Scroll foreground
        if (fgX <= - foreground.width * 2)
        {
            fgX = 0.0;
        }

        // Draw Background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos {bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.2, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 3.2, WHITE);
        
        // Draw midground
        Vector2 mg1Pos {mgX, 0.0};
        Vector2 mg2Pos {mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.2, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 3.2, WHITE);

        // Draw foreground
        Vector2 fg1Pos {fgX, 0.0};
        Vector2 fg2Pos {fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.2, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.2, WHITE);

        // Game Logic Begins
        // Perform ground check
        bool rIsInAir {false};

        // Jump Velocity & ground check
        if (isOnGround(charData, windowDimensions[1]))
        {
            velocity = 0;
            rIsInAir = false;
        }
        else 
        {
            // apply gravity
            velocity += gravity * dT;
            rIsInAir = true;
        }

        // Keys
        if (IsKeyDown(KEY_SPACE) && !rIsInAir)
        {
            velocity += jumpV;
        }

        // Nebulae Positions
        for (int i = 0; i < NEBSZ; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        // Update Finish
        finishLine += nebVel * dT;

        // character position
        charData.pos.y += velocity * dT;

        // update character animation frame
        if (!rIsInAir)
        {
           charData = updateAnimData(charData, dT, 5);
        }
        
        // Update nebulae animation frames
        for (int i = 0; i < NEBSZ; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for (AnimData nebula : nebulae)
        {
            float pad { 43 };
            Rectangle nebRec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };
            Rectangle characterRec {
                charData.pos.x,
                charData.pos.y,
                charData.rec.height,
                charData.rec.width
            };
            if (CheckCollisionRecs(nebRec, characterRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/3, windowDimensions[1]/2 , 65, RED);
                bgX = 0;
                mgX = 0;
                fgX = 0;
        }
        else if (charData.pos.x >= finishLine) 
        {
            DrawText("You Win!", windowDimensions[0]/3, windowDimensions[1]/2, 65, GREEN);
        }
        else 
        {
            // Draw hazards
            for (int i = 0; i < NEBSZ; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, BLUE);
            }
            // Assign textures to character rectangle and position
            DrawTextureRec (character, charData.rec, charData.pos, WHITE);   
        }
        // ---Game Logic Ends---

        // Stop Drawing
        EndDrawing();

        

    }
    // --Unload textures--
    UnloadTexture(character);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    // --End Program--
    CloseWindow();
}

