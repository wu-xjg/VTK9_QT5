#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL2)
//VTK_MODULE_INIT(vtkRenderingFreeType)
//VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
//this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
#include <vtkRendererCollection.h> 
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>  //vtkStandardNewMacro();
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <QDebug>
class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	
	static PointPickerInteractorStyle* New();
	//定义类的元数据，并将其与基类进行关联,接受两个参数：类名和基类名
	vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);
	virtual void OnLeftButtonDown()
	{
		//打印鼠标左键像素位置
		qDebug() << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
			<< " " << this->Interactor->GetEventPosition()[1];
		//注册拾取点函数
		this->Interactor->GetPicker()->Pick(
			this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1],
			0,  // always zero.
			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()

		);
		//打印拾取点空间位置
		double picked[3];
		this->Interactor->GetPicker()->GetPickPosition(picked);
		qDebug() << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2];

		//对拾取点进行标记
		vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->Update();
		sphereSource->SetRadius(0.1);

		vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(sphereSource->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->SetPosition(picked);
		actor->SetScale(0.1);
		actor->GetProperty()->SetColor(1.0, 0.0, 0.0 );
		//this->Interactor->GetPicker()->AddPickList(actor);
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
		
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
};
vtkStandardNewMacro(PointPickerInteractorStyle);
