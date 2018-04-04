#include "OutputLayer.hpp"

#include "../Functions/Error/ErrorFunction.hpp"
#include "../Functions/Error/MSE.hpp"
#include <utility>

namespace opml
{
	void OutputLayer::calculateOutputErrorSignals(const vector3D&  target)
	{
		this->errorFunction->apply(this, target);
	}

	double OutputLayer::overall_error(const vector3D&  target)
	{
		return this->errorFunction->overall_error(*this, target);
	}

	OutputLayer OutputLayer::setErrorFunction(const std::shared_ptr<ErrorFunction>& errorFunc)
	{
		this->errorFunction = errorFunc;
		return *this;
	}

	void OutputLayer::on_build()
	{
		if (!this->errorFunction)
		{
			this->errorFunction = std::make_shared<MSE>();
		}
	}
}  // namespace opml