#pragma once
#include <memory>

namespace dae {
	class Controller
	{
		public:
			enum class button : unsigned int
			{

                DpadUp = 0x0001,
                DpadDown = 0x0002,
                DpadLeft = 0x0004,
                DpadRight = 0x0008,
                Start = 0x0010,
                Back = 0x0020,
                LeftThumb = 0x0040,
                RightThumb = 0x0080,
                LeftShoulder = 0x0100,
                RightShoulder = 0x0200,
                ButtonA = 0x1000,
                ButtonB = 0x2000,
                ButtonX = 0x4000,
                ButtonY = 0x8000
		    };

			explicit Controller(unsigned int controllerIndex);
            ~Controller();

			Controller(const Controller&) = delete;
			Controller& operator=(const Controller&) = delete;
			Controller(Controller&&) = delete;
			Controller& operator=(Controller&&) = delete;


			void Update();
			bool IsDownThisFrame(button button) const;
			bool IsUpThisFrame(button button) const;
			bool IsPressed(button button) const;
			bool IsConnected() const;
			unsigned int GetControllerIndex() const;

		private:
			class ControllerImpl;
			std::unique_ptr<ControllerImpl> m_pImpl;

	};
}