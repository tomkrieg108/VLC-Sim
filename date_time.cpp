#include "stdafx.h"
#include "date_time.h"
#include "Utilities.h"



COleDateTimeSpan CVLCDateTime::m_offset_system_to_universal(0,0,0,0);

CVLCDateTime::CVLCDateTime():
	m_ole_date_time(COleDateTime::GetCurrentTime())
{
	//Initialise to universal date/time
	m_ole_date_time += m_offset_system_to_universal;
}

CVLCDateTime::CVLCDateTime(const CVLCDateTime &p_copy ):
	m_ole_date_time(p_copy.m_ole_date_time)
{
}

CVLCDateTime::CVLCDateTime(const COleDateTime &p_copy ):
	m_ole_date_time(m_ole_date_time)
{
}

CVLCDateTime& CVLCDateTime::operator=(const CVLCDateTime &p_copy)
{
	m_ole_date_time = p_copy.m_ole_date_time;
	return *this;
}

ushort CVLCDateTime::GetJulianDate()
{
	COleDateTime start_date_time ;
	COleDateTimeSpan julian_date_time;

	start_date_time.SetDateTime(1900,3,1,0,0,0);
	julian_date_time = m_ole_date_time - start_date_time;

	return (int)(julian_date_time.m_span)+1;

}


ushort CVLCDateTime::GetTimeInSeconds()
{
	int time = m_ole_date_time.GetSecond();
	time += m_ole_date_time.GetMinute()*60;
	time += m_ole_date_time.GetHour()*60*60;
	time /= 2;
	return time;
}


void CVLCDateTime::Set(ushort p_julian_date, ushort p_time_in_seconds)
{
	m_ole_date_time.SetDateTime(1900,3,1,0,0,0);
	COleDateTimeSpan date_offset(p_julian_date-1,0,0,0);

	m_ole_date_time += date_offset;

	int rem_secs = p_time_in_seconds*2; //seconds into current day
	int hours = rem_secs/3600;
	rem_secs = rem_secs%3600;
	int mins = rem_secs/60;
	rem_secs = rem_secs%60;
	

	COleDateTimeSpan time_offset(0,hours,mins,rem_secs);
	m_ole_date_time += time_offset;
}


//Note: Coould instead use ParseDateTime() for this and the next one
bool CVLCDateTime::SetDate(CString& p_date)
{
	//must be ddmmyy
	if(p_date.GetLength() != 6)
		return false;
	CString dd,mm,yy;
	try
	{
		dd = p_date.Left(2);
		mm = p_date.Mid(2,2);
		yy = p_date.Right(2);
	}
	catch(CMemoryException& e)
	{
		e.Delete();
		return false;
	}
	int ret = m_ole_date_time.SetDate( Convert(yy)+2000, Convert(mm), Convert(dd) ); //year is 4 digits
	return (ret==0);
}


bool CVLCDateTime::SetTime(CString& p_time)
{
	//must be hhmmss
	if(p_time.GetLength() != 6)
		return false;
	CString hh,mm,ss;
	try
	{
		hh = p_time.Left(2);
		mm = p_time.Mid(2,2);
		ss = p_time.Right(2);
	}
	catch(CMemoryException& e)
	{
		e.Delete();
		return false;
	}
	int ret = m_ole_date_time.SetTime( Convert(hh), Convert(mm), Convert(ss) );
	return (ret==0);
}


CString CVLCDateTime::GetDate(bool p_with_formatting)
{
	CString dd = Convert( m_ole_date_time.GetDay() );
	if(dd.GetLength()==1)
		dd.Insert(0,'0'); //leading 0.

	CString mm = Convert( m_ole_date_time.GetMonth() );
	if(mm.GetLength()==1)
		mm.Insert(0,'0');

	CString yy = Convert( m_ole_date_time.GetYear() );
	if(yy.GetLength()==4)
		yy = yy.Right(2);

	if(p_with_formatting)
		return dd + "/" + mm + "/" + yy;
	else
		return dd+mm+yy;
}


