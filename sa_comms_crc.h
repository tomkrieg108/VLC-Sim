/***************************************************************************************************
 *									Copyright (C) 2002, IGT
 ***************************************************************************************************
 * Filename:		sa_comms_crc.h
 *
 * Documentation:	-
 *
 * Description:		Crc classes for VLC.
 *
 *--------------------------------------------------------------------------------------------------
 * $Log: /Version 3/Source/network_sa_vlc/network_sa_vlc/sa_comms_crc.h $
 * 
 * 1     14/02/05 4:33p Martinv
 * Copied from Version 2.
 *--------------------------------------------------------------------------------------------------
 */

#ifndef _sa_comms_crc_h_
#define _sa_comms_crc_h_

#include "stdtype.h"


/*------------------------------------------------------------------------------------------------*/
/* Definitions.																					  */
/*------------------------------------------------------------------------------------------------*/

/***************************************************************************************************
 * SaCrcTable
 *
 * A dynamic CRC table, using the standard poly 0x1021, or any other polynomial.
 *
 */

class SaCrcTable
{
	// Construction.

	public:
		SaCrcTable(void)
		:	poly(0x1021)
		{
			GenerateTable();
		}

		~SaCrcTable(void) { }

	// Functions.
		
	public:
		void Calculate(ushort p)
		{
			if (p != poly)
			{
				poly = p;
				GenerateTable();
			}
		}

		ushort& operator[](uint index) { return table[index]; }

	private:
		void GenerateTable(void)
		{
			ushort crc;
			for (ushort i = 0; i < 256; i++)
			{
				crc = i << 8;
				for (ushort j = 0; j < 8; j++)
				{
					if (crc & 0x8000)
					{
						crc <<= 1;
						crc = crc ^ poly;
					}
					else
					{
						crc <<= 1;
					}
				}

				table[i] = crc;
			}
		}
	
	// Variables.

	private:
		ushort poly;		// The polynomial.
		ushort table[256];	// The generated crc table.
};

/***************************************************************************************************
 * SaCrc
 *
 * Handles the communication packet CRC processing.
 *
 */

class SaCrc
{
	// Construction.	
	
	public:


		SaCrc(ushort seed = 0) : value(seed) {};
		~SaCrc() {}
	
	// Functions.

	public:	
		inline void Seed(ushort seed) { value = seed; }
		inline void Byte(uchar byte) { value = (table[byte ^ (value >> 8)] ^ (value << 8)); }
		inline void Block(uchar *mem, uint length) { while (length--) Byte(*mem++); }
		inline ushort Value(void) { return value; }
	
	// Variables.

	private:
		SaCrcTable table;	// The generated crc table.
		ushort value;		// The current crc value.
};



#endif

