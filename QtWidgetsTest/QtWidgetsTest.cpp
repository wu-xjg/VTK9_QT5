#include "QtWidgetsTest.h"
#include <QDebug.h>
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
#include <vtkLineSource.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkCubeSource.h>
#include "propPicker.h"
#include "lineStyle.h"
#include "cubeStyle.h"
#include "arcStyle.h"
#include <vtkDistanceWidget.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <QDir>
#include <QFileDialog>
#include <vtkJPEGWriter.h>
#include <string.h>
#include <vtkDataSetReader.h>
#include "cellPicker.h"
#include "operate.h"
#include <vtkUnstructuredGridReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkPolyDataWriter.h>
#include "PointStyle.h"
#include "BoxStyle.h"
#include <vtkDistanceRepresentation.h >
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation3D.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkLookupTable.h>
#include <vtkTextRepresentation.h>
#include <vtkTextWidget.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPolyDataReader.h>
#include "MyVtkWidget.h"
#include "distance.h"
QtWidgetsTest::QtWidgetsTest(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	ui.dockWidget->setHidden(true);
	ui.horizontalSlider->setTracking(false);
	ui.horizontalSlider_2->setTracking(false);
	ui.horizontalSlider_3->setTracking(false);
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
	widget = new QVTKOpenGLNativeWidget();
	widget->resize(400, 400);
	ui.verticalLayout->addWidget(widget);


	_viewer = vtkSmartPointer< vtkImageViewer2 >::New();
	tbar = new QToolBar(this);
	addToolBar(Qt::LeftToolBarArea, tbar);
	tbar->setMovable(false);
	//tbar->addAction(ui.action1);
	tbar->addSeparator();
	//m_label = new QLabel();
	//m_label->setText("Hello, World!");
	//m_label->setGeometry(10, 10, 100, 30);
	


	_renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkCamera> camera = _renderer->GetActiveCamera();
	//camera->SetPosition(1,0,0);
	//camera->SetFocalPoint(0,0,0);
	// ������Ҫ���������������
	//_renderer->ResetCamera();  // ���������������Ӧ���ӿ�


	widget->renderWindow()->AddRenderer(_renderer);
	connect(ui.action1, &QAction::triggered, this, &QtWidgetsTest::drawLine);
	connect(ui.action2, &QAction::triggered, this, &QtWidgetsTest::drawArc);
	connect(ui.action3, &QAction::triggered, this, &QtWidgetsTest::drawCube);
	connect(ui.action4, &QAction::triggered, this, &QtWidgetsTest::pickActor);
	connect(ui.action5, &QAction::triggered, this, &QtWidgetsTest::backColor);
	connect(ui.action6, &QAction::triggered, this, &QtWidgetsTest::showAxis);
	connect(ui.action1_2, &QAction::triggered, this, &QtWidgetsTest::openFile);
	connect(ui.action2_2, &QAction::triggered, this, &QtWidgetsTest::saveFile);
	connect(ui.action3_2, &QAction::triggered, this, &QtWidgetsTest::openVTK);
	connect(ui.action4_2, &QAction::triggered, this, &QtWidgetsTest::saveLine);
	connect(ui.action5_2, &QAction::triggered, this, &QtWidgetsTest::saveArc);
	connect(ui.action6_2, &QAction::triggered, this, &QtWidgetsTest::saveCube);
	connect(ui.action7, &QAction::triggered, this, &QtWidgetsTest::clearActor);
	connect(ui.action8, &QAction::triggered, this, &QtWidgetsTest::cellPicker);
	connect(ui.action9, &QAction::triggered, this, &QtWidgetsTest::operate);
	connect(ui.action10, &QAction::triggered, this, &QtWidgetsTest::pickDot);
	connect(ui.action11, &QAction::triggered, this, [=] {
		vtkSmartPointer<BoxStyle> style = vtkSmartPointer<BoxStyle>::New();
		style->SetCurrentRenderer(_renderer);
		widget->interactor()->SetInteractorStyle(style);
		widget->interactor();
		widget->renderWindow()->Render();
		

		});
	//connect(ui.action12, &QAction::triggered, this, [=] {
		//vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
		////ָ����Ⱦ���ڽ�����,�������û��¼�
		//distanceWidget->SetInteractor(widget->interactor());
		////�������α��ʵ�塣��SetRepresentation()���¼���Widget��������
		//distanceWidget->CreateDefaultRepresentation();
		//static_cast<vtkDistanceRepresentation*> (distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g px");
		////����Widget
		//distanceWidget->On();
		//widget->interactor();
		//widget->renderWindow()->Render();

		//});
	connect(ui.action12, &QAction::triggered, this, [=] {

	/*	vtkSmartPointer< vtkLookupTable > lut = vtkSmartPointer< vtkLookupTable >::New();
		lut->Build();
		vtkSmartPointer< vtkScalarBarActor > scalarBarActor = vtkSmartPointer< vtkScalarBarActor >::New();
		scalarBarActor->SetOrientationToHorizontal();
		scalarBarActor->SetLookupTable(lut);

		vtkSmartPointer< vtkScalarBarWidget > scalarBarWidget = vtkSmartPointer< vtkScalarBarWidget >::New();
		scalarBarWidget->SetInteractor(widget->interactor());
		scalarBarWidget->SetScalarBarActor(scalarBarActor);
		scalarBarWidget->On();*/

		vtkActorCollection* actorCollection = _renderer->GetActors();
		int num = actorCollection->GetNumberOfItems();
		//��������Ƚ���Ҫ������getNextActor��û���õ���ȷ��actor
		actorCollection->InitTraversal();
		for (int i = 0; i < num; ++i)
		{	
			vtkActor* actor = actorCollection->GetNextActor();
			if(i==num-1)		
			_renderer->RemoveActor(actor);
		}
		
		widget->renderWindow()->Render();
		});
	connect(ui.action13, &QAction::triggered, this, &QtWidgetsTest::distance);

	color_rgb[0] = 0.0;
	color_rgb[1] = 0.0;
	color_rgb[2] = 0.0;

	connect(ui.action9, &QAction::triggered, this, [=] {
		isDock = !isDock;
		ui.dockWidget->setHidden(isDock);
		});
	connect(ui.horizontalSlider,  &QSlider::valueChanged, [=](int value) {
		qDebug() << "slider1 value:" << QString::number(value/100.0);
			color_rgb[0] = value / 100.0;
			
		});
	connect(ui.horizontalSlider_2, &QSlider::valueChanged, [=](int value) {
		qDebug() << "slider2 value:" << QString::number(value/100.0);
			color_rgb[1] = value / 100.0;
			 
		});
	connect(ui.horizontalSlider_3, &QSlider::valueChanged, [=](int value) {
		qDebug() << "slider3 value:" << QString::number(value/100.0);
			color_rgb[2] = value / 100.0;
			
		});
	connect(ui.pushButton, &QPushButton::clicked, [=]() {
		actor_color_after[0] = color_rgb[0];
		actor_color_after[1] = color_rgb[1];
		actor_color_after[2] = color_rgb[2];
		_renderer->Render();
		widget->interactor()->GetRenderWindow()->Render();
		});
	connect(ui.pushButton_2, &QPushButton::clicked, [=]() {
		qDebug() << "Picked value: " << worldPoint1[0] << " " << worldPoint1[1] << " " << worldPoint1[2];
		ui.label_5->setText(QString::number(worldPoint1[0]));
		ui.label_6->setText(QString::number(worldPoint1[1]));
		ui.label_7->setText(QString::number(worldPoint1[2]));
		
		_renderer->Render();
		widget->interactor()->GetRenderWindow()->Render();
		});
	
	
}


