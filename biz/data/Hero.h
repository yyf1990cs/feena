#pragma once
#include "Unit.h"


class Hero : public Unit
{
public:
	using Ptr = std::shared_ptr < Hero >;
public:
	Hero(const GameObjInfo& info);
	virtual ~Hero();

	virtual bool tick(float delta) override;


private:
	
};

