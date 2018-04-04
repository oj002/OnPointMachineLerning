#include "../OnPointMachineLerning/OnPointMachineLerning.hpp"
#include <iostream>

#define _OPML_ENABLE_OPENCV

/*
	Git canot push files larger than 100 mb
	if they are bigger than 50 mb there will be a warning

	Therefore only publish with binary's or large resources in an release
*/

int main()
{
	opml::Examples::Mnist_NN mnist(200);
	mnist.run();

	return 0;
}
