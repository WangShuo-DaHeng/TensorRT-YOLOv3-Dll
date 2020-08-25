#include "class_timer.hpp"
#include "class_detector.h"

#include <memory>
#include <thread>

/* OpenCV headers */
#ifdef _DEBUG  
#pragma comment(lib,"opencv_world411d.lib") 
#else
#pragma comment(lib,"opencv_world411.lib") 
#endif
#pragma comment(lib, "detector.lib")

int main()
{
	Config config_v3;
	config_v3.net_type = YOLOV3_TINY;
	config_v3.file_model_cfg = "C:/Users/25785/Desktop/һ������ѧϰ����Dll���롾�ѹ鵵��/yolo-tensorrt/configs/yolov3-tiny.cfg";
	config_v3.file_model_weights = "C:/Users/25785/Desktop/һ������ѧϰ����Dll���롾�ѹ鵵��/yolo-tensorrt/configs/yolov3-tiny.weights";
	config_v3.calibration_image_list_file_txt = "C:/Users/25785/Desktop/һ������ѧϰ����Dll���롾�ѹ鵵��/yolo-tensorrt/configs/calibration_images.txt";
	config_v3.inference_precison = FP32;

	cv::Mat image0 = cv::imread("C:/Users/25785/Desktop/һ������ѧϰ����Dll���롾�ѹ鵵��/yolo-tensorrt/configs/2.bmp", cv::IMREAD_UNCHANGED);
	cv::Mat image1 = cv::imread("C:/Users/25785/Desktop/һ������ѧϰ����Dll���롾�ѹ鵵��/yolo-tensorrt/configs/3.bmp", cv::IMREAD_UNCHANGED);
	std::unique_ptr<Detector> detector(new Detector());
	detector->init(config_v3);                                  //��ʼ������ṹ
	std::vector<BatchResult> batch_res;
	Timer timer;
	for (;;)
	{
		//prepare batch data
		std::vector<cv::Mat> batch_img;
		cv::Mat temp0 = image0.clone();
		cv::Mat temp1 = image1.clone();
		batch_img.push_back(temp0);
		batch_img.push_back(temp1);

		//detect
		timer.reset();
		detector->detect(batch_img, batch_res);                  //������
		timer.out("detect");

		//disp
		for (int i=0;i<batch_img.size();++i)
		{
			for (const auto &r : batch_res[i])
			{
				std::cout <<"batch "<<i<< " id:" << r.id << " prob:" << r.prob << " rect:" << r.rect << std::endl;
				cv::rectangle(batch_img[i], r.rect, cv::Scalar(255, 0, 0), 2);
				std::stringstream stream;
				stream << std::fixed << std::setprecision(2) << "id:" << r.id << "  score:" << r.prob;
				cv::putText(batch_img[i], stream.str(), cv::Point(r.rect.x, r.rect.y - 5), 0, 0.5, cv::Scalar(0, 0, 255), 2);
			}
			cv::imshow("image"+std::to_string(i), batch_img[i]);
		}
		cv::waitKey(10);
	}
}
