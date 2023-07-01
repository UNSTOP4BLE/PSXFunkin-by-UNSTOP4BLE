/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "dad.h"

#include <stdlib.h>    
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Dad character structure
enum
{
    Dad_ArcMain_Idle0,
    Dad_ArcMain_Idle1,
    Dad_ArcMain_Left,
    Dad_ArcMain_Down,
    Dad_ArcMain_Up,
    Dad_ArcMain_Right,
    
    Dad_Arc_Max,
};

typedef struct
{
    //Character base structure
    Character character;
    
    //Render data and state
    IO_Data arc_main;
    IO_Data arc_ptr[Dad_Arc_Max];
    
    Gfx_Tex tex;
    uint8_t frame, tex_id;
} Char_Dad;

//Dad character definitions
static const CharFrame char_dad_frame[] = {
    {Dad_ArcMain_Idle0, {  0,   0, 106, 192}, { 42, 183+4}}, //0 idle 1
    {Dad_ArcMain_Idle0, {107,   0, 108, 190}, { 43, 181+4}}, //1 idle 2
    {Dad_ArcMain_Idle1, {  0,   0, 107, 190}, { 42, 181+4}}, //2 idle 3
    {Dad_ArcMain_Idle1, {108,   0, 105, 192}, { 41, 183+4}}, //3 idle 4
    
    {Dad_ArcMain_Left, {  0,   0,  93, 195}, { 40, 185+4}}, //4 left 1
    {Dad_ArcMain_Left, { 94,   0,  95, 195}, { 40, 185+4}}, //5 left 2
    
    {Dad_ArcMain_Down, {  0,   0, 118, 183}, { 43, 174+4}}, //6 down 1
    {Dad_ArcMain_Down, {119,   0, 117, 183}, { 43, 175+4}}, //7 down 2
    
    {Dad_ArcMain_Up, {  0,   0, 102, 205}, { 40, 196+4}}, //8 up 1
    {Dad_ArcMain_Up, {103,   0, 103, 203}, { 40, 194+4}}, //9 up 2
    
    {Dad_ArcMain_Right, {  0,   0, 117, 199}, { 43, 189+4}}, //10 right 1
    {Dad_ArcMain_Right, {118,   0, 114, 199}, { 42, 189+4}}, //11 right 2
};

static const Animation char_dad_anim[CharAnim_Max] = {
    {2, { 1,  2,  3,  0, ASCR_BACK, 1}}, //CharAnim_Idle
    {2, { 4,  5, ASCR_BACK, 1}},         //CharAnim_Left
    {0, {ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
    {2, { 6,  7, ASCR_BACK, 1}},         //CharAnim_Down
    {0, {ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
    {2, { 8,  9, ASCR_BACK, 1}},         //CharAnim_Up
    {0, {ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
    {2, {10, 11, ASCR_BACK, 1}},         //CharAnim_Right
    {0, {ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Dad character functions
void Char_Dad_SetFrame(void *user, uint8_t frame)
{
    Char_Dad *this = (Char_Dad*)user;
    
    //Check if this is a new frame
    if (frame != this->frame)
    {
        //Check if new art shall be loaded
        const CharFrame *cframe = &char_dad_frame[this->frame = frame];
        if (cframe->tex != this->tex_id)
            Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
    }
}

void Char_Dad_Tick(Character *character)
{
    Char_Dad *this = (Char_Dad*)character;
    
    //Perform idle dance
    if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
        Character_PerformIdle(character);
    
    //Animate and draw
    Animatable_Animate(&character->animatable, (void*)this, Char_Dad_SetFrame);
    Character_Draw(character, &this->tex, &char_dad_frame[this->frame]);
}

void Char_Dad_SetAnim(Character *character, uint8_t anim)
{
    //Set animation
    Animatable_SetAnim(&character->animatable, anim);
    Character_CheckStartSing(character);
}

void Char_Dad_Free(Character *character)
{
    Char_Dad *this = (Char_Dad*)character;
    
    //Free art
    free(this->arc_main);
}

Character *Char_Dad_New(fixed_t x, fixed_t y)
{
}