CString CVLCDateTime::GetTime(bool p_with_formatting)
{
	CString hh = Convert( m_ole_date_time.GetHour() );
	if(hh.GetLength()==1)
		hh.Insert(0,'0'); //leading 0.

	CString mm = Convert( m_ole_date_time.GetMinute() );
	if(mm.GetLength()==1)
		mm.Insert(0,'0');

	CString ss = Convert( m_ole_date_time.GetSecond() );
	if(ss.GetLength()==1)
		ss.Insert(0,'0');	

	if(p_with_formatting)
		return hh + ":" + mm + ":" + ss ;
	else
		return hh+mm+ss;
}


int CVLCDateTime::Difference(const CVLCDateTime &p_date_time)
{
	COleDateTimeSpan time_span = m_ole_date_time - p_date_time.m_ole_date_time;
	int diff = (int)time_span.GetTotalSeconds();
	return diff;
}

int Difference(const CVLCDateTime &p_date_time1, const CVLCDateTime &p_date_time2)
{
	COleDateTimeSpan time_span = p_date_time1.m_ole_date_time - p_date_time2.m_ole_date_time;
	int diff = (int)time_span.GetTotalSeconds();
	return diff;
}


void CVLCDateTime::SetAsSystemTime()
{
	m_ole_date_time = COleDateTime::GetCurrentTime();
}

void CVLCDateTime::SetAsUniversalTime()
{
	m_ole_date_time = COleDateTime::GetCurrentTime() + m_offset_system_to_universal;
}

void CVLCDateTime::SetAsLocalTime(int p_offset_num_half_hours)
{
	int offset_int = p_offset_num_half_hours;
	if(p_offset_num_half_hours<0)
		offset_int = -p_offset_num_half_hours;

	if(offset_int>24) offset_int=24;
	if(offset_int<-24) offset_int= -24;

	int hours = offset_int/2;
	int minutes = (offset_int%2)*30;

	COleDateTimeSpan offset(0,hours,minutes,0);
	
	m_ole_date_time = COleDateTime::GetCurrentTime() + m_offset_system_to_universal + offset;
}


void CVLCDateTime::SetUniversalDateTime(const CVLCDateTime &p_date_time)
{
	COleDateTime system_datetime = COleDateTime::GetCurrentTime();
	m_offset_system_to_universal = p_date_time.m_ole_date_time - system_datetime; //param minus system
}




//////////////////////////////////////////////////////////////////////////////////////////////////
LARGE_INTEGER CTimeStamp::m_frequency;

void CTimeStamp::Initialise()
{
	QueryPerformanceFrequency(&m_frequency);
}

CTimeStamp::CTimeStamp()
{
	QueryPerformanceCounter(&m_timestamp);
}

CTimeStamp::CTimeStamp(const CTimeStamp &p_copy )
{
	m_timestamp.QuadPart = p_copy.m_timestamp.QuadPart;
}


CTimeStamp& CTimeStamp::operator=(const CTimeStamp &p_copy)
{
	m_timestamp.QuadPart = p_copy.m_timestamp.QuadPart;
	return *this;
}

void CTimeStamp::Stamp()
{
	QueryPerformanceCounter(&m_timestamp);
}

int CTimeStamp::Difference(const CTimeStamp& p_time)
{
	int delay_ms = (int)(1000*(double(m_timestamp.QuadPart - p_time.m_timestamp.QuadPart) / (double)(CTimeStamp::m_frequency.QuadPart))); // time in ms
	return delay_ms;
}

int Difference(const CTimeStamp& p_time1, const CTimeStamp& p_time2)
{
	int delay_ms = (int)(1000*(double(p_time1.m_timestamp.QuadPart - p_time2.m_timestamp.QuadPart) / (double)(CTimeStamp::m_frequency.QuadPart))); // time in ms
	return delay_ms;
}