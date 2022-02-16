#pragma once

#include <QtWidgets/QWidget>
#include "ui_Drowsypick_tool.h"
#include "videoUtils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Drowsypick_tool; }
QT_END_NAMESPACE

class Drowsypick_tool : public QWidget
{
    Q_OBJECT

public:
    Drowsypick_tool(QWidget *parent = nullptr);
	~Drowsypick_tool();



private slots:
	void operationCamera();
	//void on_rightBtn_clicked();

	//void on_leftBtn_clicked();

private:
    Ui::Drowsypick_tool *ui;

	void initWidget();
	void initCamera();

	void openCamera();
	void closeCamera();
	void connectSlot();

	
	//bool gotoFrame(int nIndex);
	void videoView(int frameTotal);

	//线程函数
	void cameraView();
	std::atomic<int>  m_frameIndex;

	bool m_realTimeFlag; // 是否是实时播放
	bool left_image_appearance;
	bool right_image_appearance;

	std::atomic<bool> m_isOpenCamera;
	std::atomic<bool> m_isVideoPlay;
	std::atomic<bool> m_isCANRecord;

	std::vector<std::string> m_cameraNameList;
	int m_rgbCameraId;

	VideoUtils* m_camera;
	VideoUtils* m_video;

	QTime* time_left;
	QTime* time_right;

	QPixmap m_pixRgbImage;       //视频模式下预览数据
	QPixmap m_pixStaticRgbImage; //静态图预览数据

	int t_picture_left, t_picture_right;

protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	virtual void keyPressEvent(QKeyEvent *ev);

private slots:
	void operationDisplay();


};

