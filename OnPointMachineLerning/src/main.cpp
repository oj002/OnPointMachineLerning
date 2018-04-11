#define _OPML_ENABLE_OPENMP true
#include "../OnPointMachineLerning/OnPointMachineLerning.hpp"
#include "../OnPointMachineLerning/Examples/Image_Evo.hpp"
#include <iostream>


/*
	Git can't push files larger than 100 mb
	if they are bigger than 50 mb there will be a warning.
	If the files need to be in the local directory 
	go into the Team-Explorer-Changes and click ignore this local item.

	Therefore only publish with binary's or large resources in an release.

	Sorry for the bad exception handling, its my first project extensively using it.
*/


int main()
{
	opml::Examples::Image_Evo imgEvo(750, "test.png", 5.0);
	imgEvo.run();
	/*opml::Examples::FlappyBirds_Evo evo(100, 50.0, 35.0, 10);
	evo.run();*/
	
	/*opml::Examples::Mnist_NN mnist(200);
	mnist.train(10, 0.1, 0.001);
	mnist.test();
	mnist.show(100);*/
	
	cv::waitKey();

	return 0;
}
