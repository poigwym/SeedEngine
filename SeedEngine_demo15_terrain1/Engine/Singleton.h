#pragma once

namespace Seed {


	template<class T>
	class Singleton {
	private:
		Singleton(const Singleton&) {}
		Singleton& operator =(const Singleton&){}

	protected:
		static T *instance;

	public:
		Singleton() { assert(instance == 0); }
		~Singleton(){
			assert(instance != 0);
		}
		static T& getInstance() { return *instance; }
		static T* getInstancePtr() { return instance; }
	};
	//template<class T>
	//T* Singleton<T>::instance = 0;
}