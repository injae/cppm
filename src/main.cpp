#include<iostream>
#include"cppm.h"
#include<nieel/algorithm.hpp>
#include<nieel/adoptor.hpp>

int main(int argc, char* argv[])
{
    auto cppm = Cppm::instance(); 
    cppm->run(argc,argv);
	return 0;
}

