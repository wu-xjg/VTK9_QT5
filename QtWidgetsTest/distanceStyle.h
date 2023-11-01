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
	

	//定义类的元数据，并将其与基类进行关联,接受两个参数：类名和基类名
	vtkTypeMacro(distanceStyle, vtkInteractorStyleTrackballCamera);
	distanceStyle() {

	}
	virtual ~distanceStyle() {

	}
	virtual void OnLeftButtonDown() {
		vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
		//指定渲染窗口交互器,来监听用户事件
		distanceWidget->SetInteractor(this->Interactor);
		//必要时使用观察者/命令模式创建回调函数(此处没用)
		//创建几何表达实体。用SetRepresentation()把事件与Widget关联起来
		//或者使用Widget默认的几何表达实体
		distanceWidget->CreateDefaultRepresentation();
		static_cast<vtkDistanceRepresentation*> (distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g px");
		//激活Widget
		distanceWidget->On();
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
		this->Interactor->GetRenderWindow()->Render();
	


	}


};
vtkStandardNewMacro(distanceStyle);