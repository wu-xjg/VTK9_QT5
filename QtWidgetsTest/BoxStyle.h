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

class BoxStyle : public vtkInteractorStyleTrackballCamera
{
private:

public:
	static BoxStyle* New();
	vtkSmartPointer<vtkBoxWidget2> boxWidget;
	vtkSmartPointer<vtkActor> actor1;
	//定义类的元数据，并将其与基类进行关联,接受两个参数：类名和基类名
	vtkTypeMacro(BoxStyle, vtkInteractorStyleTrackballCamera);
	BoxStyle() {
	
	}
	virtual ~BoxStyle() {
	
	}
	virtual void OnRightButtonDown() {
		int* pos = this->GetInteractor()->GetEventPosition();

		vtkNew<vtkCellPicker> picker;
		picker->SetTolerance(0.01);


		// Pick from this location.
		picker->Pick(pos[0], pos[1], 0, this->GetCurrentRenderer());
		actor1 = vtkSmartPointer<vtkActor>::New();
		actor1 = picker->GetActor();
		qDebug() << "123";
		if (actor1) {
			qDebug() << "1235";
			boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
			boxWidget->SetInteractor(this->Interactor);
			boxWidget->GetRepresentation()->SetPlaceFactor(1); 
			boxWidget->GetRepresentation()->PlaceWidget(actor1->GetBounds());
			boxWidget->RotationEnabledOn();
			boxWidget->ScalingEnabledOn();
			boxWidget->TranslationEnabledOn();
			
			boxWidget->On();
			

			vtkNew<vtkTransform> tra;
			dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation())->GetTransform(tra);
			
			// Apply boxWidget's transform to actor1
			vtkSmartPointer<vtkTransform> actorTransform = vtkSmartPointer<vtkTransform>::New();
			if (actor1->GetUserTransform()) {
				actorTransform->DeepCopy(actor1->GetUserTransform());
				qDebug() << "12";
			}
			actorTransform->Concatenate(tra);
			actor1->SetUserTransform(actorTransform);
			dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation())->SetTransform(actorTransform);
			/*this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor1);*/
			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
			this->Interactor->GetRenderWindow()->Render();
		}
		
		
	}

	virtual void OnLeftButtonDown() {
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

		// Update actor1's transform to match boxWidget's transform
		if (boxWidget) {
			vtkNew<vtkTransform> tra;
			dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation())->GetTransform(tra);

			vtkSmartPointer<vtkTransform> actorTransform = vtkSmartPointer<vtkTransform>::New();
			actorTransform->DeepCopy(tra);
			actor1->SetUserTransform(actorTransform);

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
			this->Interactor->GetRenderWindow()->Render();
		}
	}


};
vtkStandardNewMacro(BoxStyle);