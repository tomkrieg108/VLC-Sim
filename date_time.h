
#ifndef __DATE_TIME_H
#define __DATE_TIME_H

#include "stdtype.h"

class CVLCDateTime
{
	friend int Difference(const CVLCDateTime &p_date_time1, const CVLCDateTime &p_date_time2); //time1 - time2 In seconds
public:

	CVLCDateTime();	//Initialised to universal time
	CVLCDateTime(const CVLCDateTime &p_copy );
	CVLCDateTime(const COleDateTime &p_copy );
	CVLCDateTime& operator=(const CVLCDateTime &p_copy);

	ushort GetJulianDate();
	ushort GetTimeInSeconds();
	void Set(ushort p_julian_date, ushort p_time_in_seconds);
	bool SetDate(CString& p_date);
	bool SetTime(CString& p_time);
	CString GetDate(bool p_with_formatting = false);
	CString GetTime(bool p_with_formatting = false);

	int Difference(const CVLCDateTime &p_date_time);		//object time - parameter time  In seconds

	void SetAsSystemTime();
	void SetAsUniversalTime();
	void SetAsLocalTime(int p_offset_num_half_hours);
	
	static void SetUniversalDateTime(const CVLCDateTime &p_date_time);

	
private:

	COleDateTime m_ole_date_time;
	static COleDateTimeSpan m_offset_system_to_universal;

};


class CTimeStamp
{
	friend int Difference(const CTimeStamp &p_time1, const CTimeStamp &p_time2); //In seconds

public:
	static void Initialise();
	CTimeStamp();
	CTimeStamp(const CTimeStamp &p_copy );
	CTimeStamp& operator=(const CTimeStamp &p_copy);
	
	void Stamp();
	int Difference(const CTimeStamp &p_time);	//In milli-seconds
private:

	LARGE_INTEGER m_timestamp;
	static LARGE_INTEGER m_frequency;

};


#endif