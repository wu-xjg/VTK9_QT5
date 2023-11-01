#include <vtkAutoInit.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h> 
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>  
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <QDebug>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
//vtkActor* picked;

class propPicker : public vtkInteractorStyleTrackballCamera
{
private:

public:
	vtkActor* LastPickedActor;
	vtkProperty* LastPickedProperty;
	double* color;
	double* pos;
	static propPicker* New();
	//定义类的元数据，并将其与基类进行关联,接受两个参数：类名和基类名
	vtkTypeMacro(propPicker, vtkInteractorStyleTrackballCamera);
	propPicker() {
		LastPickedActor = NULL;
		LastPickedProperty = vtkProperty::New();

	}
	virtual ~propPicker() {
		LastPickedProperty->Delete();
	}
	virtual void OnRightButtonDown() {
		int* clickPos = this->GetInteractor()->GetEventPosition();
	
		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();

		picker->Pick(clickPos[0], clickPos[1], 0, this->GetCurrentRenderer());

		pos = picker->GetPickPosition();
		if (this->LastPickedActor) {
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
			qDebug() << this->LastPickedProperty<<1111;
		}
		this->LastPickedActor = picker->GetActor();

		if (this->LastPickedActor) {
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			qDebug() << this->LastPickedActor->GetProperty()<<2222;
			this->LastPickedActor->GetProperty()->SetColor(1, 1, 0);
			this->LastPickedActor->GetProperty()->SetDiffuse(1);
			this->LastPickedActor->GetProperty()->SetSpecular(0);
			/*picked->ShallowCopy(this->LastPickedActor);*/
			color = this->LastPickedActor->GetProperty()->GetColor();
			pos = this->LastPickedActor->GetPosition();
			/*qDebug() << color[0] << color[1] << color[2];*/
			//qDebug() << pos[0] << pos[1] << pos[2];
		}
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}


};
vtkStandardNewMacro(propPicker);