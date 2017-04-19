/*
 * Copyright (c) 2017 Lutz Hofmann.	
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#include <assert.h>
#include <AntTweakBar.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCallbackCommand.h>
#include <vtkOpenGLRenderWindow.h>

#include "vtkAntTweakBar.hpp"

vtkAntTweakBar::vtkAntTweakBar()
	: initialized_(false),
	  alwaysRender_(false),
	  mouseWheelPosition_(0),
	  interactor_(nullptr),
	  interactorStyle_(nullptr)
{ }

vtkStandardNewMacro(vtkAntTweakBar);

void vtkAntTweakBar::EnableAlwaysRenderOnEvent() {
	alwaysRender_ = true;
}

void vtkAntTweakBar::DisableAlwaysRenderOnEvent() {
	alwaysRender_ = false;
}

bool vtkAntTweakBar::AlwaysRenderOnEvent() const {
	return alwaysRender_;
}

void vtkAntTweakBar::Initialize(vtkRenderWindowInteractor* interactor) {
	assert(!initialized_);
	assert(interactor);

	interactor->Initialize();

	vtkRenderWindow* renderWindow = interactor->GetRenderWindow();
	assert(renderWindow);
	
	if (vtkOpenGLRenderWindow::GetContextSupportsOpenGL32()) {
		TwInit(TW_OPENGL_CORE, NULL);
	}
	else {
		TwInit(TW_OPENGL, NULL);
	}

	initialized_ = true;
	interactor_ = interactor;
	interactorStyle_ = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
	assert(interactorStyle_);
	
	float priority = std::numeric_limits<float>::max();
	static const vtkCommand::EventIds interactorEvents[] = {
		vtkCommand::MouseMoveEvent,
		vtkCommand::LeftButtonPressEvent,
		vtkCommand::LeftButtonReleaseEvent,
		vtkCommand::RightButtonPressEvent,
		vtkCommand::RightButtonReleaseEvent,
		vtkCommand::MiddleButtonPressEvent,
		vtkCommand::MiddleButtonReleaseEvent,
		vtkCommand::MouseWheelForwardEvent,
		vtkCommand::MouseWheelBackwardEvent,
		vtkCommand::KeyPressEvent
	};
	for (auto e : interactorEvents) {
		interactorStyle_->AddObserver(e, this, priority);
	}

	renderWindow->AddObserver(vtkCommand::RenderEvent, this);
	renderWindow->AddObserver(vtkCommand::ModifiedEvent, this);

	int* size = renderWindow->GetSize();
	TwWindowSize(size[0], size[1]);
}

void vtkAntTweakBar::Terminate() {
	if (initialized_) {
		interactorStyle_->RemoveObserver(this);
		vtkRenderWindow* renderWindow = interactor_->GetRenderWindow();
		renderWindow->RemoveObserver(this);
		TwTerminate();
		initialized_ = false;
	}
}

void vtkAntTweakBar::Delete() {
	Terminate();
}

int KeySymToTwKeyCode(std::string key) {
	int keycode = 0;

	if (key.length() == 1 && 12 < key[0] && key[0] < 127) {
		keycode = key[0];
	}
	if (key == "Escape") keycode = TW_KEY_ESCAPE;
	if (key == "Help" || key == "F1") keycode = TW_KEY_F1;
	if (key == "F2") keycode = TW_KEY_F2;
	if (key == "F3") keycode = TW_KEY_F3;
	if (key == "F4") keycode = TW_KEY_F4;
	if (key == "F5") keycode = TW_KEY_F5;
	if (key == "F6") keycode = TW_KEY_F6;
	if (key == "F7") keycode = TW_KEY_F7;
	if (key == "F8") keycode = TW_KEY_F8;
	if (key == "F9") keycode = TW_KEY_F9;
	if (key == "F10") keycode = TW_KEY_F10;
	if (key == "F11") keycode = TW_KEY_F11;
	if (key == "F12") keycode = TW_KEY_F12;
	if (key == "Up") keycode = TW_KEY_UP;
	if (key == "Down") keycode = TW_KEY_DOWN;
	if (key == "Left") keycode = TW_KEY_LEFT;
	if (key == "Right") keycode = TW_KEY_RIGHT;
	if (key == "Return" || key == "KP_Enter") keycode = TW_KEY_RETURN;
	if (key == "Insert") keycode = TW_KEY_INSERT;
	if (key == "Delete") keycode = TW_KEY_DELETE;
	if (key == "BackSpace") keycode = TW_KEY_BACKSPACE;
	if (key == "Home") keycode = TW_KEY_HOME;
	if (key == "End") keycode = TW_KEY_END;
	if (key == "Tab") keycode = TW_KEY_TAB;

	return keycode;
}

void vtkAntTweakBar::Execute(vtkObject* caller, unsigned long event, void *) {
	int* pos = interactor_->GetEventPosition();
	bool handled = false;

	int windowHeight, modifiers, keycode;
	char c;
	vtkRenderWindow* window;
	int* size;	

	switch (event)
	{
	case vtkCommand::MouseMoveEvent:
		windowHeight = interactor_->GetRenderWindow()->GetSize()[1];
		if (!(handled = TwMouseMotion(pos[0], windowHeight - pos[1]))) {
			interactorStyle_->OnMouseMove();
		}
		break;
	case vtkCommand::LeftButtonPressEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT))) {
			interactorStyle_->OnLeftButtonDown();
		}
		break;
	case vtkCommand::LeftButtonReleaseEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT))) {
			interactorStyle_->OnLeftButtonUp();
		}
		break;
	case vtkCommand::RightButtonPressEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_RIGHT))) {
			interactorStyle_->OnRightButtonDown();
		}
		break;
	case vtkCommand::RightButtonReleaseEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_RIGHT))) {
			interactorStyle_->OnRightButtonUp();
		}
		break;
	case vtkCommand::MiddleButtonPressEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_MIDDLE))) {
			interactorStyle_->OnMiddleButtonDown();
		}
		break;
	case vtkCommand::MiddleButtonReleaseEvent:
		if (!(handled = TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_MIDDLE))) {
			interactorStyle_->OnMiddleButtonUp();
		}
		break;
	case vtkCommand::MouseWheelForwardEvent:
		mouseWheelPosition_++;
		if (!(handled = TwMouseWheel(mouseWheelPosition_))) {
			interactorStyle_->OnMouseWheelForward();
		}
		break;
	case vtkCommand::MouseWheelBackwardEvent:
		mouseWheelPosition_--;
		if (!(handled = TwMouseWheel(mouseWheelPosition_))) {
			interactorStyle_->OnMouseWheelBackward();
		}
		break;
	case vtkCommand::KeyPressEvent:
		modifiers = 0;
		if (interactor_->GetControlKey())
			modifiers |= TW_KMOD_CTRL;
		if (interactor_->GetShiftKey())
			modifiers |= TW_KMOD_SHIFT;
		if (interactor_->GetAltKey())
			modifiers |= TW_KMOD_ALT;
		c = interactor_->GetKeyCode();
		if (12 < c && c < 127) {
			keycode = c;
		}
		else {
			keycode = KeySymToTwKeyCode(interactor_->GetKeySym());
		}
		if (keycode != 0) {
			if (!(handled = TwKeyPressed(keycode, modifiers))) {
				interactorStyle_->OnKeyPress();
			}
		}
		break;
	case vtkCommand::RenderEvent:
		TwDraw();
		break;
	case vtkCommand::ModifiedEvent:
		window = static_cast<vtkRenderWindow*>(caller);
		size = window->GetSize();
		TwWindowSize(size[0], size[1]);
		break;
	default:
		break;
	}
	if (alwaysRender_ || handled) {
		interactor_->GetRenderWindow()->Render();
	}
}
