#### 图像读取和显示

```c++
Mat img = imread("path");//读取图像
Mat img = imread("path", IMREAD_GRAYSCALE);//读取图像，灰色显示
if (img.empty()) {//检查是否有图像
	retrun -1;
}
namedWindow("showimg", WINDOW_FREERATIO);//新建窗口，名为"showimg"
imshow("showimg", img);//在 showimg 窗口显示 8 位色彩图片
waitKey(n);//等待 n 毫秒，0 表示无限等待
destroyAllWindows();//关闭所有图像
```

#### 图像色彩空间转换

色彩空间在物体识别中扮演重要角色，从图像中提取特定范围的颜色并加上逻辑判断来识别物体。

色彩空间用不同方式去定义颜色，不同角度去识别颜色。

`RGB`：R - 红色，G - 绿色，B - 蓝色 ，模拟光的混合原理，颜色越混合越浅

缺点：色相（颜色）和亮度（强度）数据混淆；

`CMY`：C - 青色，M - 品红色，Y - 黄色，模拟颜料混合原理，颜色越混合越深

`HSV` ：H - 色相（主波长，颜色信息），S - 饱和度（纯度/颜色阴影），V - 值（亮度/强度）

优点：颜色只和一个通道相关

缺点：受设备影响

`LAB`：L - 亮度（强度）, A - 颜色A（从绿色到洋红色），B - 颜色B（从蓝色到黄色）

优点：与设备无关

`YCrCb`：Y - 伽马校正后从 `RGB` 获得的亮度，Cr - 红色距离 `Luma` 多远，Cb - 蓝色距离 `Luma`多远

优点：将亮度和颜色分成不同通道

缺点：受设备影响

`灰度图`：单通道图像，只包含图像的光度信息，没有颜色信息

```c++
Mat gray, hsv, img = imread("path");
cvtColor(img, hsv, COLOR_BGR2HSV);//色彩转换
cvtColor(img, gray, COLOR_BGR2GRAY);
imshow("HSV", hsv);
imshow("GRAY", gray);
imwrite("hsv_path", hsv);//保存图片
imwrite("gray_path", gray);
```

#### 图像对象的创建和赋值

Mat 数据分为头部和数据，数据就是图像像素信息，头部包含了图像大小，长宽等信息

```c++
Mat img = imread("path");
Mat img1 = img.clone();//创建，克隆
Mat img2; img.copyTo(img2);//复制
Mat img3 = img;//赋值
//创建空白
Mat img4 = Mat::zeros(img.size(), img.type());
img4 = Scalar(127, 127, 127);//赋值B G R通道
Mat img5 = Mat::zeros(Size(512, 512), CV_8UC3);
Mat img5 = Mat::ones(Size(512, 512), CV_8UC3);
//简单卷积核
Mat kernel = (Mat_<char>(3,3) << 0,-1,0,-1,5,-1,0,-1,0);
```

#### 图像像素读写操作

```c++
Mat img = imread("path");
//如果图像是单通道，读的是灰度图像
int pv = img.at<uchar>(row, col)//读取 row 行 col 列的像素值
img.at<uchar>(row, col) = pv;//像素赋值
//如果图像是三通道，读的是bgr图像
Vec3b bgr = img.at<Vec3b>(row, col);//读取 row 行 col 列的b g r三通道像素值
int pvb = img.at<Vec3b>(row, col)[0];//读取像素的 b 通道像素值
int pvg = img.at<Vec3b>(row, col)[1];//读取像素的 g 通道像素值
int pvr = img.at<Vec3b>(row, col)[2];//读取像素的 r 通道像素值
img.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(bgr[0] + bgr[1] + bgr[2]);//像素赋值

Mat dst = img + Scalar(50, 50, 50);//img 三个通道的像素全部加 50（提高亮度，减是降低亮度）
Mat ret;
multipy(img, dst, ret);//图像相乘
```

#### 调整图像亮度

```c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//这里的 b 就是 createTrackbar 绑定滑条的变量
//调节亮度
static void on_track(int b, void* userdata) {
    Mat img = *((Mat*)userdata);
    Mat la_dst = Mat::zeros(img.size(), img.type());
    Mat la_m = Mat::zeros(img.size(), img.type());
    la_m = Scalar(b, b, b);
    add(img, la_m, la_dst);
    imshow("brightness adjust", la_dst);
}
//这里的 c 就是 createTrackbar 绑定滑条的变量
//调节对比度
static void on_contrast(int c, void* userdata) {
    Mat img = *((Mat*)userdata);
    Mat la_dst = Mat::zeros(img.size(), img.type());
    Mat la_m = Mat::zeros(img.size(), img.type());
    //混合图像
    addWeighted(img, c / 50, la_m, 0.0, 0, la_dst);
    imshow("brightness adjust", la_dst);
}

int main(int argc, char const **argv)
{
    int lightness = 50;int bmax = 100;
    int contrast = 1;int cmax = 100;
    Mat la_src = imread("/home/lmr/opencv_proj/test/00.jpeg");    
    namedWindow("brightness adjust", WINDOW_FREERATIO);
    //给窗口创建滑条
    //第一个参数是滑条的显示标签
    //第二个参数是要创建滑条的窗口
    //第三个参数是滑条绑定的变量
    //第四个参数是滑条最大值
    //第五个参数是滑条触发的回调函数
    createTrackbar("BrightnessBar:", "brightness adjust", &lightness, bmax, on_track, (void*)(&la_src));
    //初始化lightness值
    on_track(50, &la_src);
    createTrackbar("ContrastBar:", "brightness adjust", &contrast, cmax, on_contrast, (void*)(&la_src));
	on_contrast(50, &la_src);
	waitKey(0);
    destroyAllWindows();
    return 0;
}
```

#### 键盘相应操作

```c++
waitkey(n);//n 毫秒检查一次键盘输入，返回 int 类型
```

#### 颜色表操作

