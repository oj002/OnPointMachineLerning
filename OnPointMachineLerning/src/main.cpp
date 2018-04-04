#include "../OnPointMachineLerning/OnPointMachineLerning.hpp"
#include <iostream>

#define _OPML_ENABLE_OPENCV

/*
	Git can't push files larger than 100 mb
	if they are bigger than 50 mb there will be a warning.
	If the files need to be in the local directory 
	go into the Team-Explorer-Changes and click ignore this local item.

	Therefore only publish with binary's or large resources in an release.
*/

int main()
{
	opml::Examples::Mnist_NN mnist(20 * 20, 15 * 15, 10 * 10, 50);
	mnist.train(5, 0.25, 0.0001);
	mnist.train(5, 0.15, 0.0001);
	mnist.train(5, 0.075, 0.0001);
	mnist.train(5, 0.05, 0.0001);
	mnist.test();

	// acc: (9750/10000): 0.975
	system("pause");
	return 0;
}

