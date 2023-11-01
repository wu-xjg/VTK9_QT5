
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsTest.h"
#include <vtkTexture.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkImageViewer2.h>
#include <vtkJPEGReader.h>
#include <vtkOrientationMarkerWidget.h>
#include <QToolBar>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <QDebug>

class QtWidgetsTest : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsTest(QWidget *parent = nullptr);
    ~QtWidgetsTest();
    
    //QLabel* getLabel() { return m_label; }
public slots:
    void drawLine();
    void drawArc();
    void drawCube();
    void pickActor();
    void backColor();
    void showAxis();
    void clearActor();
    void openFile();
    void openVTK();
    void saveFile();
    void saveLine();
    void saveArc();
    void saveCube();
    void pickDot();
    void cellPicker();
    void operate();
    void distance();
    void mouseReleaseEvent(QMouseEvent* event);

    

private:
    QLabel* m_label;
    Ui::QtWidgetsTestClass ui;
    int number = 0;
    bool isLine;
    bool isArc;
    bool isCube;
    vtkSmartPointer<vtkRenderer> _renderer;
    vtkSmartPointer<vtkImageViewer2> _viewer;
    vtkSmartPointer<vtkRenderWindow> _renderWindow;
    QVector<int> _actorList;
    QVTKOpenGLNativeWidget* widget;
    bool isDock = true;
    double color_rgb[3];
    vtkSmartPointer<vtkJPEGReader> reader;
    vtkOrientationMarkerWidget* Markwidget=NULL;
    QToolBar* tbar;

    vtkSmartPointer<vtkActor> m_previousActor;
    vtkSmartPointer<vtkPoints> m_points;
    vtkSmartPointer<vtkCellArray> m_cellArrays;
    vtkSmartPointer<vtkUnstructuredGrid> m_polyData;

    //vtkSmartPointer <vtkDataSetReader> Reader;
    vtkSmartPointer <vtkDataSetAttributes> DatasetAttributesPoint;
    vtkSmartPointer <vtkDataSetAttributes> DatasetAttributesCell;
};
