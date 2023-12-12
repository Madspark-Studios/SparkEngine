#pragma once
#include "Spark/Layer.h"
#include "Spark/Events/MouseEvent.h"
#include "Spark/Events/KeyEvent.h"
#include "Spark/Events/ApplicationEvent.h"

namespace Spark
{
	class SPARK_API ImGUILayer : public Layer
	{
	public:
		ImGUILayer();
		~ImGUILayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGUIRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0;
	};
}
