#include "videoUtils.h"
#include "Tool.h"

#include <mutex>
#include <QMutex>

std::mutex g_mtx;

VideoUtils::VideoUtils() :
	m_isSaveVideo(false)
{
}

VideoUtils::~VideoUtils()
{
}

bool VideoUtils::openCamera(int cameraId, double frameWidth, double frameHeight, std::string &msg)
{
	if (!m_videoCapture.isOpened())
	{
		if (m_videoCapture.open(cameraId))
		{
			if (!(m_videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280) && m_videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 720)))
			{
				msg = " RGB camera init error !";
			}
			else
			{
				return true;
			}
		}
		else
		{
			msg = " RGB camera Index config error !";
		}
	}
	else
	{
		msg = "RGB camera is occupied !";
	}

	return false;
}

bool VideoUtils::openVideo(const std::string & videoPath)
{
	if (!m_videoCapture.open(videoPath))
	{
		return false;
	}

	return true;
}

void VideoUtils::closeVideo()
{
	if (m_videoCapture.isOpened())
	{
		m_videoCapture.release();
	}
}

QPixmap VideoUtils::getFrame()
{
	//处理RGB摄像头数据
	m_videoCapture >> m_rgbFrame;



	//保存视频
	if (m_isSaveVideo)
	{
		m_outputVideo << m_rgbFrame;
	}

	if (!m_rgbFrame.empty())
	{
		IplImage *rgbImage = NULL;
		{
			std::lock_guard<std::mutex> lock(g_mtx);
			IplImage iplrgbImage(m_rgbFrame);
			rgbImage = cvCloneImage(&iplrgbImage);
		}

		m_pixRgbImage = QPixmap::fromImage(cvMatToQImage(cv::cvarrToMat(rgbImage)));
		cvReleaseImage(&rgbImage);

		return m_pixRgbImage;
	}

	return QPixmap();
}


void VideoUtils::setSpecifyFrame(int frameIndex)
{
	m_videoCapture.set(CV_CAP_PROP_POS_FRAMES, frameIndex);
}

void VideoUtils::saveVideoEnable(bool isSaveVideo)
{
	m_isSaveVideo = isSaveVideo;

	if (!m_isSaveVideo)
	{
		m_outputVideo.release();
	}
}

bool VideoUtils::initVideoWriter(const std::string localPath)
{
	if (m_videoCapture.isOpened())
	{
		bool ret = m_outputVideo.open(localPath, CV_FOURCC('M', 'J', 'P', 'G'), 25.0,
		cv::Size((int)m_videoCapture.get(CV_CAP_PROP_FRAME_WIDTH), (int)m_videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT)));

		return ret;
	}

	return false;
}

int VideoUtils::getVideoFrameTotalNum(const std::string& videoPath)
{
	//获取视频总帧数
		CvCapture* pCapture = NULL;
		if (!(pCapture = cvCaptureFromAVI(videoPath.c_str())))
		{
			return -1;
		}
		return (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);

}