vtkSmartPointer<LineStyle> style_line;
vtkSmartPointer<ArcStyle> style_arc;
vtkSmartPointer<CubeStyle> style_cube;
//connect(style_line, &LineStyle::mySignal(), this, &QtWidgetsTest::operate);
QtWidgetsTest::~QtWidgetsTest()
{}

void QtWidgetsTest::drawLine() {
	isLine =true ;
	isArc = false;
	isCube = false;
	//vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	//lineSource->SetPoint1(0,0,0);
	//lineSource->SetPoint2(1,1,1);
	//lineSource->Update();
	//
	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(lineSource->GetOutput());
	//// ����actor
	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);
	//_renderer->AddActor(actor);
	
	style_line = vtkSmartPointer<LineStyle>::New();
	style_line->SetCurrentRenderer(_renderer);

	widget->interactor()->SetInteractorStyle(style_line);
	qDebug() << "update";
	for (int i = 0; i < line_number - old_line_number; i++) {

		QLabel* state_msg = new QLabel(this);
		state_msg->setMinimumSize(state_msg->sizeHint()); //����label����
		state_msg->setAlignment(Qt::AlignHCenter);        //����label����
		state_msg->setText(QString::fromLocal8Bit("ֱ��_") + QString::number(old_line_number + i));
		tbar->addWidget(state_msg);
		/*QPushButton* state_btn = new QPushButton(this);
		state_btn->setText(QString::fromLocal8Bit("ɾ��"));*/
		//state_btn->setLayoutDirection(Qt::RightToLeft);
		//tbar->addWidget(state_btn);
		tbar->addSeparator();
		
	}
	
	
	old_line_number = line_number;
	//widget->renderWindow()->SetSize(800,600);

	widget->interactor();
	widget->renderWindow()->Render();
	
}

