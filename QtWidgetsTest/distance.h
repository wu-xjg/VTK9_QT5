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

        // 尝试拾取 Actor
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.001);
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetCurrentRenderer());
        vtkActor* pickedActor = picker->GetActor();

        if (pickedActor) {
            // 如果成功拾取到 Actor，记录下来
            this->SelectedActor = pickedActor;

            // 提取 Actor 的面
            //this->ExtractEdges();
            this->ExtractPoints();

            // 执行其他操作...
        }
        this->Interactor->GetRenderWindow()->Render();
        qDebug() << "click";
    }
    void ExtractPoints() {
        if (this->SelectedActor) {
            // 使用vtkVertexGlyphFilter将Actor的点数据转换为可显示的形式
            vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
            vertexGlyphFilter->SetInputData(this->SelectedActor->GetMapper()->GetInput());
            vertexGlyphFilter->Update();

            // 创建一个高亮效果的属性
            vtkSmartPointer<vtkProperty> highlightProperty = vtkSmartPointer<vtkProperty>::New();
            highlightProperty->SetColor(1.0, 0.0, 0.0);  // 设置高亮的颜色

            // 创建一个Actor来显示高亮的点
            vtkSmartPointer<vtkActor> highlightActor = vtkSmartPointer<vtkActor>::New();
            vtkSmartPointer<vtkPolyDataMapper> highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            highlightMapper->SetInputData(vertexGlyphFilter->GetOutput());

            // 设置高亮Actor的Mapper和Property
            highlightActor->SetMapper(highlightMapper);
            highlightActor->SetProperty(highlightProperty);

            highlightActor->GetProperty()->SetPointSize(10);  // 设置高亮Actor的点大小
            highlightActor->GetProperty()->SetColor(1.0, 0.0, 0.0);  // 设置高亮Actor的点颜色

            // 删除原始Actor
            this->GetCurrentRenderer()->RemoveActor(this->SelectedActor);

            // 将高亮Actor添加到当前渲染器
            this->GetCurrentRenderer()->AddActor(highlightActor);

            // 执行其他操作...
        }
    }
    //void ExtractEdges() {
    //    if (this->SelectedActor) {
    //        // 使用 vtkFeatureEdges 进行边的提取
    //        vtkSmartPointer<vtkFeatureEdges> edgeExtractor = vtkSmartPointer<vtkFeatureEdges>::New();
    //        edgeExtractor->SetInputData(this->SelectedActor->GetMapper()->GetInput());
    //        edgeExtractor->BoundaryEdgesOn(); // 仅提取边界边
    //        edgeExtractor->FeatureEdgesOff();  // 关闭其他类型的边提取
    //        edgeExtractor->ManifoldEdgesOff(); // 关闭多重边提取
    //        edgeExtractor->NonManifoldEdgesOff(); // 关闭非多重边提取
    //        edgeExtractor->Update();

    //        // 创建一个高亮效果的属性
    //        vtkSmartPointer<vtkProperty> highlightProperty = vtkSmartPointer<vtkProperty>::New();
    //        highlightProperty->SetColor(1.0, 0.0, 0.0);  // 设置高亮的颜色

    //        // 创建一个 Actor 来显示高亮的边
    //        vtkSmartPointer<vtkActor> highlightActor = vtkSmartPointer<vtkActor>::New();
    //        vtkSmartPointer<vtkPolyDataMapper> highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //        highlightMapper->SetInputData(edgeExtractor->GetOutput());

    //        // 设置高亮 Actor 的 Mapper和Property
    //        highlightActor->SetMapper(highlightMapper);
    //        highlightActor->SetProperty(highlightProperty);

    //        // 删除原始的 Actor
    //        this->GetCurrentRenderer()->RemoveActor(this->SelectedActor);

    //        // 将高亮的 Actor 添加到当前渲染器
    //        this->GetCurrentRenderer()->AddActor(highlightActor);

    //        // 执行其他操作...
    //    }
    //}


private:
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkPolyData> PolyData;
    vtkSmartPointer<vtkPolyData> ExtractedFace;
    vtkSmartPointer<vtkExtractGeometry> ExtractFilter;
};

vtkStandardNewMacro(ExtractFaceStyle);