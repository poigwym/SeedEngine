#pragma once

#include"prequest.h"


namespace Seed
{

	class Timer
	{
	protected:
		float now, pre, dt;
	public:
		Timer() {
			now = pre = GetTickCount()*0.001;
			dt = 0;
		}
		virtual ~Timer() {} 
		inline virtual void update() {
			now = GetTickCount()*0.001;
			dt = now - pre;
			//pre = now;
		}
		inline virtual float getNow() const { return now; }
		inline virtual void setLastTime(float t) { pre = t; }

		float getDelta() { return dt; }
		bool isPass(float t) { return dt >= t; }
	};



}