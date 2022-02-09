#include "Drowsypick_tool.h"
#include "ui_Drowsypick_tool.h"
#include <cstdlib>
#include <ctime>
#include "Tool.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include <fstream>
#include <iostream>
#include <QDebug>
#include <algorithm>
#include <QTextCursor>
#include <QApplication>
#include <QFileDialog>
#include <QTime>
#include <QtWidgets/QMessageBox>
#include <QKeyEvent>

#define random(x) rand()%(x)


Drowsypick_tool::Drowsypick_tool(QWidget *parent)
    : QWidget(parent), 
	ui(new Ui::Drowsypick_tool)
{

	initWidget();
	initCamera();

	QImage image1("C:\\Users\\lxh8557\\Pictures\\test.png");
	ui->label_left->setPixmap(QPixmap::fromImage(image1));
	ui->label_right->setPixmap(QPixmap::fromImage(image1));
	time_left = new QTime;
	time_right = new QTime;
	int rand_left =  6;
	int rand_right =10;
	t_picture_left = startTimer(1000 * rand_left);
	t_picture_right = startTimer(1000 * rand_right);
	qDebug() << rand_left;
	qDebug() << rand_right;
	
}

Drowsypick_tool::~Drowsypick_tool()
{
	delete ui;
}



void Drowsypick_tool::initWidget()
{
	ui->setupUi(this);



}


void Drowsypick_tool::connectSlot()
{
	connect(ui->PLAY, &QPushButton::clicked, this, &Drowsypick_tool::operationCamera);



}


void Drowsypick_tool::operationCamera()
{
	
		openCamera();


}

void Drowsypick_tool::keyPressEvent(QKeyEvent *ev)
{
	QImage image1("C:\\Users\\lxh8557\\Pictures\\test.png");
	if (ev->key() == Qt::Key_Left)
	{
		ui->label_left->setPixmap(QPixmap::fromImage(image1));
		ui->label_left->repaint();
		//ui->textBrowser_left->append(time_left->elapsed());
		qDebug() << "The slow operation took" << time_left->elapsed() << "milliseconds";
	}

	if (ev->key() == Qt::Key_Right)
	{
		ui->label_right->setPixmap(QPixmap::fromImage(image1));
		ui->label_right->repaint();
	}

	QWidget::keyPressEvent(ev);
}


void Drowsypick_tool::timerEvent(QTimerEvent *event)
{
	QImage image("C:\\Users\\lxh8557\\Pictures\\run.png");

	if (event->timerId() == t_picture_left) {       // 判断是哪个定时器
		ui->label_left->setPixmap(QPixmap::fromImage(image));
		ui->label_left->repaint();
		time_left->start();
	}

	if (event->timerId() == t_picture_right) {
		ui->label_right->setPixmap(QPixmap::fromImage(image));
		ui->label_right->repaint();
		time_right->start();
	}
}



/*
void Drowsypick_tool::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	srand((int)time(0));
	int t_random = random(100);

	
	QImage image1("C:\\Users\\lxh8557\\Pictures\\test.png");

	
	QImage image("C:\\Users\\lxh8557\\Pictures\\run.png");
	if (t_random % 2 == 1)
	{
		ui->label_left->setPixmap(QPixmap::fromImage(image));
		Sleep(1000);
		//->label_left->hide();
	}
	else {
		ui->label_left->setPixmap(QPixmap::fromImage(image1));
	}
	
	if (t_random % 2 == 0)
	{
		ui->label_right->setPixmap(QPixmap::fromImage(image));
		Sleep(1000);
		//ui->label_right->hide();
	}
	else {
		ui->label_right->setPixmap(QPixmap::fromImage(image1));
	}
	
	
}
*/

void Drowsypick_tool::initCamera()
{
	m_cameraNameList.clear();

	m_camera = new VideoUtils;
	//m_video = new VideoUtils;
}


void Drowsypick_tool::openCamera()
{
	m_cameraNameList.clear();
	listDevices(m_cameraNameList);


	if (2 == m_cameraNameList.size())
	{
		m_rgbCameraId = ui->comboBox->currentIndex();

		std::string errMsg;
		if (!m_camera->openCamera(m_rgbCameraId, 1080, 720, errMsg))
		{
			QMessageBox::about(NULL, "提示", QString::fromStdString(errMsg));
		}
		else
		{
			m_isOpenCamera = true;
		}
	}
	else
	{
		QMessageBox::about(NULL, "提示", "摄像头数量不支持！");
	}
}

void Drowsypick_tool::closeCamera()
{
	m_isOpenCamera = false;
	Sleep(200);
}

void Drowsypick_tool::cameraView()
{
	while (m_isOpenCamera)
	{
		//Sleep(3);
		m_pixRgbImage = m_camera->getFrame();

	}

	ui->CameraView->clear();
	m_camera->closeVideo();
}

