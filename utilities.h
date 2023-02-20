

#ifndef __UTILITIES_H
#define __UTILITIES_H

#include "stdtype.h"

//int StringToBin(CString input);
//CString HexToString(int input);
int StringToHex(CString input);
byte CharToHex(char input);
char HexToChar(byte p_input);
CString HexToString(byte p_input);
//int BinToHex(int input);
//CString ReverseASCIIBytes(CString input);
//CString ByteToString(BYTE input);

int Convert(const CString& p_val);
CString Convert(const int p_val);
CString Convert(const uint p_val);
CString Convert(const float p_val);

uint SwapBytes(uint p_input);
ushort SwapBytes(ushort p_input);

int GetUniversalTime();
int GetJulianDate();


bool IsDec(char p_char);
bool IsHex(char p_char);
bool IsBin(char p_char);

bool IsDec(const CString& p_val);
bool IsHex(const CString& p_val);
bool IsBin(const CString& p_val);

int ConvertHexStringToInt(const CString& p_val);
int ConvertDecStringToInt(const CString& p_val);
int ConvertBinStringToInt(const CString& p_val);

#endif