#ifndef AWARD_INTERFACE_H
#define AWARD_INTERFACE_H
class AwardInterface
{
public:
	virtual void OnAwardCompleted(int awardId) = 0;
};
#endif

