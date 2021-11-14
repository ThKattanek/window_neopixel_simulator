#ifndef COMETRAIN_H
#define COMETRAIN_H

#include <inttypes.h>

class Cometrain
{
public:
	Cometrain(int xw, int yw, uint32_t* buffer);
	~Cometrain();

	void Init();
	void Render();

private:
	int xw, yw, led_count;
	uint32_t *buffer;
};

#endif // COMETRAIN_H
