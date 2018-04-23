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
	omp_set_num_threads(omp_get_max_threads() / 2);
	opml::Examples::Image_Evo imgEvo(1000, "test.png");
	imgEvo.run();
	/*
	opml::Examples::FlappyBirds_Evo evo(100, 50.0, 35.0, 10);
	evo.run();*/

	/*double total{ 0 };
	for (size_t i = 0; i < 10; i++)
	{
		const opml::Clock c;
		opml::Examples::Mnist_NN mnist(200);
		mnist.train(5, 0.15, 0.001);
		mnist.test();
		const double elapsed{ c.getElapsedTime<double>() };
		total += elapsed;
		std::cout << "Time took: " << elapsed << "s" << std::endl;
	}
	std::cout << "Average: " << (total / 10.0) << "s" << std::endl;*/

	// Average: 27.1962s
	// Average: 26.6373s

	system("pause");
	return 0;
}
