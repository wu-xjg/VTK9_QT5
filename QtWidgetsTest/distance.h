#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceWidget.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkExtractGeometry.h>
#include <vtkGeometryFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkVertexGlyphFilter.h>

class ExtractFaceStyle : public vtkInteractorStyleTrackballCamera {
public:
    static ExtractFaceStyle* New();
    vtkTypeMacro(ExtractFaceStyle, vtkInteractorStyleTrackballCamera);

    ExtractFaceStyle() {
        this->SelectedActor = nullptr;
        this->PolyData = vtkSmartPointer<vtkPolyData>::New();
        this->ExtractedFace = vtkSmartPointer<vtkPolyData>::New();
        this->ExtractFilter = vtkSmartPointer<vtkExtractGeometry>::New();
    }

    virtual void OnRightButtonDown() {
        int* clickPos = this->GetInteractor()->GetEventPosition();

        // ����ʰȡ Actor
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.001);
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetCurrentRenderer());
        vtkActor* pickedActor = picker->GetActor();

        if (pickedActor) {
            // ����ɹ�ʰȡ�� Actor����¼����
            this->SelectedActor = pickedActor;

            // ��ȡ Actor ����
            //this->ExtractEdges();
            this->ExtractPoints();

            // ִ����������...
        }
        this->Interactor->GetRenderWindow()->Render();
        qDebug() << "click";
    }
    void ExtractPoints() {
        if (this->SelectedActor) {
            // ʹ��vtkVertexGlyphFilter��Actor�ĵ�����ת��Ϊ����ʾ����ʽ
            vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
            vertexGlyphFilter->SetInputData(this->SelectedActor->GetMapper()->GetInput());
            vertexGlyphFilter->Update();

            // ����һ������Ч��������
            vtkSmartPointer<vtkProperty> highlightProperty = vtkSmartPointer<vtkProperty>::New();
            highlightProperty->SetColor(1.0, 0.0, 0.0);  // ���ø�������ɫ

            // ����һ��Actor����ʾ�����ĵ�
            vtkSmartPointer<vtkActor> highlightActor = vtkSmartPointer<vtkActor>::New();
            vtkSmartPointer<vtkPolyDataMapper> highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            highlightMapper->SetInputData(vertexGlyphFilter->GetOutput());

            // ���ø���Actor��Mapper��Property
            highlightActor->SetMapper(highlightMapper);
            highlightActor->SetProperty(highlightProperty);

            highlightActor->GetProperty()->SetPointSize(10);  // ���ø���Actor�ĵ��С
            highlightActor->GetProperty()->SetColor(1.0, 0.0, 0.0);  // ���ø���Actor�ĵ���ɫ

            // ɾ��ԭʼActor
            this->GetCurrentRenderer()->RemoveActor(this->SelectedActor);

            // ������Actor��ӵ���ǰ��Ⱦ��
            this->GetCurrentRenderer()->AddActor(highlightActor);

            // ִ����������...
        }
    }
    //void ExtractEdges() {
    //    if (this->SelectedActor) {
    //        // ʹ�� vtkFeatureEdges ���бߵ���ȡ
    //        vtkSmartPointer<vtkFeatureEdges> edgeExtractor = vtkSmartPointer<vtkFeatureEdges>::New();
    //        edgeExtractor->SetInputData(this->SelectedActor->GetMapper()->GetInput());
    //        edgeExtractor->BoundaryEdgesOn(); // ����ȡ�߽��
    //        edgeExtractor->FeatureEdgesOff();  // �ر��������͵ı���ȡ
    //        edgeExtractor->ManifoldEdgesOff(); // �رն��ر���ȡ
    //        edgeExtractor->NonManifoldEdgesOff(); // �رշǶ��ر���ȡ
    //        edgeExtractor->Update();

    //        // ����һ������Ч��������
    //        vtkSmartPointer<vtkProperty> highlightProperty = vtkSmartPointer<vtkProperty>::New();
    //        highlightProperty->SetColor(1.0, 0.0, 0.0);  // ���ø�������ɫ

    //        // ����һ�� Actor ����ʾ�����ı�
    //        vtkSmartPointer<vtkActor> highlightActor = vtkSmartPointer<vtkActor>::New();
    //        vtkSmartPointer<vtkPolyDataMapper> highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //        highlightMapper->SetInputData(edgeExtractor->GetOutput());

    //        // ���ø��� Actor �� Mapper��Property
    //        highlightActor->SetMapper(highlightMapper);
    //        highlightActor->SetProperty(highlightProperty);

    //        // ɾ��ԭʼ�� Actor
    //        this->GetCurrentRenderer()->RemoveActor(this->SelectedActor);

    //        // �������� Actor ��ӵ���ǰ��Ⱦ��
    //        this->GetCurrentRenderer()->AddActor(highlightActor);

    //        // ִ����������...
    //    }
    //}


private:
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkPolyData> PolyData;
    vtkSmartPointer<vtkPolyData> ExtractedFace;
    vtkSmartPointer<vtkExtractGeometry> ExtractFilter;
};

vtkStandardNewMacro(ExtractFaceStyle);