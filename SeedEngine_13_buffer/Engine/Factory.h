#pragma once

#include<string>

namespace NBengine
{

	template<class T> class Factory {
	public:
		virtual ~Factory(){}
		virtual T* createInstance(const std::string &) = 0;
		virtual const std::string& getType() = 0;
	};

}
