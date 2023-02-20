

#include "stdafx.h"
#include "utilities.h"






int StringToBin(CString input)
{
	int output;
	
	switch (input.GetAt(0))
	{

		case '0':
			output = 0;
			break;

		case '1':
			output = 1;
			break;

		case '2':
			output = 2;
			break;

		case '3':
			output = 3;
			break;

		case '4':
			output = 4;
			break;

		case '5':
			output = 5;
			break;

		case '6':
			output = 6;
			break;

		case '7':
			output = 7;
			break;

		case '8':
			output = 8;
			break;

		case '9':
			output = 9;
			break;
	}
	return (output);

}

CString HexToString(int input)
{
	CString output;
	if (input < 10)
	{
		output.Format("%d", input);
	}
	else 
	{
		switch (input)
		{
			
		case 10:
			output = 'A';
			break;
			
		case 11:
			output = 'B';
			break;
			
		case 12:
			output = 'C';
			break;
			
		case 13:
			output = 'D';
			break;
			
		case 14:
			output = 'E';
			break;
			
		case 15:
			output = 'F';
			break;
		}
	}
	
	return (output);
}

int StringToHex(CString input)
{
	int output;
	
	switch (input.GetAt(0))
	{

		case '0':
			output = 0;
			break;

		case '1':
			output = 1;
			break;

		case '2':
			output = 2;
			break;

		case '3':
			output = 3;
			break;

		case '4':
			output = 4;
			break;

		case '5':
			output = 5;
			break;

		case '6':
			output = 6;
			break;

		case '7':
			output = 7;
			break;

		case '8':
			output = 8;
			break;

		case '9':
			output = 9;
			break;
	
		case 'a':
			output = 10;
			break;

		case 'b':
			output = 11;
			break;

		case 'c':
			output = 12;
			break;

		case 'd':
			output = 13;
			break;

		case 'e':
			output = 14;
			break;

		case 'f':
			output = 15;
			break;

		case 'A':
			output = 10;
			break;

		case 'B':
			output = 11;
			break;

		case 'C':
			output = 12;
			break;

		case 'D':
			output = 13;
			break;

		case 'E':
			output = 14;
			break;

		case 'F':
			output = 15;
			break;
	
	}
	return (output);
}


byte CharToHex(char input)
{
	byte output;
	
	switch (input)
	{

		case '0':
			output = 0;
			break;

		case '1':
			output = 1;
			break;

		case '2':
			output = 2;
			break;

		case '3':
			output = 3;
			break;

		case '4':
			output = 4;
			break;

		case '5':
			output = 5;
			break;

		case '6':
			output = 6;
			break;

		case '7':
			output = 7;
			break;

		case '8':
			output = 8;
			break;

		case '9':
			output = 9;
			break;
	
		case 'a':
			output = 10;
			break;

		case 'b':
			output = 11;
			break;

		case 'c':
			output = 12;
			break;

		case 'd':
			output = 13;
			break;

		case 'e':
			output = 14;
			break;

		case 'f':
			output = 15;
			break;

		case 'A':
			output = 10;
			break;

		case 'B':
			output = 11;
			break;

		case 'C':
			output = 12;
			break;

		case 'D':
			output = 13;
			break;

		case 'E':
			output = 14;
			break;

		case 'F':
			output = 15;
			break;
	
	}
	return (output);
}


char HexToChar(byte p_input)
{
	char output;
	
	switch (p_input)
	{

		case 0:
			output = '0';
			break;

		case 1:
			output = '1';
			break;

		case 2:
			output = '2';
			break;

		case 3:
			output = '3';
			break;

		case 4:
			output = '4';
			break;

		case 5:
			output = '5';
			break;

		case 6:
			output = '6';
			break;

		case 7:
			output = '7';
			break;

		case 8:
			output = '8';
			break;

		case 9:
			output = '9';
			break;
	
		case 10:
			output = 'A';
			break;

		case 11:
			output = 'B';
			break;

		case 12:
			output = 'C';
			break;

		case 13:
			output = 'D';
			break;

		case 14:
			output = 'E';
			break;

		case 15:
			output = 'F';
			break;
	}
	return (output);
}

CString HexToString(byte p_input)
{
	CString output;
	
	switch (p_input)
	{

		case 0:
			output = "0";
			break;

		case 1:
			output = "1";
			break;

		case 2:
			output = "2";
			break;

		case 3:
			output = "3";
			break;

		case 4:
			output = "4";
			break;

		case 5:
			output = "5";
			break;

		case 6:
			output = "6";
			break;

		case 7:
			output = "7";
			break;

		case 8:
			output = "8";
			break;

		case 9:
			output = "9";
			break;
	
		case 10:
			output = "A";
			break;

		case 11:
			output = "B";
			break;

		case 12:
			output = "C";
			break;

		case 13:
			output = "D";
			break;

		case 14:
			output = "E";
			break;

		case 15:
			output = "F";
			break;
	}
	return (output);
}



int BinToHex(int input)
{
	int output = 16*((input - (input%10))/10) + (input%10); // Turn a binary number to hex :)
	return(output);
}


CString ByteToString(BYTE input)
{
	CString theByte, Sum1, Sum2;
	int xsum1, xsum2;

	xsum1 = input % 16;						// if checksum = 255 xsum1 = 15
	Sum1 = HexToString(xsum1);			// = F
	input -= (input %16);					// = 240
	xsum2 = input;							// = 240
	Sum2 = HexToString(xsum2/16);		// = F
	
	theByte = Sum2+Sum1;

	return(theByte);
}


