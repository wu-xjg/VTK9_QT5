#include <QMainWindow>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
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
#include <vtkPolyLine.h>
//#include "QtWidgetsTest.h"

//vtkSmartPointer<vtkRenderer> line_renderer;
int line_number = 0;
int old_line_number = 0;
vtkIdType pointIds[100][2];

class LineStyle :public vtkInteractorStyleTrackballCamera {

public:
	static inline LineStyle* New();
	vtkTypeMacro(LineStyle, vtkInteractorStyleTrackballCamera);
	int* clickPos;
	int* clickPos1;
	double point1[3];
	double point2[3];
	double worldPoint1[3];
	double worldPoint2[3];
	//QtWidgetsTest* mainWindow;
	
	vtkSmartPointer<vtkCellArray> cellArrays;
	vtkSmartPointer<vtkPoints> points= vtkSmartPointer<vtkPoints>::New();

	int lines = 0;
	vtkSmartPointer<vtkPoints> getPoints() {
		return points;
	}
	vtkSmartPointer<vtkCellArray> getArrays() {
		return cellArrays;
	}
	int getlines() {
		return lines;
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
		//QLabel* label = mainWindow->getLabel();
		//QLabel* label = new QLabel();
		//label->setText("Hello, World!");
		//
		////QString labelText = QString("Line Number: %1").arg(lines);
		////label->setText(labelText);
		//label->setGeometry(10, 10, 100, 30);
		//mainWindow->tbar->addWidget(label);
		clickPos1 = this->GetInteractor()->GetEventPosition();
		point2[0] = clickPos1[0];
		point2[1] = clickPos1[1];
		
		
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(point2[0], point2[1], 0);
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
		worldPoint2[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
		worldPoint2[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
		worldPoint2[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];

		vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
		lineSource->SetPoint1(worldPoint1);
		lineSource->SetPoint2(worldPoint2);

		//cellArray->InsertNextCell(2);
		pointIds[lines][0] = points->InsertNextPoint(worldPoint1);
		//cellArray->InsertCellPoint(0);
		pointIds[lines][1] = points->InsertNextPoint(worldPoint2);
	
	

		lines++;

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(lineSource->GetOutputPort());
		//mapper->SetInputData(polyData);
	
		vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
		actor1->SetMapper(mapper);
		actor1->GetProperty()->SetColor(0.0, 0.0, 1.0);

		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor1);
	
		this->Interactor->GetRenderWindow()->Render();

		line_number++;
	
	}

};
vtkStandardNewMacro(LineStyle);