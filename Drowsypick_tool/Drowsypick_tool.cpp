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
#include <thread>


#define random(x) rand()%(x)


Drowsypick_tool::Drowsypick_tool(QWidget *parent)
    : QWidget(parent), 
	ui(new Ui::Drowsypick_tool)
{
	initWidget();
	initCamera();
	m_isVideoPlay = true;
	connectSlot();
}

Drowsypick_tool::~Drowsypick_tool()
{
	delete ui;
}


void Drowsypick_tool::initWidget()
{
	ui->setupUi(this);

	QImage image_background("C:\\Users\\lxh8557\\Pictures\\test.png");
	ui->label_left->setPixmap(QPixmap::fromImage(image_background));
	ui->label_right->setPixmap(QPixmap::fromImage(image_background));

}




void Drowsypick_tool::connectSlot()
{
	connect(ui->PLAY, &QPushButton::clicked, this, &Drowsypick_tool::operationDisplay);
	connect(ui->exit1, &QPushButton::clicked, this, [=]()
	{
	 // ui->setAttribute(Qt::WA_DeleteOnClose);
		close();
		QApplication* app;
		app->exit(0);
	});

	//connect(ui->exit1, SIGNAL(clicked()), this, SLOT(close()));
	//connect(sEditors, SIGNAL(closeWindow()), this, SLOT());

}

void Drowsypick_tool::operationDisplay()
{
	time_left = new QTime;
	time_right = new QTime;

	left_image_appearance = false;
	right_image_appearance = false;

	int rand_left = 30;
	int rand_right = 150;
	t_picture_left = startTimer(1000 * rand_left);
	t_picture_right = startTimer(1000 * rand_right);

	std::string filePath = "20211209_152611.avi";

	m_video->openVideo(filePath);
	int totalFrameNum = m_video->getVideoFrameTotalNum(filePath);
	std::thread threadVideo([this, totalFrameNum]() {videoView(totalFrameNum); });
	threadVideo.detach();

	qDebug() << rand_left;
	qDebug() << rand_right;

}


void Drowsypick_tool::operationCamera()
{

	std::thread threadCamera(&Drowsypick_tool::cameraView, this);
	threadCamera.detach();
}

void Drowsypick_tool::keyPressEvent(QKeyEvent *ev)
{
	QImage image_background("C:\\Users\\lxh8557\\Pictures\\test.png");
	QString output;
	if (ev->key() == Qt::Key_Left && left_image_appearance==true)
	{
		//ui->Drowsypick_tool->reloadPage();
		ui->label_left->setPixmap(QPixmap::fromImage(image_background));
		ui->label_left->repaint();
		output = "reaction  time of left " + QString::number(time_left->elapsed()) + "  ms";
		//
		QString currentTime = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
		std::string filePath = currentTime.toStdString() + ".avi";
		m_camera->initVideoWriter(filePath);
		m_camera->saveVideoEnable(true);
		//

		QMessageBox::information(NULL, "left time", output, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//ui->textBrowser_left->append(QString::number(time_left->elapsed(),10,5));
		left_image_appearance = false;
	}

	if (ev->key() == Qt::Key_Right && right_image_appearance == true)
	{
		ui->label_right->setPixmap(QPixmap::fromImage(image_background));
		ui->label_right->repaint();
		output = "reaction  time of right " + QString::number(time_right->elapsed()) + "  ms";
		QMessageBox::information(NULL, "right time", output, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		right_image_appearance = false;
	}


	QWidget::keyPressEvent(ev);
}


void Drowsypick_tool::timerEvent(QTimerEvent *event)
{
	QImage image("C:\\Users\\lxh8557\\Pictures\\run.png");

	if (event->timerId() == t_picture_left) {       // 判断是哪个定时器
		left_image_appearance = true;
		ui->label_left->setPixmap(QPixmap::fromImage(image));
		ui->label_left->repaint();
		time_left->start();
		//QString::number(time_left->elapsed()).toStdString()
		
		
	}

	if (event->timerId() == t_picture_right) {
		right_image_appearance = true;
		ui->label_right->setPixmap(QPixmap::fromImage(image));
		ui->label_right->repaint();
		time_right->start();

	}
}




void Drowsypick_tool::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);


	ui->CameraView->setPixmap(m_pixStaticRgbImage);
	ui->CameraView->setAlignment(Qt::AlignCenter);
	ui->CameraView->setScaledContents(true);
	
}


void Drowsypick_tool::initCamera()
{
	m_cameraNameList.clear();

	m_camera = new VideoUtils;
	m_video = new VideoUtils;
	openCamera();
	operationCamera();
	

}


void Drowsypick_tool::openCamera()
{
	m_cameraNameList.clear();
	listDevices(m_cameraNameList);


	if (1 == m_cameraNameList.size())
	{
		m_rgbCameraId = ui->comboBox->currentIndex();

		std::string errMsg;
		if (!m_camera->openCamera(m_rgbCameraId, 1080, 720, errMsg))
		{
			QMessageBox::about(NULL, "attention", QString::fromStdString(errMsg));
		}
		else
		{
			m_isOpenCamera = true;
		}
	}
	else
	{
		QMessageBox::about(NULL, "attention", "  Can support the number of camera  ");
	}

	
}

void Drowsypick_tool::closeCamera()
{
	m_isOpenCamera = false;
	Sleep(200);
}

void Drowsypick_tool::videoView(int frameTotal)
{


	while (m_isVideoPlay)
	{
		m_pixStaticRgbImage = m_video->getFrame();
		if (m_frameIndex == frameTotal - 1)
		{
			Sleep(25);
			m_isVideoPlay = false;
			//emit signalVideoPlayFinished();
			break;
		}

		Sleep(1000);
	}

	m_video->closeVideo();
}

void Drowsypick_tool::cameraView()
{
	while (m_isOpenCamera)
	{
		Sleep(3);
		m_pixRgbImage = m_camera->getFrame();
	}

	//ui->CameraView->clear(); //dispay on ui
	m_camera->closeVideo();
}

