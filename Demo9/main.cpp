#include <iostream>
using namespace std;

#include <string.h>
#include <time.h>
#include <chrono>  

#include <ceres/ceres.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

void yaml_read_write()
{
	std::string config_file = "/home/song/catkin_ws/src/VINS-Fusion_Learn/config/mynteye-s/mynt_mono_config.yaml";

	//read yaml file. 
    cv::FileStorage fsSettings(config_file, cv::FileStorage::READ);
    if (!fsSettings.isOpened()) {
        std::cerr << "ERROR: Wrong path to settings" << std::endl;
    }

	std::string IMAGE0_TOPIC;
    fsSettings["image0_topic"] >> IMAGE0_TOPIC;
	cout << "image0_topic: " << IMAGE0_TOPIC << endl << endl;

    cv::Mat cv_T;
	fsSettings["body_T_cam0"] >> cv_T;
	Eigen::Matrix4d T;
	cv::cv2eigen(cv_T, T);
	cout << "body_T_cam0 Matrix: " << endl << T << endl << endl;

	//write yaml file. 
	cv::FileStorage fs("file.csv", cv::FileStorage::WRITE);

	// Eigen::Matrix4d eigen_T = Eigen::Matrix4d::Identity();
	cv::Mat cv_T_;
	cv::eigen2cv(T, cv_T_);
	fs << "body_T_cam_new" << cv_T_ ;
	fs.release();
}

// 正常来讲，一个四元数是不能和一个向量直接做乘法的，维度都对不上；
// 但Eigen::Quaterniond可以和向量v直接做乘法，其结果为q转为R后，R与向量v的乘积。
// 如下边的例子，v1等于v2。
void EigenTest1()
{
	Eigen::Quaterniond q(2, 0, 1, -3);
	q = q.normalized(); //验证了下，不做归一化不影响后边结果。
	cout << q.w() << ", " << q.x() << ", " << q.y() << ", " << q.z() << endl;

	Eigen::Matrix3d R = q.toRotationMatrix();
	Eigen::Vector3d v(0, 0, 9.8);

	Eigen::Vector3d v1 = q * v;
	cout << v1[0] << ", " << v1[1] << ", " << v1[2] << endl;

	Eigen::Vector3d v2 = R * v;
	cout << v2[0] << ", " << v2[1] << ", " << v2[2] << endl;
}

void CopyTest()
{
    queue<pair<double, Eigen::Vector3d>> accBuf;
	Eigen::Vector3d v(0, 0, 9.8);
    accBuf.push(make_pair(1, v));
    accBuf.push(make_pair(2, v));
    accBuf.push(make_pair(3, v));

	//验证此处是深拷贝还是浅拷贝。答案：深拷贝
    queue<pair<double, Eigen::Vector3d>> tmp_accBuf = accBuf;

    while(!tmp_accBuf.empty())
    {
        Eigen::Vector3d acc = tmp_accBuf.front().second;
        tmp_accBuf.pop();
		cout << accBuf.size() << endl;
    }
}

//ref:https://blog.csdn.net/m0_37263637/article/details/95059209
void getTimeStamp1()
{
	std::time_t timestamp;
    std::cout << time(&timestamp) << std::endl;//秒级时间戳, 
	string s = asctime(std::localtime(&timestamp));
    cout << s << endl;

    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
    timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
    std::cout << timestamp << std::endl;//毫秒级时间戳
	//输出如:1572849784510

	// using namespace std::chrono;
	// steady_clock::duration d = steady_clock::now().time_since_epoch();
	// minutes min = duration_cast<minutes>(d);
	// seconds sec = duration_cast<seconds>(d);
	// milliseconds mil = duration_cast<milliseconds>(d);
	// microseconds mic = duration_cast<microseconds>(d);
	// nanoseconds nan = duration_cast<nanoseconds>(d);
	// cout << min.count() << "分钟" << endl;
	// cout << sec.count() << "秒" << endl;
	// cout << mil.count() << "毫秒" << endl;
	// cout << mic.count() << "微妙" << endl;
	// cout << nan.count() << "纳秒" << endl;
}

void getTimeStamp2()
{
	//https://blog.csdn.net/q229827701/article/details/51272753
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
	
    int64 milli = timestamp+ (int64)8*60*60*1000;//此处转化为东八区北京时间，如果是其它时区需要按需求修改
    auto mTime = std::chrono::milliseconds(milli);
    auto tp_=std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>(mTime);
    auto tt = std::chrono::system_clock::to_time_t(tp_);
    std::tm* now = std::gmtime(&tt);
    printf("%4d/%02d/%02d %02d:%02d:%02d\n",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
}

void cal_diff_time(void)
{
    long i = 800000000L;
    double duration;
    clock_t start, finish;

    start = clock();
    while( i-- );
	// sleep(1); //测sleep的耗时不准。
    finish = clock();

    duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "start:"<< start<< " ,finish:"<< finish<< endl;
    printf( "%f seconds\n", duration );
    // system("pause");
}

int main(int argc, char** argv)
{
	// yaml_read_write();

	EigenTest1();

	// CopyTest();

	// getTimeStamp1();

	// getTimeStamp2();

	// cal_diff_time();

	// getTimeStamp2();

	return 0;
}
