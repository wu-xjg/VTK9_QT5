#include "vtkAutoInit.h"


#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkCellPicker.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkRendererCollection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>
#include <vtkNamedColors.h>

#include <vtkUnstructuredGrid.h>
#include <vtkNamedColors.h>


class CellPicker : public vtkInteractorStyleTrackballCamera
{
public:
	static CellPicker* New();
	vtkActor* LastPickedActor;
	vtkProperty* LastPickedProperty;


	CellPicker()
	{
		LastPickedActor = NULL;
		LastPickedProperty = vtkProperty::New();
		//selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		//selectedActor = vtkSmartPointer<vtkActor>::New();
	}
	virtual ~CellPicker() {
		LastPickedProperty->Delete();
	}
	virtual void OnLeftButtonDown() override
	{
		vtkNew<vtkNamedColors> colors;

		int* pos = this->GetInteractor()->GetEventPosition();

		vtkNew<vtkCellPicker> picker;
		picker->SetTolerance(0.01);

		// Pick from this location.
		picker->Pick(pos[0], pos[1], 0, this->GetCurrentRenderer());

		if (this->LastPickedActor) {
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
			qDebug() << this->LastPickedProperty << 1111;
		}
		this->LastPickedActor = picker->GetActor();
	
		if (this->LastPickedActor) {
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			qDebug() << this->LastPickedActor->GetProperty() << 2222;
			this->LastPickedActor->GetProperty()->SetColor(1, 0, 0);
			this->LastPickedActor->GetProperty()->SetDiffuse(1);
			this->LastPickedActor->GetProperty()->SetSpecular(0);
			/*picked->ShallowCopy(this->LastPickedActor);*/
	
		}

	
		qDebug() << "Cell id is: " << picker->GetCellId();

		//if (picker->GetCellId() != -1)
		//{

			//qDebug()<< "Pick position is: (" << worldPosition[0] << ", "<< worldPosition[1] << ", " << worldPosition[2] << ")" ;
			////选中索引号
			//vtkNew<vtkIdTypeArray> ids;
			//ids->SetNumberOfComponents(1);
			//ids->InsertNextValue(picker->GetCellId());
			////提取数据类型
			//vtkNew<vtkSelectionNode> selectionNode;
			//selectionNode->SetFieldType(vtkSelectionNode::CELL);
			//selectionNode->SetContentType(vtkSelectionNode::INDICES);
			//selectionNode->SetSelectionList(ids);

			//vtkNew<vtkSelection> selection;
			//selection->AddNode(selectionNode);
			////数据提取
			//vtkNew<vtkExtractSelection> extractSelection;
			//extractSelection->SetInputData(0, polyData);
			//extractSelection->SetInputData(1,selection);

			//extractSelection->Update();

			//// In selection
			//vtkNew<vtkUnstructuredGrid> selected;
			//selected->ShallowCopy(extractSelection->GetOutput());

			//qDebug() << "Number of points in the selection: "<< selected->GetNumberOfPoints() ;
			//qDebug() << "Number of cells in the selection : "<< selected->GetNumberOfCells() ;

			//selectedMapper->SetInputData((vtkDataSet*)extractSelection->GetOutput());
			//selectedActor->SetMapper(selectedMapper);
			//selectedActor->GetProperty()->EdgeVisibilityOn();
			//selectedActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
			/*selectedActor->GetProperty()->SetColor(1,0,0);
			selectedActor->GetProperty()->SetLineWidth(2);
			
			this->Interactor->GetRenderWindow()
				->GetRenderers()
				->GetFirstRenderer()
				->AddActor(selectedActor);
			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
			this->Interactor->GetRenderWindow()->Render();
		}*/
		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	
	//vtkSmartPointer<vtkPolyData> polyData;
	//vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	//vtkSmartPointer<vtkActor> selectedActor;
};

vtkStandardNewMacro(CellPicker);
