
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include<vtkLine.h>
#include<vtkProperty.h>
#include<vtkPolyLine.h>
#include<vtkUnstructuredGrid.h>
#include<vtkDataSetMapper.h>
#include<vtkLineSource.h>
#include<vtkRendererCollection.h>
#include<vtkCellPicker.h>
#include "vtkAutoInit.h"
//vtkSmartPointer<vtkRenderer> cube_renderer;
int cube_number = 0;
int old_cube_number=0;
vtkIdType c_poi[30][24];
class CubeStyle :public vtkInteractorStyleTrackballCamera {
public:
	static inline CubeStyle* New();
	vtkTypeMacro(CubeStyle, vtkInteractorStyleTrackballCamera);
	int* clickPos;
	int* clickPos1;
	double point1[3];
	double point2[3];
	double worldPoint1[3];
	double worldPoint2[3];
	int cubes = 0;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> getPoints() {
		return points;
	}
	int getcubes() {
		return cubes;
	}
	virtual void OnLeftButtonDown() {
		clickPos = this->GetInteractor()->GetEventPosition();
		point1[0] = clickPos[0];
		point1[1] = clickPos[1];

		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(point1[0], point1[1], 0);
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		worldPoint1[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
		worldPoint1[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
		worldPoint1[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];
	};

	virtual void OnLeftButtonUp()
	{

		clickPos1 = this->GetInteractor()->GetEventPosition();
		point2[0] = clickPos1[0];
		point2[1] = clickPos1[1];



		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(point2[0], point2[1], 0);
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		worldPoint2[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
		worldPoint2[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
		worldPoint2[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];

		double a = sqrt((worldPoint2[0] - worldPoint1[0]) * (worldPoint2[0] - worldPoint1[0]) + (worldPoint2[1] - worldPoint1[1]) * (worldPoint2[1] - worldPoint1[1]));
		
		vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
		cube->SetXLength(a);
		cube->SetYLength(a);
		cube->SetZLength(a);
		cube->SetCenter((worldPoint2[0] + worldPoint1[0]) / 2, (worldPoint2[1] + worldPoint1[1]) / 2, 0);
		
		//cube->SetCenter((clickPos1[0] + clickPos[0]) / 2, (clickPos1[1] + clickPos[1]) / 2, 0);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

		mapper->SetInputConnection(cube->GetOutputPort());
		mapper->Update();

		vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
		pb = cube->GetOutput()->GetPoints();
		for (int i = 0; i < pb->GetNumberOfPoints(); ++i) {
			double* point_bdr = pb->GetPoint(i);
			c_poi[cubes][i]= points->InsertNextPoint(point_bdr);
			qDebug() << point_bdr[0]<< point_bdr[1] << point_bdr[2] ;
		}


		vtkSmartPointer<vtkActor> actor3 = vtkSmartPointer<vtkActor>::New();
		actor3->SetMapper(mapper);
		actor3->GetProperty()->SetColor(1, 1, 1);
		actor3->GetProperty()->SetOpacity(0.5);
		actor3->GetProperty()->SetLineWidth(5);
		actor3->GetProperty()->SetDiffuse(1);
		actor3->GetProperty()->SetEdgeColor(0, 0, 1);

		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor3);
		
		this->Interactor->GetRenderWindow()->Render();
		/*cube_renderer->GetRenderWindow()->GetInteractor()->Initialize();
		cube_renderer->GetRenderWindow()->GetInteractor()->Render();*/
		cubes++;
		cube_number++;
	}

};
vtkStandardNewMacro(CubeStyle);