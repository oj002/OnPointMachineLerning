#pragma once
#include "../EvoNet/EvoNet.hpp"

#if _OPML_ENABLE_SFML
#include <SFML\Graphics.hpp>
#else // _OPML_ENABLE_OPENCV
#pragma message ("opml warning: If you whant to use the FlapyBirds_Evo example pleas add '#define _OPML_ENABLE_SFML true' before including and setup the correct dependencies")
#endif // _OPML_ENABLE_OPENCV
#include <fstream>
#include <string>
#include <sstream>

namespace opml::Examples
{
	class FlapyBirds_Evo
	{
	public:
		template<typename ...Args>
		FlapyBirds_Evo(Args... layerSizes)
		{
		}


	private:


	private:
	};
}