#include "cometrain.h"

Cometrain::Cometrain(int xw, int yw, uint32_t *buffer)
{
	this->xw = xw;
	this->yw = yw;
	this->buffer = buffer;

	led_count = xw * yw;
}

Cometrain::~Cometrain()
{

}

void Cometrain::Init()
{

}

void Cometrain::Render()
{

}