void QtWidgetsTest::drawArc() {
	isLine = false;
	isArc =true ;
	isCube = false;
	//double circleCenter[3] = { 0, 0, 0 };
	//double l = 0.001;
	//double r = 1;
	//double arf = 1;
	//double theta = 2 * asin(l / (2 * r));
	//double M_PI = 3.14159265;
	////Բ���ϵ����Ŀ2* M_PI
	//long long pntNum = (long long)(arf * M_PI / theta);
	////����Բ���ϵĵ�
	//vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//for (long long i = 0; i < pntNum; i++)
	//{
	//	points->InsertPoint(i, circleCenter[0] + r * cos(i * theta), circleCenter[1] + r * sin(i * theta), 0);
	//}

	//vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
	////pntNum+1
	//polyLine->GetPointIds()->SetNumberOfIds(pntNum);
	//for (long long i = 0; i < pntNum; i++)
	//{
	//	polyLine->GetPointIds()->SetId(i, i); //��һ��������polyline�еĵ��������ڶ���������point�еĵ�����
	//}
	//vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	//cells->InsertNextCell(polyLine);
	//vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

	//polyData->SetPoints(points);
	//polyData->SetLines(cells);
	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(polyData);

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);
	//
	//_renderer->AddActor(actor);
	style_arc = vtkSmartPointer<ArcStyle>::New();
	style_arc->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style_arc);

	for (int i = 0; i < arc_number - old_arc_number; i++) {

		QLabel* state_msg = new QLabel(this);
		state_msg->setMinimumSize(state_msg->sizeHint()); //����label����
		state_msg->setAlignment(Qt::AlignHCenter);        //����label����
		state_msg->setText(QString::fromLocal8Bit("Բ��_") + QString::number(old_arc_number+i));
		tbar->addWidget(state_msg);
		tbar->addSeparator();
		//qDebug() << "i";
	}
	old_arc_number = arc_number;
	widget->interactor();
	widget->renderWindow()->Render();
}

void QtWidgetsTest::drawCube() {
	isLine = false;
	isArc = false;
	isCube = true;
	
	/*int a = 1;
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	cube->SetXLength(a);
	cube->SetYLength(a);
	cube->SetZLength(a);
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cube->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	_renderer->AddActor(actor);*/

	style_cube = vtkSmartPointer<CubeStyle>::New();
	style_cube->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style_cube);
	
	for (int i = 0; i < cube_number- old_cube_number; i++) {

		QLabel* state_msg = new QLabel(this);
		state_msg->setMinimumSize(state_msg->sizeHint()); //����label����
		state_msg->setAlignment(Qt::AlignHCenter);        //����label����
		state_msg->setText(QString::fromLocal8Bit("������_") + QString::number(old_cube_number+i));
		tbar->addWidget(state_msg);
		tbar->addSeparator();
		/*qDebug() << "i";*/
	}
	old_cube_number = cube_number;
	widget->interactor();
	widget->renderWindow()->Render();
}

void QtWidgetsTest::pickActor() {
	vtkSmartPointer<propPicker> style = vtkSmartPointer<propPicker>::New();
	style->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style);
	widget->interactor();
	widget->renderWindow()->Render();
	
}
void QtWidgetsTest::backColor() {
	double r, g, b;
	r = vtkMath::Random(.4, 1.0);
	g = vtkMath::Random(.4, 1.0);
	b = vtkMath::Random(.4, 1.0);
	_renderer->SetBackground(r, g, b);
	widget->renderWindow()->Render();

}
void QtWidgetsTest::showAxis() {
	/*vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	style->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style);*/
	//double pos1[3];
	//double pos2[3];
	//vtkSmartPointer<vtkDistanceWidget> distance = vtkSmartPointer<vtkDistanceWidget>::New();
	//
	//distance->SetInteractor(widget->interactor());
	//distance->CreateDefaultRepresentation();
	///*static_cast<vtkDistanceRepresentation*> (distance->GetRepresentation())->SetLabelFormat("%-#6.3g px");*/

	//distance->On();
	//widget->interactor();
	//
	//widget->renderWindow()->Render();
	vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
	axesActor->SetAxisLabels(1);

	//vtkSmartPointer<vtkOrientationMarkerWidget> Markwidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	Markwidget= vtkOrientationMarkerWidget::New();
	Markwidget->SetOrientationMarker(axesActor);
	Markwidget->SetInteractor(widget->interactor());

	Markwidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	Markwidget->SetEnabled(1);
	Markwidget->InteractiveOn();

	/*xAxisCaptionActor->SetWidth(0.5);
	xAxisCaptionActor->SetHeight(0.2);*/
	/*	_renderer->AddActor(axesActor);*/
	widget->interactor();
	widget->renderWindow()->Render();


		//vtkActorCollection* actorCollection = _renderer->GetActors();
		//int num = actorCollection->GetNumberOfItems();
		////��������Ƚ���Ҫ������getNextActor��û���õ���ȷ��actor
		//actorCollection->InitTraversal();
		//for (int i = 0; i < num; ++i)
		//{
		//	vtkActor* actor = actorCollection->GetNextActor();	
		//	qDebug() << actor;
		//	_renderer->RemoveActor(actor);
		//}
		//widget->interactor();
		//widget->renderWindow()->Render();
	
	
}
void QtWidgetsTest::clearActor()
{
	vtkActorCollection* actorCollection = _renderer->GetActors();
	int num = actorCollection->GetNumberOfItems();
		//��������Ƚ���Ҫ������getNextActor��û���õ���ȷ��actor
	actorCollection->InitTraversal();
	for (int i = 0; i < num; ++i)
	{
		vtkActor* actor = actorCollection->GetNextActor();	
		qDebug() << actor;
		_renderer->RemoveActor(actor);
	}
	if(Markwidget)
		Markwidget->Delete();
	tbar->clear();
	line_number = 0;
	old_line_number = 0;
	old_arc_number = 0;
	arc_number = 0;
	cube_number = 0;
	old_cube_number =0;
	widget->interactor();
	widget->renderWindow()->Render();
}
void QtWidgetsTest::openFile() {
	// ����vtkPolyDataReader����
	
	qDebug() << number;
	// ����ÿ��vtkActor
	//vtkSmartPointer<vtkRenderer> renderer= vtkSmartPointer<vtkRenderer>::New();
	//widget->renderWindow()->AddRenderer(renderer);
	for (vtkIdType i = 0; i < number; i++)
	{
		vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
		// ����vtkPolyDataReader���ļ���
		reader->SetFileName(("actor_" + std::to_string(i) + ".vtk").c_str());

		// ��ȡvtkPolyData
		reader->Update();

		// ��ȡvtkPolyData
		vtkSmartPointer<vtkPolyData> polyData = reader->GetOutput();

		// ����vtkPolyDataMapper��vtkActor
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(polyData);
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		// ��vtkActor��ӵ���Ⱦ��
		_renderer->AddActor(actor);
		
	}
	widget->interactor();
	widget->renderWindow()->Render();
}

