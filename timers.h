
#ifndef __TIMERS_H
#define __TIMERS_H


class ITimer;

class CTimers
{

public:

	static void Initialise();
	static void Destroy();

	void ProcessTimeout( int p_id );
	int AddTimer(int p_rate, ITimer* p_timer);
	void RemoveTimer(int &p_id);
	void ResetTimer(int& p_id, int p_rate, ITimer* p_timer);


private:

	enum
	{
		CAPACITY = 50
	};

	CTimers(void);
	~CTimers(void);
		
	ITimer*  m_timers[CAPACITY];
	int m_count;
};



class ITimer
{

friend class CTimers;

protected:
	ITimer() {}
	virtual ~ITimer() {}
	virtual void ProcessTimeout( int p_id );

};


extern CTimers *g_timers;

#endif