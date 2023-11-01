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
        // 创建 QLabel
        QLabel* label = new QLabel(this);
        label->setText("Hello, world!");

        // 将 QLabel 添加到工具栏中
        QToolBar* toolbar = parentWidget()->findChild<QToolBar*>("myToolbar");
        if (toolbar)
        {
            toolbar->addWidget(label);
        }

        // 调用基类的函数，以便处理其他的鼠标事件
        QVTKOpenGLNativeWidget::mousePressEvent(event);
    }
}; 
