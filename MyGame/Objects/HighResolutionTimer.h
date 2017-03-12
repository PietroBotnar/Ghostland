#ifndef HIGHRESOLUTIONTIMER_H
#define HIGHRESOLUTIONTIMER_H

#include <windows.h>

class CHighResolutionTimer 
{
public:
	CHighResolutionTimer();
	~CHighResolutionTimer();

	void	Start();
	double	Elapsed();

private:
	LARGE_INTEGER m_t1, m_t2;
	bool m_started;
};

#endif