void QtWidgetsTest::openVTK()
{
	QString filter;
	filter = "vtk file (*.txt)";

	QDir dir;
	QString fileName = QFileDialog::getOpenFileName(this, QString(tr("open file")), dir.absolutePath(), filter);
	if (fileName.isEmpty() == true) return;

	// ֧�ִ�����·���Ķ�ȡ
	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();
	//vtkSmartPointer<vtkDataSetReader> Reader = vtkSmartPointer<vtkDataSetReader>::New();
	//Reader->SetFileName(fileName_str);

	//Reader->ReadAllScalarsOn();
	//Reader->ReadAllVectorsOn();
	//Reader->ReadAllNormalsOn();
	//Reader->ReadAllTensorsOn();
	//Reader->ReadAllColorScalarsOn();
	//Reader->ReadAllTCoordsOn();
	//Reader->ReadAllFieldsOn();
	//Reader->GetOutput()->Register(Reader);

	////���ȶ�������Point��Cell ������
	//DatasetAttributesPoint = vtkSmartPointer <vtkDataSetAttributes> ::New();
	//DatasetAttributesCell = vtkSmartPointer <vtkDataSetAttributes> ::New();
	//vtkSmartPointer<vtkDataSet> DataSet = vtkSmartPointer<vtkDataSet> ::New();
	//DataSet = Reader->GetOutput();
	//DatasetAttributesPoint = DataSet->GetAttributes(0);// ��ȡ����POINT��
	//DatasetAttributesCell = DataSet->GetAttributes(1);// CELL enum���Ϊ1��

	//// ��ȡ�ļ��и����������Ե�����
	//int numScalars = Reader->GetNumberOfScalarsInFile();
	//int numVectors = Reader->GetNumberOfVectorsInFile();
	//int numTensors = Reader->GetNumberOfTensorsInFile();

	////��ȡ�����������Ե����ƣ������д洢����
	//for (int i = 0; i < numScalars; i++) {
	//	const char* name = Reader->GetScalarsNameInFile(i);	
	//}
	//for (int i = 0; i < numVectors; i++) {
	//	const char* name = Reader->GetVectorsNameInFile(i);
	//}
	//for (int i = 0; i < numTensors; i++) {
	//	const char* name = Reader->GetTensorsNameInFile(i);
	//}
	//�������ƻ�ȡ����
	//vtkSmartPointer<vtkDataArray> dataSet;
	//dataSet = DatasetAttributesCell->GetScalars(name.c_str());
	//dataSet = DatasetAttributesPoint->GetVectors(name.c_str());
	//dataSet = DatasetAttributesPoint->GetTensors(name.c_str());

	vtkSmartPointer<vtkUnstructuredGridReader> MeshReader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	MeshReader->SetFileName(fileName_str);
	MeshReader->Update();
	vtkSmartPointer<vtkPoints> MeshPoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> MeshCells = vtkSmartPointer<vtkCellArray>::New();
	MeshPoints = MeshReader->GetOutput()->GetPoints();
	MeshCells = MeshReader->GetOutput()->GetCells();
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(MeshPoints);
	//polyData->SetCells(VTK_LINE,MeshCells);
	//polyData->SetPolys(MeshCells);
	polyData->SetLines(MeshCells);

	//// �������ԣ�����ʵ�����ѡ�����
	//polyData->GetPointData()->SetScalars(dataSet);
	//polyData->GetPointData()->SetVectors(dataSet);
	//polyData->GetPointData()->SetTensors(dataSet);

	//polyData->GetCellData()->SetScalars(dataSet);
	//polyData->GetCellData()->SetVectors(dataSet);
	//polyData->GetCellData()->SetTensors(dataSet);

	//// ���´�������Qt����ʾvtk Widget�Ĵ���

	vtkSmartPointer<vtkPolyDataMapper> vMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vMapper->SetInputData(polyData);
	vtkSmartPointer<vtkActor> vActor = vtkSmartPointer<vtkActor>::New();
	vActor->SetMapper(vMapper);
	_renderer->AddActor(vActor);


	widget->update();
	widget->interactor()->GetRenderWindow()->AddRenderer(_renderer);
	widget->interactor()->GetRenderWindow()->Render();
	widget->interactor();
	widget->renderWindow()->Render();

}

