#ifndef _STANDARDWIDGET_H_
#define _STANDARDWIDGET_H_

#include <QWidget>

class Device;
class MenuBar;
class StatusBar;

class StandardWidget : public QWidget
{
Q_OBJECT
public:
	StandardWidget(Device *device, QWidget *parent = 0);
	~StandardWidget();
	
protected:
	void performStandardSetup(const QString& title);
	
	Device *m_device;
	MenuBar *m_menuBar;
	StatusBar *m_statusBar;
};

#endif