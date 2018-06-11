#include<iostream>
#include"cppm.h"

int main(int argc, char* argv[])
{
    auto cppm = Cppm::instance(); 
    cppm->run(argc,argv);
	return 0;
}

