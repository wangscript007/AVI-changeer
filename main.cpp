

#include "aviCahngger.h"

int main(int argc, char **argv)
{
	int ret = 0;
	if (argc!=3)
	{
		std::cout << "invalid param:" << std::endl;
		std::cout << "sourceFileName rate" << std::endl;
		std::cout << "for example :" << std::endl << "test.avi 0.5" << std::endl;
		return -1;
	}
	float speed = 0;
	sscanf(argv[2], "%f", &speed);
	aviChanger instance(argv[1], speed);
	return ret;
}