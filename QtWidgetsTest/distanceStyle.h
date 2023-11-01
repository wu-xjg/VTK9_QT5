#include <vtkAutoInit.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkRendererCollection.h> 
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>  
#include <vtkProperty.h>

#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <QDebug>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkBoxWidget2.h>

#include <vtkTransform.h>
#include <vtkBoxRepresentation.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceWidget.h>

class distanceStyle : public vtkInteractorStyleTrackballCamera
{
private:

public:
	static distanceStyle* New();
	

	//�������Ԫ���ݣ��������������й���,�������������������ͻ�����
	vtkTypeMacro(distanceStyle, vtkInteractorStyleTrackballCamera);
	distanceStyle() {

	}
	virtual ~distanceStyle() {

	}
	virtual void OnLeftButtonDown() {
		vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
		//ָ����Ⱦ���ڽ�����,�������û��¼�
		distanceWidget->SetInteractor(this->Interactor);
		//��Ҫʱʹ�ù۲���/����ģʽ�����ص�����(�˴�û��)
		//�������α��ʵ�塣��SetRepresentation()���¼���Widget��������
		//����ʹ��WidgetĬ�ϵļ��α��ʵ��
		distanceWidget->CreateDefaultRepresentation();
		static_cast<vtkDistanceRepresentation*> (distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g px");
		//����Widget
		distanceWidget->On();
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
		this->Interactor->GetRenderWindow()->Render();
	


	}


};
vtkStandardNewMacro(distanceStyle);