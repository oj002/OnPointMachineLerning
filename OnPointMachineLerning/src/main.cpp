#include "../OnPointMachineLerning/OnPointMachineLerning.hpp"
#include <iostream>

#define _OPML_ENABLE_OPENCV

int main()
{
	opml::Examples::Mnist_NN mnist(200);
	mnist.run();

	return 0;
}