void QtWidgetsTest::saveFile() {
	//QString filter;
	////filter = "VTK file (*.vtk)";
	//filter = "image file (*.jpg *.jpeg)";

	//QDir dir;
	//QString fileName = QFileDialog::getSaveFileName(this, QString(tr("save file")), dir.absolutePath(), filter);
	//if (fileName.isEmpty() == true) return;

	//// ֧�ִ�����·���Ķ�ȡ
	//QByteArray ba = fileName.toLocal8Bit();
	//const char* fileName_str = ba.data();
	//vtkSmartPointer<vtkJPEGWriter> writer = vtkSmartPointer<vtkJPEGWriter>::New();
	//writer->SetFileName(fileName_str);
	//writer->SetInputConnection(reader->GetOutputPort());
	//writer->Write();
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	number = _renderer->GetActors()->GetNumberOfItems();
	qDebug() << number;
	// ����ÿ��vtkActor
	for (vtkIdType i = 0; i < _renderer->GetActors()->GetNumberOfItems(); i++)
	{
		vtkSmartPointer<vtkActor> actor = vtkActor::SafeDownCast(_renderer->GetActors()->GetItemAsObject(i));

		// ��ȡvtkActor��vtkMapper
		vtkSmartPointer<vtkMapper> mapper = actor->GetMapper();

		// ��ȡvtkMapper��vtkPolyData
		vtkSmartPointer<vtkPolyData> polyData = vtkPolyData::SafeDownCast(mapper->GetInputDataObject(0, 0));

		// ����vtkPolyDataWriter���������ݺ��ļ���
		writer->SetInputData(polyData);
		writer->SetFileName(("actor_" + std::to_string(i) + ".vtk").c_str());

		// ����vtkPolyData
		writer->Write();
	}


}

