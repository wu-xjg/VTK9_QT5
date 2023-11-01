
#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>
#include <QVTKOpenGLNativeWidget.h>
#include "QtWidgetsTest.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsTest w;
	

    w.show();
    return a.exec();
}
