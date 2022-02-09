#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QObject>
#include <QImage>
#include <QPixmap>

class VideoUtils : public QObject
{
public:
	explicit VideoUtils();
	~VideoUtils();

	bool openCamera(int cameraId, double frameWidth, double frameHeight, std::string &msg);
	bool openVideo(const std::string& videoPath);
	void closeVideo();

	QPixmap getFrame();
	void setSpecifyFrame(int frameIndex);

	void saveVideoEnable(bool isSaveVideo);
	bool initVideoWriter(const std::string localPath);

	int getVideoFrameTotalNum(const std::string& videoPath);

	void getCameraFrameFromQueue();

private:
	std::atomic<bool> m_isSaveVideo;

	cv::VideoCapture m_videoCapture;
	cv::Mat m_rgbFrame;
	cv::VideoWriter m_outputVideo;

	QPixmap m_pixRgbImage;       //视频模式下预览数据
	QPixmap m_pixStaticRgbImage; //静态图预览数据
};