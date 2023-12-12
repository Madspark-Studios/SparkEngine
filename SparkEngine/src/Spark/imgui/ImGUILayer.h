#pragma once
#include "Spark/Layer.h"

namespace Spark
{
	class SPARK_API ImGUILayer : public Layer
	{
	public:
		ImGUILayer();
		~ImGUILayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);
	private:
		float m_Time = 0;
	};
}
