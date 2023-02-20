

#ifndef __RANDOM_H
#define __RANDOM_H

typedef unsigned int uint;
typedef unsigned char uchar;

class Rng;
extern Rng* g_random;

class Rng
{
private:

	enum
	{
		SEED_1_MOD_FACTOR	= 30269,
		SEED_2_MOD_FACTOR	= 30307,
		SEED_3_MOD_FACTOR	= 30323,
	};

	uint seed1;
	uint seed2;
	uint seed3;

	uint seed1_init; // initial seed values
	uint seed2_init;
	uint seed3_init;
	
	void Seed(void);
	uchar GetRandomByte(uint &p_seed1, uint &p_seed2, uint &p_seed3);
	Rng(void)
	{
		Seed();
	}

public:
	static void Initialise(void);
	static void Destroy(void);

	uint Number(uint p_range);
	
	void GetInitialSeeds(uint &p_seed1, uint &p_seed2, uint &p_seed3)
	{
		p_seed1 = seed1_init;
		p_seed2 = seed2_init;
		p_seed3 = seed3_init;
	}

	void CycleRng(void)
	{
		GetRandomByte(seed1,seed2,seed3);
	}
};



#endif
