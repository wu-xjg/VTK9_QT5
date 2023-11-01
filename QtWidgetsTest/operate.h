#include <vtkInteractorStyleTrackballActor.h>
//#include <InteractionProp.h>
#include "vtkAutoInit.h"
#include <QLabel>
#include <vtkWidgetRepresentation.h>



double actor_color[3];
double *actor_color_pointer;
double actor_color_after[3] = {1,1,1};
double worldPoint1[3] = {0,0,0};

class OperateStyle : public vtkInteractorStyleTrackballActor{
public:
    vtkSmartPointer<vtkActor> m_pickedActor;
    vtkSmartPointer<vtkActor> m_removedActor;
   
    static OperateStyle* New();
    vtkTypeMacro(OperateStyle, vtkInteractorStyleTrackballActor);
    vtkActor* getactor() {
        return ActorA;
    }
    virtual void OnLeftButtonDown()
    {
    
        vtkInteractorStyleTrackballActor::OnLeftButtonDown();
        int* clickPos = this->GetInteractor()->GetEventPosition();
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.001);
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetCurrentRenderer());
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(clickPos[0], clickPos[1], 0);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
        worldPoint1[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
        worldPoint1[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
        worldPoint1[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];

        
        ActorA = picker->GetActor();
        if (this->InteractionProp == this->ActorA)
        {
            qDebug() << "Picked ActorA.";  
            if (this->ActorA) {
                actor_color_pointer = this->ActorA->GetProperty()->GetColor();
                actor_color[0] = actor_color_pointer[0];
                actor_color[1] = actor_color_pointer[1];
                actor_color[2] = actor_color_pointer[2];
                qDebug() <<"before:" << actor_color[0] << actor_color[1] << actor_color[2];
            }
            else {
                //actor_color = (0,0,0);
            }
         
            if (this->ActorA && actor_color_after) {
                this->ActorA->GetMapper()->ScalarVisibilityOff();
                this->ActorA->GetProperty()->SetColor(actor_color_after);
                qDebug() <<"after:"<< actor_color_after[0] << actor_color_after[1] << actor_color_after[2];
            }
            else {
                /*actor_color = (0,0,0);*/
            }
                       
        }  

        
    /*    else if (this->InteractionProp == this->ActorB)
        {
            std::cout << "Picked ActorB." << std::endl;
        }*/
    }
  
    vtkActor* ActorA;

   
    virtual void OnKeyPress() override
    {
        
      
        m_pickedActor = ActorA;
        std::string key = this->GetInteractor()->GetKeySym();
       
        if (key == "BackSpace" && m_pickedActor)
        {
            this->GetCurrentRenderer()->RemoveActor(m_pickedActor);
            this->GetCurrentRenderer()->Render();
            m_pickedActor = nullptr;
        }
      
        else if (key == "z" && this->GetInteractor()->GetControlKey() && m_pickedActor)
        {
            // Undo the deletion of ActorA by adding it back to the renderer
            this->GetCurrentRenderer()->AddActor(m_pickedActor);
            this->GetCurrentRenderer()->Render();
            m_pickedActor = nullptr;
           
        }
        


        if (key == "Up")
        {
            // 向上平移
            double translation[3] = { 0, 0.1, 0 };
            this->ActorA->AddPosition(translation);
        }
        else if (key == "Down")
        {
            // 向下平移
            double translation[3] = { 0, -0.1, 0 };
            this->ActorA->AddPosition(translation);
        }
        else if (key == "Left")
        {
            // 向左平移
            double translation[3] = { -0.1, 0, 0 };
            this->ActorA->AddPosition(translation);
        }
        else if (key == "Right")
        {
            // 向右平移
            double translation[3] = { 0.1, 0, 0 };
            this->ActorA->AddPosition(translation);
        }
        else if (key == "1")
        {
            // 左旋
            double rotation = -10;
            this->ActorA->AddOrientation(0, 0, rotation);
        }
        else if (key == "2")
        {
            // 右旋
            double rotation = 10;
            this->ActorA->AddOrientation(0, 0, rotation);
        }

      
        
        this->Interactor->GetRenderWindow()->Render();
    }
};
vtkStandardNewMacro(OperateStyle);