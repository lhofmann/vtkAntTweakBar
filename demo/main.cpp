/*
 * Copyright (c) 2017 Lutz Hofmann.	
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty.h>

#include <AntTweakBar.h>
#include "vtkAntTweakBar.hpp"

using namespace std;

void TW_CALL SetColor(const void *value, void *clientData) {
	auto actor = static_cast<vtkActor*>(clientData);
	auto color = static_cast<const float*>(value);
	actor->GetProperty()->SetColor(color[0], color[1], color[2]);
}

void TW_CALL GetColor(void *value, void *clientData) {
	auto actor = static_cast<vtkActor*>(clientData);
	auto color = static_cast<float*>(value);
	auto actorColor = actor->GetProperty()->GetColor();
	for (int i = 0; i < 3; i++)
		color[i] = (float)actorColor[i];
}

void TW_CALL SetAlwaysRenderOnEvent(const void *value, void *clientData) {
	auto callback = static_cast<vtkAntTweakBar*>(clientData);
	bool alwaysRender = *static_cast<const bool*>(value);
	if (alwaysRender)
		callback->EnableAlwaysRenderOnEvent();
	else
		callback->DisableAlwaysRenderOnEvent();
}

void TW_CALL GetAlwaysRenderOnEvent(void *value, void *clientData) {
	auto callback = static_cast<vtkAntTweakBar*>(clientData);
	*static_cast<bool*>(value) = callback->AlwaysRenderOnEvent();
}

int main(int, char *[])
{
	auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(1.0);
	sphereSource->Update();

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(1.0, 1.0, 1.0);

	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);

	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(1280, 680);

	auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	auto axes = vtkSmartPointer<vtkAxesActor>::New();
	auto widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget->SetOrientationMarker(axes);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.2, 0.2);
	widget->SetEnabled(1);
	widget->InteractiveOff();

	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	renderWindowInteractor->SetInteractorStyle(style);

	renderer->ResetCamera();

	auto twCallback = vtkSmartPointer<vtkAntTweakBar>::New();
	twCallback->Initialize(renderWindowInteractor);

	TwBar* bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with VTK.' ");

	TwAddVarCB(bar, "Sphere Color", TW_TYPE_COLOR3F, SetColor, GetColor, actor, 
		" help='Change the color of the sphere' ");
	TwAddSeparator(bar, "", "");
	TwAddVarCB(bar, "Render On Event", TW_TYPE_BOOLCPP, SetAlwaysRenderOnEvent, GetAlwaysRenderOnEvent, twCallback, 
		" true='Always' false='If handled' help=`Specify vtkAntTweakBar's behaviour.` ");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
