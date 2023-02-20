
#include "stdafx.h"
#include "random.h"
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>


void Rng::Initialise(void)
{
	if( g_random == 0 )
		g_random = new Rng;
}

void Rng::Destroy(void)
{
	if( g_random != 0 )
	{
		delete g_random;
		g_random = 0;
	}
}



void Rng::Seed(void)
{
	struct _timeb t;
	_ftime( &t );
	
	
	srand( (uint)time( NULL ) );

	seed1 = rand()% SEED_1_MOD_FACTOR;
	seed2 = t.millitm % SEED_2_MOD_FACTOR;
	seed3 = t.time % SEED_3_MOD_FACTOR;

	// Protect against zero seeds.
	if (seed1 == 0)
		seed1 = 1;

	if (seed2 == 0)
		seed2 = 1;

	if (seed3 == 0)
		seed3 = 1;

	//save the initial seeds
	seed1_init = seed1;
	seed2_init = seed2;
	seed3_init = seed3;
}

uchar Rng::GetRandomByte(uint &p_seed1, uint &p_seed2, uint &p_seed3)
{
	p_seed1 = ((171 * p_seed1) % SEED_1_MOD_FACTOR) & 0x0000FFFF;
	p_seed2 = ((172 * p_seed2) % SEED_2_MOD_FACTOR) & 0x0000FFFF;
	p_seed3 = ((170 * p_seed3) % SEED_3_MOD_FACTOR) & 0x0000FFFF;

	return ((p_seed1 ^ p_seed2 ^ p_seed3) & 0xff);
}

uint Rng::Number(uint p_range)
{
	int i;

	// Maximum number that will map to result.
	uint max;		
	
	// Number of bytes required
	int bytes;		

	// The working random value.
	uint outcome;	

	if (p_range <= 0x100)
	{
		max = (0x100 / p_range) * p_range;
		bytes = 1;
	}
	else if (p_range <= 0x10000)
	{
		max = (0x10000 / p_range) * p_range;
		bytes = 2;
	}
	else if (p_range <= 0x1000000)
	{
		max = (0x1000000 / p_range) * p_range;
		bytes = 3;
	}
	else
	{
		max = (0xFFFFFFFF / p_range) * p_range;
		bytes = 4;
	}

	// Keep rolling until we get a number in the correct range.
	do
	{
		outcome = 0;

		for (i = 0; i < bytes; i++)
			outcome |= GetRandomByte(seed1, seed2, seed3) << (i * 8);

	} while (outcome >= max);

	return (outcome % p_range);
}



Rng* g_random = 0;