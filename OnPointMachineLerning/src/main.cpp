#define _OPML_ENABLE_OPENCV true
#define _OPML_ENABLE_SFML true
#include "../OnPointMachineLerning/OnPointMachineLerning.hpp"
#include <iostream>

/*
	Git can't push files larger than 100 mb
	if they are bigger than 50 mb there will be a warning.
	If the files need to be in the local directory 
	go into the Team-Explorer-Changes and click ignore this local item.

	Therefore only publish with binary's or large resources in an release.
*/

int main()
{
	opml::Examples::Mnist_NN mnist(200);
	mnist.train(10, 0.1, 0.001);
	mnist.test();
	mnist.show(25);

	#ifdef _OPML_ENABLE_OPENCV
	cv::waitKey();
	#else
	system("pause");
	#endif // _OPML_ENABLE_OPENCV

	return 0;
}
