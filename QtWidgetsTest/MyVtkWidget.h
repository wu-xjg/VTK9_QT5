#include <QToolBar>
#include <QLabel>
#include <QVTKOpenGLNativeWidget.h>
class MyVtkWidget : public QVTKOpenGLNativeWidget
{
public:
    MyVtkWidget(QWidget* parent = nullptr) : QVTKOpenGLNativeWidget(parent) {}

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        // ���� QLabel
        QLabel* label = new QLabel(this);
        label->setText("Hello, world!");

        // �� QLabel ��ӵ���������
        QToolBar* toolbar = parentWidget()->findChild<QToolBar*>("myToolbar");
        if (toolbar)
        {
            toolbar->addWidget(label);
        }

        // ���û���ĺ������Ա㴦������������¼�
        QVTKOpenGLNativeWidget::mousePressEvent(event);
    }
}; 
