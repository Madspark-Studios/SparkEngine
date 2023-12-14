#pragma once
#include "Spark/Core.h"
#include "Spark/Input.h"

namespace Spark
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyDownImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	private:
		static Input* s_Instance;
	};
}