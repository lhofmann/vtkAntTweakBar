/*
 * Copyright (c) 2017 Lutz Hofmann.	
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#ifndef VTKANTTWEAKBAR_HPP
#define VTKANTTWEAKBAR_HPP

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>

class vtkAntTweakBar : public vtkCommand {
public:	
	static vtkAntTweakBar* New();
	void Initialize(vtkRenderWindowInteractor* interactor);
	void Execute(vtkObject* caller, unsigned long event, void *) VTK_OVERRIDE;
	void Terminate();
	void Delete() VTK_OVERRIDE;

	void EnableAlwaysRenderOnEvent();
	void DisableAlwaysRenderOnEvent();
	bool AlwaysRenderOnEvent() const;
private:
	vtkAntTweakBar();

	bool initialized_;
	bool alwaysRender_;
	int mouseWheelPosition_;
	vtkRenderWindowInteractor* interactor_;
	vtkInteractorStyle* interactorStyle_;
};

#endif
