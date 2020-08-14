#include "GravityManager.h"

void GravityManager::normalizeAngle(double* angle)
{
	while (*angle >= 2 * M_PI || *angle < 0)
	{
		if (*angle < 0)
		{
			*angle = (2 * M_PI) + *angle;
		}
		if (*angle >= 2 * M_PI)
		{
			*angle = *angle - (2 * M_PI);
		}
	}
}