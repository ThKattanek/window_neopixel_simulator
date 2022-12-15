//////////////////////////////////////////////////
//                                              //
// SDL2 Render Threading                        //
// von Thorsten Kattanek                        //
//                                              //
// #file: pixeltest.cpp                         //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 27.11.2022                //
//                                              //
//                                              //
//////////////////////////////////////////////////

#include "effect_kordesii.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define DEAD_CELL 0x00000000
#define ALIVE_CELL 0xffffffff

EffectKordesii01::EffectKordesii01(int xw, int yw, uint32_t *buffer)
{
	this->xw = xw;
	this->yw = yw;
	this->buffer = buffer;
    led_count = xw * yw;

    privateBuffer = new uint32_t[led_count];

    srand (time(NULL));

    // AliveCell = 0xffffffff;
    // DeadCell =  0x00000000;
}

EffectKordesii01::~EffectKordesii01()
{
    delete[] privateBuffer;
}

void EffectKordesii01::Init()
{
}

void EffectKordesii01::RandomizeBuffer()
{
    for(int i=0; i<led_count; i++)
    {
        privateBuffer[i] = DEAD_CELL;
        int randomLEDIndex = rand() % led_count + 0;
        privateBuffer[randomLEDIndex] = ALIVE_CELL;
    }
    bufferWasRandomized = true;
}

void EffectKordesii01::Render()
{
    bufferWasRandomized ? CalcNextGeneration(yw, xw) : RandomizeBuffer();

    for(int i=0; i<led_count; i++)
        buffer[i] = privateBuffer[i];
}

void EffectKordesii01::CalcNextGeneration(int row, int column)
{
    uint32_t new_playfield[(row+2) * (column+2)];
    for(int i=0; i<((row+2) * (column+2)); i++ ) new_playfield[i] = DEAD_CELL;

    for(int y=1; y<row+1; y++)
        for(int x=1; x<column+1; x++)
        {
            int neighbor = GetCountNeighbor(privateBuffer,column+2,x,y);

            if(GetCellLife(privateBuffer,column+2,x,y))
            {
                /// Zelle lebt
                if(neighbor < 2) SetCellLife(new_playfield,column+2,x,y,DEAD_CELL);
                if((neighbor == 2) || (neighbor == 3)) SetCellLife(new_playfield,column+2,x,y,ALIVE_CELL);
                if(neighbor > 3) SetCellLife(new_playfield,column+2,x,y,DEAD_CELL);
            }
            else
            {
                /// Zelle ist tod
                if(neighbor == 3) SetCellLife(new_playfield,column+2,x,y,ALIVE_CELL);
                else SetCellLife(new_playfield,column+2,x,y,DEAD_CELL);
            }
        }

    for(int i=0; i<((row+2) * (column+2)); i++)
    {
        privateBuffer[i] = new_playfield[i];
    }
}

int EffectKordesii01::GetCountNeighbor(uint32_t *playfield, int pitch, int x, int y)
{
    int pos = y * pitch + x;
    int count = 0;

    if(playfield[pos - 1] == ALIVE_CELL) count++;
    if(playfield[pos + 1] == ALIVE_CELL) count++;
    if(playfield[pos + pitch] == ALIVE_CELL) count++;
    if(playfield[pos + pitch - 1] == ALIVE_CELL) count++;
    if(playfield[pos + pitch + 1] == ALIVE_CELL) count++;
    if(playfield[pos - pitch] == ALIVE_CELL) count++;
    if(playfield[pos - pitch - 1] == ALIVE_CELL) count++;
    if(playfield[pos - pitch + 1] == ALIVE_CELL) count++;

    return count;
}

void EffectKordesii01::SetCellLife(uint32_t *playfield, int pitch, int x, int y, uint32_t life)
{
    int pos = y * pitch + x;
    playfield[pos] = life;
}

bool EffectKordesii01::GetCellLife(uint32_t *playfield, int pitch, int x, int y)
{
    int pos = y * pitch + x;
    return playfield[pos];
}