//void QtWidgetsTest::saveVTK(){
//	QString filter;
//	filter = "vtk file (*.txt)";
//
//	QDir dir;
//	QString fileName = QFileDialog::getSaveFileName(this, QString(tr("save file")), dir.absolutePath(), filter);
//	if (fileName.isEmpty() == true) return;
//
//	// ֧�ִ�����·���Ķ�ȡ
//	QByteArray ba = fileName.toLocal8Bit();
//	const char* fileName_str = ba.data();
//
//	m_points = vtkSmartPointer<vtkPoints>::New();
//	m_cellArrays =  vtkSmartPointer<vtkCellArray>::New();
//			
//	m_polyData = vtkSmartPointer<vtkUnstructuredGrid>::New();
//	/*long int tmp1 = 0;
//	long int tmp2 = 0;
//	long int tmp3 = 0;*/
//	if (style_line->getlines() > 0) {
//		/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
//		pb = style_line->getPoints();
//		tmp1 = pb->GetNumberOfPoints();
//		for (int i = 0; i < tmp1; ++i) {
//			double* point_bdr = pb->GetPoint(i);
//			m_points->InsertNextPoint(point_bdr);
//			qDebug() << i<<"line";
//		}*/
//		
//	
//		m_polyData->SetPoints(m_points);
//		for (int i = 0; i < style_line->getlines(); i++) {
//			vtkNew<vtkIdList> Line;
//			Line->InsertNextId(pointIds[i][0]);
//			Line->InsertNextId(pointIds[i][1]);
//			m_polyData->InsertNextCell(VTK_LINE, Line);
//		}
//			/*
//
//			Line->InsertNextId(pointIds[0]);
//			Line->InsertNextId(pointIds[1]);
//			m_polyData->InsertNextCell(VTK_LINE, Line);*/
//
//
//	}
//	
//	qDebug() << "123";
//
//			//m_points = NULL;
//
//
//	if (style_arc->getarcs()>0) {
//		/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
//		pb = style_arc->getPoints();
//
//		tmp2 = pb->GetNumberOfPoints();
//		for (int i = tmp1; i < tmp1+tmp2; ++i) {
//			double* point_bdr = pb->GetPoint(i-tmp1);
//			m_points->InsertNextPoint(point_bdr);
//			qDebug() <<i<<"arc";
//		}*/
//		
//		m_points = style_arc->getPoints();
//
//		for (int i = 0; i < style_arc->getarcs(); i++) {
//			vtkNew<vtkIdList> Arc;
//			for (long int j = 0; j < pnt_num[i]; j++) {
//				Arc->InsertNextId(pnt[i][j]);
//			}
//			m_polyData->InsertNextCell(VTK_POLY_LINE, Arc);
//		}
//		m_polyData->SetPoints(m_points);
//
//	}
//	
//
//	qDebug() << "123";
//	if (style_cube->getcubes() > 0) {
//		m_points = style_cube->getPoints();
//		/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
//		pb = style_cube->getPoints();
//		tmp3= pb->GetNumberOfPoints();
//		for (int i = tmp1+tmp2; i < tmp1 + tmp2+ tmp3; ++i) {
//			double* point_bdr = pb->GetPoint(i-tmp1-tmp2);
//			m_points->InsertNextPoint(point_bdr);
//			qDebug() << i<<"cube";
//		}*/
//		for (int i = 0; i < style_cube->getcubes(); i++) {
//			vtkIdType c_cells[6][4];
//			/*	= { {1,0,2,3},
//										{5,4,6,7} ,
//										{1,0,4,5} ,
//										{0,2,6,4} ,
//										{2,3,7,6} ,
//										{3,1,5,7} };*/
//										/*for (int j = 0; j < 4; j++) {
//											c_cells[0][j] = c_poi[i][j];
//										}
//										for (int j = 4; j < 8; j++) {
//											c_cells[1][j-4] = c_poi[i][j];
//										}
//										for (int j = 8; j < 12; j++) {
//											c_cells[2][j-8] = c_poi[i][j];
//										}
//										for (int j = 12; j < 16; j++) {
//											c_cells[3][j-12] = c_poi[i][j];
//										}
//										for (int j = 16; j < 20; j++) {
//											c_cells[4][j-16] = c_poi[i][j];
//										}
//										for (int j = 20; j < 24; j++) {
//											c_cells[5][j-20] = c_poi[i][j];
//
//										}*/
//			c_cells[0][0] = c_poi[i][0];//
//			c_cells[0][1] = c_poi[i][1];
//			c_cells[0][2] = c_poi[i][3];
//			c_cells[0][3] = c_poi[i][2];
//			c_cells[1][0] = c_poi[i][4];//
//			c_cells[1][1] = c_poi[i][5];
//			c_cells[1][2] = c_poi[i][7];
//			c_cells[1][3] = c_poi[i][6];
//			c_cells[2][0] = c_poi[i][0];//
//			c_cells[2][1] = c_poi[i][1];
//			c_cells[2][2] = c_poi[i][5];
//			c_cells[2][3] = c_poi[i][4];
//			c_cells[3][0] = c_poi[i][1];//
//			c_cells[3][1] = c_poi[i][3];
//			c_cells[3][2] = c_poi[i][7];
//			c_cells[3][3] = c_poi[i][5];
//			c_cells[4][0] = c_poi[i][3];//
//			c_cells[4][1] = c_poi[i][2];
//			c_cells[4][2] = c_poi[i][6];
//			c_cells[4][3] = c_poi[i][7];
//			c_cells[5][0] = c_poi[i][2];//
//			c_cells[5][1] = c_poi[i][0];
//			c_cells[5][2] = c_poi[i][4];
//			c_cells[5][3] = c_poi[i][6];
//
//			vtkNew<vtkIdList> faces;
//			for (int k = 0; k < 6; k++) {
//				faces->InsertNextId(c_cells[k][0]);
//				faces->InsertNextId(c_cells[k][1]);
//				faces->InsertNextId(c_cells[k][2]);
//				faces->InsertNextId(c_cells[k][3]);
//				//qDebug() << a;
//			}
//			m_polyData->InsertNextCell(VTK_VOXEL, faces);
//		}
//		m_polyData->SetPoints(m_points);
//
//	}
//	m_polyData->SetPoints(m_points);
//			//m_polyData->SetVerts(m_cellArrays);
//	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
//	writer->SetFileName(fileName_str);
//	writer->SetInputData(m_polyData);
//
//			//writer->SetDataModeToBinary();
//			//writer->SetDataModeToAscii();
//	writer->Write();
//}
void QtWidgetsTest::saveLine() {
	QString filter;
	filter = "vtk file (*.txt)";

	QDir dir;
	QString fileName = QFileDialog::getSaveFileName(this, QString(tr("save file")), dir.absolutePath(), filter);
	if (fileName.isEmpty() == true) return;

	// ֧�ִ�����·���Ķ�ȡ
	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();

	m_points = vtkSmartPointer<vtkPoints>::New();
	m_cellArrays = vtkSmartPointer<vtkCellArray>::New();

	m_polyData = vtkSmartPointer<vtkUnstructuredGrid>::New();
	/*long int tmp1 = 0;
	long int tmp2 = 0;
	long int tmp3 = 0;*/

		/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
		pb = style_line->getPoints();
		tmp1 = pb->GetNumberOfPoints();
		for (int i = 0; i < tmp1; ++i) {
			double* point_bdr = pb->GetPoint(i);
			m_points->InsertNextPoint(point_bdr);
			qDebug() << i<<"line";
		}*/

	m_points = style_line->getPoints();
	m_polyData->SetPoints(m_points);
	for (int i = 0; i < style_line->getlines(); i++) {
		vtkNew<vtkIdList> Line;
		Line->InsertNextId(pointIds[i][0]);
		Line->InsertNextId(pointIds[i][1]);
		m_polyData->InsertNextCell(VTK_LINE, Line);
	}
		/*

		Line->InsertNextId(pointIds[0]);
		Line->InsertNextId(pointIds[1]);
		m_polyData->InsertNextCell(VTK_LINE, Line);*/




	qDebug() << "123";
	

	//m_polyData->SetPoints(m_points);
	//m_polyData->SetVerts(m_cellArrays);
	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
	writer->SetFileName(fileName_str);
	writer->SetInputData(m_polyData);

	//writer->SetDataModeToBinary();
	//writer->SetDataModeToAscii();
	writer->Write();
}
void QtWidgetsTest::saveArc() {
	QString filter;
	filter = "vtk file (*.txt)";

	QDir dir;
	QString fileName = QFileDialog::getSaveFileName(this, QString(tr("save file")), dir.absolutePath(), filter);
	if (fileName.isEmpty() == true) return;

	// ֧�ִ�����·���Ķ�ȡ
	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();

	m_points = vtkSmartPointer<vtkPoints>::New();
	m_cellArrays = vtkSmartPointer<vtkCellArray>::New();

	m_polyData = vtkSmartPointer<vtkUnstructuredGrid>::New();

	
	/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
	pb = style_arc->getPoints();

	tmp2 = pb->GetNumberOfPoints();
	for (int i = tmp1; i < tmp1+tmp2; ++i) {
		double* point_bdr = pb->GetPoint(i-tmp1);
		m_points->InsertNextPoint(point_bdr);
		qDebug() <<i<<"arc";
	}*/

	m_points = style_arc->getPoints();
	m_polyData->SetPoints(m_points);
	for (int i = 0; i < style_arc->getarcs(); i++) {
		vtkNew<vtkIdList> Arc;
		for (long int j = 0; j < pnt_num[i]; j++) {
			Arc->InsertNextId(pnt[i][j]);
		}
		m_polyData->InsertNextCell(VTK_POLY_LINE, Arc);
	}



	//m_polyData->SetPoints(m_points);
	//m_polyData->SetVerts(m_cellArrays);
	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
	writer->SetFileName(fileName_str);
	writer->SetInputData(m_polyData);

	//writer->SetDataModeToBinary();
	//writer->SetDataModeToAscii();
	writer->Write();

}
void QtWidgetsTest::saveCube() {
	QString filter;
	filter = "vtk file (*.txt)";

	QDir dir;
	QString fileName = QFileDialog::getSaveFileName(this, QString(tr("save file")), dir.absolutePath(), filter);
	if (fileName.isEmpty() == true) return;

	// ֧�ִ�����·���Ķ�ȡ
	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();

	m_points = vtkSmartPointer<vtkPoints>::New();
	m_cellArrays = vtkSmartPointer<vtkCellArray>::New();

	m_polyData = vtkSmartPointer<vtkUnstructuredGrid>::New();
	qDebug() << "123";
	
		m_points = style_cube->getPoints();
		m_polyData->SetPoints(m_points);
		/*vtkSmartPointer<vtkPoints> pb = vtkSmartPointer<vtkPoints>::New();
		pb = style_cube->getPoints();
		tmp3= pb->GetNumberOfPoints();
		for (int i = tmp1+tmp2; i < tmp1 + tmp2+ tmp3; ++i) {
			double* point_bdr = pb->GetPoint(i-tmp1-tmp2);
			m_points->InsertNextPoint(point_bdr);
			qDebug() << i<<"cube";
		}*/


		for (int i = 0; i < style_cube->getcubes(); i++) {
			vtkIdType c_cells[6][4];
	
										/*for (int j = 0; j < 4; j++) {
											c_cells[0][j] = c_poi[i][j];
										}
										for (int j = 4; j < 8; j++) {
											c_cells[1][j-4] = c_poi[i][j];
										}
										for (int j = 8; j < 12; j++) {
											c_cells[2][j-8] = c_poi[i][j];
										}
										for (int j = 12; j < 16; j++) {
											c_cells[3][j-12] = c_poi[i][j];
										}
										for (int j = 16; j < 20; j++) {
											c_cells[4][j-16] = c_poi[i][j];
										}
										for (int j = 20; j < 24; j++) {
											c_cells[5][j-20] = c_poi[i][j];

										}*/
			c_cells[0][0] = c_poi[i][0];//
			c_cells[0][1] = c_poi[i][1];
			c_cells[0][2] = c_poi[i][3];
			c_cells[0][3] = c_poi[i][2];
			c_cells[1][0] = c_poi[i][4];//
			c_cells[1][1] = c_poi[i][5];
			c_cells[1][2] = c_poi[i][7];
			c_cells[1][3] = c_poi[i][6];
			c_cells[2][0] = c_poi[i][0];//
			c_cells[2][1] = c_poi[i][1];
			c_cells[2][2] = c_poi[i][5];
			c_cells[2][3] = c_poi[i][4];
			c_cells[3][0] = c_poi[i][1];//
			c_cells[3][1] = c_poi[i][3];
			c_cells[3][2] = c_poi[i][7];
			c_cells[3][3] = c_poi[i][5];
			c_cells[4][0] = c_poi[i][3];//
			c_cells[4][1] = c_poi[i][2];
			c_cells[4][2] = c_poi[i][6];
			c_cells[4][3] = c_poi[i][7];
			c_cells[5][0] = c_poi[i][2];//
			c_cells[5][1] = c_poi[i][0];
			c_cells[5][2] = c_poi[i][4];
			c_cells[5][3] = c_poi[i][6];

			vtkNew<vtkIdList> faces;
			for (int k = 0; k < 6; k++) {
				faces->InsertNextId(c_cells[k][0]);
				faces->InsertNextId(c_cells[k][1]);
				faces->InsertNextId(c_cells[k][2]);
				faces->InsertNextId(c_cells[k][3]);
				//qDebug() << a;
			}
			m_polyData->InsertNextCell(VTK_VOXEL, faces);
		}
		
	//m_polyData->SetPoints(m_points);
	//m_polyData->SetVerts(m_cellArrays);
	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
	writer->SetFileName(fileName_str);
	writer->SetInputData(m_polyData);

	//writer->SetDataModeToBinary();
	//writer->SetDataModeToAscii();
	writer->Write();
}
void QtWidgetsTest::cellPicker()
{

	/*double p[2][3] = { {3,2,8},{12,12,6} };
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	cellArray->InsertNextCell(2);
	for (int i = 0; i < 2; i++) {
		points->InsertPoint(i, p[i]);
		cellArray->InsertCellPoint(i);
	}
	polyData->SetPoints(points);
	polyData->SetLines(cellArray);*/

	

	vtkNew<CellPicker> style;
	style->SetCurrentRenderer(_renderer);

	widget->interactor()->SetInteractorStyle(style);

	widget->interactor();
	widget->renderWindow()->Render();

}
void QtWidgetsTest::operate()
{
	
	vtkNew<OperateStyle> style;
	style->SetCurrentRenderer(_renderer);
	if (actor_color) {
		ui.horizontalSlider->setValue((int)actor_color[0] * 100);
		ui.horizontalSlider_2->setValue((int)actor_color[1] * 100);
		ui.horizontalSlider_3->setValue((int)actor_color[2] * 100);
		qDebug() << "horizontal:" << actor_color[0] << actor_color[1] << actor_color[2];
	}
	
	ui.horizontalSlider->update();
	ui.horizontalSlider_2->update();
	ui.horizontalSlider_3->update();
	

	qDebug() << worldPoint1[0];
	widget->interactor()->SetInteractorStyle(style);

	widget->interactor();
	widget->renderWindow()->Render();

}
void QtWidgetsTest::pickDot() {
	vtkSmartPointer<PointPickerInteractorStyle> style = vtkSmartPointer<PointPickerInteractorStyle>::New();
	style->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style);
	widget->interactor();
	widget->renderWindow()->Render();
}
void QtWidgetsTest::distance() {
	vtkSmartPointer<ExtractFaceStyle> style = vtkSmartPointer<ExtractFaceStyle>::New();
	style->SetCurrentRenderer(_renderer);
	widget->interactor()->SetInteractorStyle(style);

	widget->interactor();
	widget->renderWindow()->Render();
}
void QtWidgetsTest::mouseReleaseEvent(QMouseEvent* event) {

}
