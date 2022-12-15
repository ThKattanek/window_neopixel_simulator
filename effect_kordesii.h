//////////////////////////////////////////////////
//                                              //
// SDL2 Render Threading                        //
// von Thorsten Kattanek                        //
//                                              //
// #file: pixeltest.h                           //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 27.11.2022                //
//                                              //
//                                              //
//////////////////////////////////////////////////

#ifndef EFFECT_KORDESII01_H
#define EFFECT_KORDESII01_H


#include <cstdint>
class EffectKordesii01
{
public:
    EffectKordesii01(int xw, int yw, uint32_t *buffer);
    ~EffectKordesii01();

	void Init();
	void Render();

private:
	int xw, yw, led_count;
	uint32_t *buffer;
    uint32_t *privateBuffer;
    bool bufferWasRandomized = false;

    void RandomizeBuffer();

    void CalcNextGeneration(int row, int column);
    int GetCountNeighbor(uint32_t *playfield, int pitch, int x, int y);
    void SetCellLife(uint32_t *playfield, int pitch, int x, int y, uint32_t life);
    bool GetCellLife(uint32_t *playfield, int pitch, int x, int y);
};

#endif // EFFECT_KORDESII01_H
