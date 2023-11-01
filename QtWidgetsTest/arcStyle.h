
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
#include<vtkArcSource.h>
#include<vtkRendererCollection.h>
#include<vtkCellPicker.h>
#include "vtkAutoInit.h"
//vtkSmartPointer<vtkRenderer> arc_renderer;
int arc_number = 0;
int old_arc_number = 0;
long int pnt[20][5000];//�����裬��ȡ��������ȡ�㼯
int pnt_num[20];
class ArcStyle :public vtkInteractorStyleTrackballCamera {
public:
	static inline ArcStyle* New();
	vtkTypeMacro(ArcStyle, vtkInteractorStyleTrackballCamera);
	int* clickPos;
	int* clickPos1;
	double point1[3];
	double point2[3];
	double worldPoint1[3];
	double worldPoint2[3];
	int arcs = 0;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> getPoints() {
		return points;
	}
	int getarcs() {
		return arcs;
	}
	double arf = 1 ;
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
		double circleCenter[3] = { worldPoint1[0], worldPoint1[1], worldPoint1[2] };
		//Բ�뾶
		//double circleCenter[3] = { point1[0], point1[1], point1[2] };
		double r = sqrt((worldPoint2[0] - worldPoint1[0]) * (worldPoint2[0] - worldPoint1[0]) + (worldPoint2[1] - worldPoint1[1]) * (worldPoint2[1] - worldPoint1[1]));
		//double r = sqrt((point2[0] - point1[0]) * (point2[0] - point1[0]) + (point2[1] - point1[1]) * (point2[1] - point1[1]));
		//Բ���ϵ����ڵ㹹�ɵ��ҳ�
		double l = 0.001;
		
		//���ҳ��Ͱ뾶���㻡��
		double theta = 2 * asin(l / (2 * r));
		double M_PI = 3.14159265;
		//Բ���ϵ����Ŀ2* M_PI
		long long pntNum = (long long)(arf * M_PI / theta);
		//����Բ���ϵĵ�
		vtkSmartPointer<vtkPoints> arc_points = vtkSmartPointer<vtkPoints>::New();
		for (long long i = 0; i < pntNum; i++)
		{
			arc_points->InsertPoint(i, circleCenter[0] + r * cos(i * theta), circleCenter[1] + r * sin(i * theta), 0);
		}
		for (long long i = 0; i < pntNum; i++)
		{
			pnt[arcs][i] = points->InsertNextPoint(circleCenter[0] + r * cos(i * theta), circleCenter[1] + r * sin(i * theta), 0);
		}
		pnt_num[arcs] = pntNum;
		arcs++;
		qDebug() << pntNum;
		vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();

		polyLine->GetPointIds()->SetNumberOfIds(pntNum);
		for (long long i = 0; i < pntNum; i++)
		{
			polyLine->GetPointIds()->SetId(i, i); //��һ��������polyline�еĵ��������ڶ���������point�еĵ�����
		}

		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
		cells->InsertNextCell(polyLine);
		vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
		//�����
		polyData->SetPoints(arc_points);
		//���������ӷ�ʽ�����㰴�����ķ�ʽ���ӳɶ����
		polyData->SetLines(cells);
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(polyData);
		mapper->Update();
		//	vtkSmartPointer<vtkArcSource> arcSource = vtkSmartPointer<vtkArcSource>::New();
		//	arcSource->SetPoint1(worldPoint1);
		//	arcSource->SetPoint2(worldPoint2);
		///*	arcSource->SetAngle(30);*/
			//arcSource->SetCenter(0,0,0);
		//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		//	mapper->SetInputConnection(arcSource->GetOutputPort());
		vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
		actor2->SetMapper(mapper);
		actor2->GetProperty()->SetColor(0.0, 0.0, 1.0);
		actor2->GetProperty()->SetOpacity(0.5);
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor2);
		
		this->Interactor->GetRenderWindow()->Render();
		arc_number++;
	}
	virtual void OnKeyPress()
	{
		std::string key = this->GetInteractor()->GetKeySym();

		if (key == "Up") {
			// ����Բ���Ƕ�
			arf += 0.1;
		}
		else if (key == "Down") {
			// ����Բ���Ƕ�
			arf -= 0.1;
		}

		// ����Բ��
		//UpdateArc();

		
	}
};
vtkStandardNewMacro(ArcStyle);