CString ReverseASCIIBytes(CString input)
{

	CString output = input;
	int l = input.GetLength()-1;

	for (int i=0; i <= l; i++){

		output.SetAt(i , input.GetAt(l-i));
	}
	
	return (output);


}



int Convert(const CString& p_val)
{
	CString str = p_val;
	int val = atoi( str.GetBuffer( str.GetLength() ) );
	return val;
}

CString Convert(const int p_val)
{
	char buff[11]; //max int will be 10 digits

	_itoa(p_val, buff, 10); //base 10 !

	CString op(buff);

	return op;
}


CString Convert(const uint p_val)
{
	char buff[11]; //max int will be 10 digits

	_itoa(p_val, buff, 10); //base 10 !

	CString op(buff);

	return op;
}



CString Convert(const float p_val)
{
	return Convert( int(p_val) );
}

BYTE BitReverse(BYTE p_input)
{
	BYTE result=0;

	result |= ((p_input&0x01)<<7);
	result |= ((p_input&0x02)<<5);
	result |= ((p_input&0x04)<<3);
	result |= ((p_input&0x08)<<1);
	result |= ((p_input&0x10)>>1);
	result |= ((p_input&0x20)>>3);
	result |= ((p_input&0x40)>>5);
	result |= ((p_input&0x80)>>7);

	return result;
}



uint SwapBytes(uint p_input)
{
	uint val=0;

	val = ((p_input<<24)&(0xF000));
	val |= ((p_input<<8)&(0x0F00));
	val |= ((p_input>>8)&(0x00F0));
	val |= ((p_input>>24)&(0x000F));

	return val;
}


ushort SwapBytes(ushort p_input)
{
	ushort val=0;
	val = ((p_input<<8)&(0xF0));
	val = ((p_input>>8)&(0x0F));
	return val;
}


int GetUniversalTime()
{
	COleDateTime cur_time = COleDateTime::GetCurrentTime();
	
	int universal_time = cur_time.GetSecond();
	universal_time += cur_time.GetMinute()*60;
	universal_time += cur_time.GetHour()*60*60;
	universal_time /= 2;
	return universal_time;
}


int GetJulianDate()
{
	COleDateTime start_date, julian_date;

	start_date.SetDateTime(1900,3,1,0,0,0);
	julian_date = COleDateTime::GetCurrentTime() - start_date;
	return (int)(julian_date.m_dt)+1;
}






bool IsDec(char p_char)
{
	if(  ((uchar)(p_char)>= 48) && ((uchar)(p_char)<= 57) ) //0 to 9
		return true;
	else
		return false;
}

bool IsHex(char p_char)
{
	if(  ((uchar)(p_char)>= 48) && ((uchar)(p_char)<= 57) ) //0 to 9
		return true;
	if(  ((uchar)(p_char)>= 65) && ((uchar)(p_char)<= 70) ) //A to F
		return true;
	if(  ((uchar)(p_char)>= 97) && ((uchar)(p_char)<= 102) ) //a to f
		return true;
	return false;
}

bool IsBin(char p_char)
{
	if(  ((uchar)(p_char)>= 48) && ((uchar)(p_char)<= 49) ) //0 to 1
		return true;
	return false;
}


bool IsDec(const CString& p_val)
{
	if(p_val.IsEmpty())
		return false;
	for(int i=0; i<p_val.GetLength(); i++)
	{
		char ch = p_val[i];
		if( !IsDec(ch) )
			return false;
	}
	return true;
}

bool IsHex(const CString& p_val)
{
	if(p_val.IsEmpty())
		return false;
	for(int i=0; i<p_val.GetLength(); i++)
	{
		char ch = p_val[i];
		if( !IsHex(ch) )
			return false;
	}
	return true;
}

bool IsBin(const CString& p_val)
{
	if(p_val.IsEmpty())
		return false;
	for(int i=0; i<p_val.GetLength(); i++)
	{
		char ch = p_val[i];
		if( !IsBin(ch) )
			return false;
	}
	return true;
}

int ConvertHexStringToInt(const CString& p_val)
{
	int val=0;
	int length = p_val.GetLength();
	if(length>8)
		return 0;

	int multiplier=1;
	for(int i=1; i<=length; i++)
	{
		char ch = p_val[length-i];
		if( ((uchar)(ch)>= 48) && ((uchar)(ch)<= 57) ) //0 to 9
			val += int( (ch-48)*multiplier );
		else if(  ((uchar)(ch)>= 65) && ((uchar)(ch)<= 70) ) //A to F
			val += int( (ch-55)*multiplier );
		else if(  ((uchar)(ch)>= 97) && ((uchar)(ch)<= 102) ) //a to f
			val += int( (ch-87)*multiplier );
		else
			return 0;
		multiplier *= 16;
	}
	return val;
}

int ConvertDecStringToInt(const CString& p_val)
{
	int val=0;
	int length = p_val.GetLength();
	if(length>=10)
		return 0;

	int multiplier=1;
	for(int i=1; i<=length; i++)
	{
		char ch = p_val[length-i];
		if( ((uchar)(ch)>= 48) && ((uchar)(ch)<= 57) ) //0 to 9
			val += int( (ch-48)*multiplier );
		else
			return 0;
		multiplier *= 10;
	}
	return val;
}

int ConvertBinStringToInt(const CString& p_val)
{
	int val=0;
	int length = p_val.GetLength();
	if(length>=32)
		return 0;

	int multiplier=1;
	for(int i=1; i<=length; i++)
	{
		char ch = p_val[length-i];
		if( ((uchar)(ch)>= 48) && ((uchar)(ch)<= 49) ) //0 to 9
			val += int( (ch-48)*multiplier );
		else
			return 0;
		multiplier *= 2;
	}
	return val;
}





