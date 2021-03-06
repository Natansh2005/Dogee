// Dogee.cpp : 定义控制台应用程序的入口点。
//
#ifdef _WIN32
#include "stdafx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DogeeHelper.h" //there is a GCC bug. DogeeHelper.h should be included before DogeeMacro.h
#include "DogeeBase.h"
#include "DogeeStorage.h"
#include "DogeeRemote.h"
#include "DogeeThreading.h"
#include "DogeeMacro.h"

#include <memory>
using namespace Dogee;

class clsa : public DObject
{
	DefBegin(DObject);
public:
	Def(i, int);
	Def(m_vector, Array<double>);
	Def(m_matrix, Array<Array<int>>);
	Def(prv, Ref<clsa>);
	DefEnd();
	clsa(ObjectKey obj_id) : DObject(obj_id)
	{
	}
	clsa(ObjectKey obj_id, int a) : DObject(obj_id)
	{
		self->m_vector = NewArray<double>(10);
		self->m_matrix = NewArray<Array<int>>(10);
		self->m_matrix[0] = NewArray<int>(10);
	}

	void Destroy()
	{
		std::cout << "destroctor" << std::endl;
		DelArray<double>(self->m_vector);
		DelArray<int>(self->m_matrix[0]);
		DelArray<Array<int>>(self->m_matrix);
	}
};

DefGlobal(g_i,int);
DefGlobal(g_obj, Ref<clsa>);
DefGlobal(sem,Ref<DSemaphore>);

void threadfun(uint32_t param)
{
	std::cout << "The value of g_i is " << g_i << std::endl;
	sem->Acquire(-1);
	g_obj->m_matrix[0][2] = 23;
	std::cout << "The value of g_obj->i is " << g_obj->i << std::endl;
}
RegFunc(threadfun);



int main(int argc, char* argv[])
{
	HelperInitCluster(argc, argv);

	//initialize the shared variables
	g_i = 123445;
	g_obj = NewObj<clsa>(3);
	g_obj->i = 1234;
	sem = NewObj<DSemaphore>(0);

	//create a remote thread on node 1, give the parameter "3232"
	Ref<DThread> thread = NewObj<DThread>(threadfun, 1, 3232);


	std::string dummy;
	std::cout << "Input any string to resume the remote thread" << std::endl;
	std::cin >> dummy;
	//Release the semaphore
	sem->Release();
	DelObj(g_obj);
	std::cout << "Input any string to close the cluster" << std::endl;
	std::cin >> dummy;
	CloseCluster();
	
	return 0;
}
