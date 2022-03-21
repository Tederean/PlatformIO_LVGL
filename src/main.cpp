#include <Arduino.h>
#include <application/gui/GuiService.h>
#include <application/display/DisplayService.h>
#include <framework/services/SystemService.h>

void loop()
{
	Services::System::LoopEvent.Invoke(nullptr);
}

void setup()
{
	Services::System::Initialize();

	Services::Display::Initialize();
	Services::Gui::Initialize();
}