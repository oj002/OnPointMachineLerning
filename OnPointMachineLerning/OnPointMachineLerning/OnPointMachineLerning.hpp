#pragma once
#define SFML_STATIC
#ifdef _OPML_ENABLE_OPENCV
#ifdef _DEBUG

#else // _DEBUG

#endif // _DEBUG 
#endif // _OPML_ENABLE_OPENCV

#ifdef _OPML_ENABLE_SFML
#ifdef _DEBUG
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "sfml-system-s-d.lib")

	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "sfml-network-s-d.lib")

	#pragma comment(lib, "openal32.lib")
	#pragma comment(lib, "flac.lib")
	#pragma comment(lib, "vorbisenc.lib")
	#pragma comment(lib, "vorbisfile.lib")
	#pragma comment(lib, "vorbis.lib")
	#pragma comment(lib, "ogg.lib")
	#pragma comment(lib, "sfml-audio-s-d.lib")

	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "sfml-window-s-d.lib")

	#pragma comment(lib, "freetype.lib")
	#pragma comment(lib, "jpeg.lib")
	#pragma comment(lib, "sfml-graphics-s-d.lib")
#else // _DEBUG
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "sfml-system-s.lib")
	
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "sfml-network-s.lib")
	
	#pragma comment(lib, "openal32.lib")
	#pragma comment(lib, "flac.lib")
	#pragma comment(lib, "vorbisenc.lib")
	#pragma comment(lib, "vorbisfile.lib")
	#pragma comment(lib, "vorbis.lib")
	#pragma comment(lib, "ogg.lib")
	#pragma comment(lib, "sfml-audio-s.lib")
	
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "sfml-window-s.lib")
	
	#pragma comment(lib, "freetype.lib")
	#pragma comment(lib, "jpeg.lib")
	#pragma comment(lib, "sfml-graphics-s.lib")
#endif // _DEBUG 
#endif // _OPML_ENABLE_SFML


#include "Examples\Mnist.hpp"
#include "NeuralNetwork\data\TrainSet.hpp"
#include "NeuralNetwork\Functions\Functions.hpp"
#include "NeuralNetwork\Layers\DenseLayer.hpp"
#include "NeuralNetwork\Layers\InputLayer.hpp"
#include "NeuralNetwork\Layers\Layer.hpp"
#include "NeuralNetwork\Layers\OutputLayer.hpp"
#include "NeuralNetwork\Layers\PoolingLayer.hpp"
#include "NeuralNetwork\Layers\TransformLayer.hpp"
#include "NeuralNetwork\Network.hpp"
#include "NeuralNetwork\NetworkBuilder.hpp"
#include "NeuralNetwork\Tools\ArrayTools.hpp"
#include "NeuralNetwork\Tools\Clock.hpp"
#include "NeuralNetwork\Tools\Random.hpp"
