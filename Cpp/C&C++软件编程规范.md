## 命名

函数命名, 变量命名, 文件命名应具备描述性; 不要过度缩写. 类型和变量应该是名词, 函数名可以用 “命令性” 动词。

### 1. 生命周期命名规范

1. 全局变量：
    * 规则：以前缀g_开头，单词全为小写，用下划线隔开。
    * 示例:

    ```C++
    uint8_t g_ack_list_cnt;
    ```

    * 注：全局变量会增大模块之间的耦合关系，所以尽可能减少使用太多冗余的全局变量。
1. 局部变量：
    * 规则：
        1. 单词全部为小写，用下划线分割。
        1. 使用常用的习惯变量。示例: min、max、sum、num、diff。
        1. 如循环变量，可以使用i、j、k、m、n等，其他类型变量尽可能做到望文知意，可使用长一点的单词，便于理解代码。
    * 示例：

    ```C++
    uint16_t left_wheel_current;
    ```

1. 类成员变量：
    * 规则：单词全部为小写，用下划线隔开，**以下划线结尾**。
    * 示例：

    ```C++
    uint8_t msg_list_cnt_;
    ```

### 2. 类型命名规范

1. 指针变量：
    * 规则：以前缀p_开头，全小写。
    * 示例：

    ```C++
    char *p_msg;
    ```

1. 常量：
    * 规则：声明为 constexpr 或 const 的变量, 或在程序运行期间其值始终保持不变的, 命名时以 “k” 开头, 单词首字母大写。
    * 注：全大写，有可能与宏命名冲突, 不推荐。
    * 示例：

    ```C++
    const int kMaxSize = 20;
    ```

1. 数组：
    * 规则：以前缀a_开头，全小写。
    * 注：数组长度尽量用常量代替。
    * 示例：

    ```C++
    uint8_t a_buf[kMaxSize];
    ```

1. 结构体：
    * 规则：以后缀_t结尾，单词首字母大写，结构体内成员命名为变量命名。
    * 示例：

    ```C++
    typedef struct
    {
        int x;
        int y;
    } Point_t;
    typedef struct
    {
        uint8_t size;
        int x_min;
        int x_max;
        int y_min;
        int y_max;
        Point_t middle;
    } ZoneAttrid_t;
    ```

1. 枚举类型：
    * 规则：
        1. 枚举名称个单词首字母大写，以后缀_t结尾。
        1. 枚举内部常量：
            * 规则：命名时以 “k” 开头, 单词首字母大写。
        1. 需要指定第一个枚举量的值，后面的值**不可指定**。
        示例：

        ```C++
        typedef enum
        {
            kLinuxDownCmdLed = 0,
            kLinuxDownCmdGyro,
            kLinuxDownCmdVacuum,
        } DownCmd_t;
        ```

1. 枚举变量：
    * 规则：以前缀e_开头，单词全为小写，用下划线隔开。
    * 示例：
        `DownCmd_t e_down_cmd;`

1. 宏定义：
    * 规则：
        1. 全为大写，用下划线隔开。
        1. 若宏定义内部有计算，需要用括号包裹起来。
    * 示例：

        ```C++
        #define ROBOT_HARDWARE_TYPE_A 1
        #define ROBOT_HARDWARE_TYPE ROBOT_HARDWARE_TYPE_A
        
        #define MAP_MAX (CELL_SIZE * TEN_CELL)
        ```

1. 宏开关：
    * 规则：
        1. 尽量使用清晰详细的表述，不可过于广泛表义。
        1. 在开关结束时需要注释对应开关。
        1. 宏开关不缩进。
    * 示例：

        ```C++
        #define ENABLE_SHADOW_DEVICE
        #define ENABLE_WIFI
        
        {
        #ifdef ENABLE_SHADOW_DEVICE
            //Code...
            {
        #ifdef ENABLE_WIFI
                //Code...
        #endif // ENABLE_WIFI
            }
            //Code...
        #endif // ENABLE_SHADOW_DEVICE
        }
        
        ```

1. 函数：
    * 规则：
        1. 首字母大写。
        1. 遵循动词接名词方式。
        1. 对于首字母缩写的单词，视作一个普通单词。
    * 示例：

        ```C++
        void AddTwoNumbers();
        void SetCurrentDate(); //非CurrentDateSet();
        void GetAppData(); //非GetAPPData();
        ```

1. 类：
    * 规则：
        1. 首字母大写。
        1. 类中权限声明顺序为public到protected到private，中途不可穿插。
        1. 权限声明范围内，成员函数在上，成员变量在下。
    * 示例：

    ```C++
    class SocketWrapper
    {
    public:
        Func1();
    
        Func2();
    
        bool variable1;
    
    protected:
        Func3();
    
        bool variable2;
    
    // 不可在此又添加一个public:
    
    private:
        Func4();
    
        uint8_t variable3;
    }
    ```

1. 文件、文件夹：
    * 规则： 单词小写，以下划线隔开。
    * .h文件应该和.cpp文件配合，头文件负责声明，代码文件负责实现。
    * .hpp文件一般来说不需要有一个.cpp文件配合，所有实现都在.hpp文件中编写，一般这种文件不能实现很复杂的功能，每个函数代码量是精简的，和工作内容都是单一的。实现复杂功能的话，就使用.h和.cpp文件配合来实现。
    * 示例：
        * 文件夹： ali_linkkit_wrapper
        * 文件：
            event_manager.cpp
            hal_wheel.h

1. 命名空间：
    * 规则：
        1. 用小写字母命名, 并基于项目名称和目录结构命名，单词以下划线隔开。
        1. 内容不缩进。
        1. 禁止在头文件中using命名空间。
        1. 在命名空间结束时需要注释对应空间名。
    * 示例：

        ```C++
        namespace cartographer {
        namespace sensor {
        
        class class1
        {
        
        }
        
        void Func1();


        } // namespace sensor
        } // namespace cartographer
        ```

## 排版

1. 文件编码使用**utf-8**。
1. 文件换行符用**Unix格式**。
1. 代码中使用缩进风格编写，缩进使用4空格。
1. 所有行与行之间的代码对齐需使用空格键，**不能使用Tab键缩进**。
1. 关键词需自占一行，后面需有一个空格。比如if、for、do、while、switch、case。括号内侧不需要留空格。
    * 示例：

    ```C++
    // 不能是 if( power_switch )
    if (power_switch)
    {
        // Code..
    }
    ```

1. 部分操作符号前后需要加空格。比如: "+"，"-"，"="，"=="，"!="，"<<"，"&"，"||"。
    * 示例：

    ```C++
    // 不能是 if ((current_mode==kModeIdle)&&(power_switch!=kOff))
    if ((current_mode == kModeIdle) && (power_switch != kOff))
    {
        // 不能是 int a=b+c;
        int a = b + c;
        // 不能是 LOG(INFO)<<"Test";
        LOG(INFO) << "Test";
    }
    ```

1. 尽量用英文单词书写，不能用拼音。
1. switch语句的case值不能使用数字，且一定要有default选项。
    * 示例：

    ```C++
    switch (task_type)
    {
        // 不能是 case 1:
        case kUploadPowerSwitch:
            // Code...
            break;
        default:
            break;
    }
    ```

1. 逗号后面需要加空格，参照英文书写方式。
    * 示例：

    ```C++
    // 不能是 AddThreeNumbers(a,b,c);
    AddThreeNumbers(a, b, c);
    
    // 英文注释也需要注意逗号。
    // 不能是 This function do this,and that.
    // This function do this, and that.
    ```

1. 循环、判断若有较长的表达式，超过每行字符数，则要进行拆分。操作符放在新行之首，并对齐总括号。
    * 示例：

    ```C++
    if ((num < max_list_number) && (num <= kMaxSize)
        && (item_valid(stat_item)))
    {
    
    }
    switch ((num < max_list_number) && (num <= kMaxSize)
            && (item_valid(stat_item)) && condition2
            && condition3)
    {
    
    }
    
    while ((num < max_list_number) && (num <= kMaxSize)
           && (item_valid(stat_item)))
    ```

1. switch程序块模板：
    * 示例：

    ```C++
    switch (task_type)
    {
        // 不能是 case 1:
        // 缩进4个空格
        case kUploadPowerSwitch:
            // Code...
            break;
        case kUploadGoHomeSwitch:
            // Code...
            break;
        default: // 一定要有！
            break;
    }
    ```

1. 头文件预处理指令不使用#pragma，用#ifndef代替。
    * 示例：
    hal_wifi.h文件

    ```C++
    #ifndef HAL_WIFI_H
    #define HAL_WIFI_H
    
    // Code...
    
    #endif // HAL_WIFI_H
    ```

1. 函数形参的"*"和"&"统一写在靠近变量的左边。不和数据类型靠近。
    * 示例：

    ```C++
    // 不能是 void test(int* p_addr);
    void test(int *p_addr);
    // 不能是 void test(int& addr);
    void test(int &addr);
    ```

1. const有助于编译层面保护数据稳定，比如说参数或者地址不可修改，前面可加const，函数里面不会由于错误操作导致修改了参数。const位于"\*"的左侧，则const是用来修饰指针所指向的变量，则指针指向内容为常量。const位于"\*"的右侧，则const是用来修饰指针本身，则指针本身是常量，但内容可修改。
    * 示例：

    ```C++
    void LedUpdate(const HwLed_t *this)
    {
        uint32_t time = this->change_time; 
        //只能读取this指向内容，不能赋值。
        this->change_time = time; // 错误！！
    }
    
    void LedUpdate(HwLed_t * const this)
    {
        this->change_time = now();
        //能对this指向内容修改
        this = new Time(); // 错误！！
    }
    ```

1. 每一行代码字符数不超过**80**。

1. 左大括号换行放置，if后如果只有一行执行代码，也需要用大括号包含。
    * 示例：

    ```C++
    if (a > b)
    {
        uint32_t time = this->change_time;
    }
    ```

1. 头文件引用规则：
    * 需要对外输出的头文件放到include目录下，否则头文件放在与.cpp同目录下。
    * 工程项目将设置头文件搜索路径为include目录和src目录。
    * 项目内头文件应按照项目源代码目录树结构排列, 避免使用. (当前目录) 或 .. (上级目录).
    * .cpp中第一个include头文件为.cpp对应的.h文件，防止交叉引用和重复引用。
    * 系统库函数文件include使用<>，其他头文件include使用""。

    * 示例：
    pp/src/base/logging.h

    在logging.cpp中的include顺序为：
    1. #include "base/logging.h”
    1. C系统文件
    1. C++系统文件
    1. 其他库.h文件
    1. 本项目.h库文件

## 编码

1. 所有变量声明时要进行初始化, 类成员变量初始化在构造函数上初始化，不在声明时初始化。**初始化要按变量定义顺序编写。**
    * 示例：

    ```C++
    class Test1()
    {
    public:
        Test1();
        ~Test1();
    
        // 不写成 bool is_on_{false};
        bool is_on_;
        // 不写成 int test_count_{0};
        int test_count_;
    }
    
    // 不能是 Test1::Test1(): test_count_(0),
    //                       is_on_(false)
    Test1::Test1(): is_on_(false), 
                    test_count_(0)
    {
        //Code...
    }
    ```

1. 建议尽量编写简短, 功能单一的函数，如果函数超过 60 行, 可以思索一下能不能在不影响程序结构的前提下对其进行分割。

## 注释

1. 注释的原则有助于对程序的阅读，注释的语言必须准确、易懂，简洁。
1. 有能力的话尽量用英文注释，但必须清晰准确。不行的话用中文也可以。
1. 自解释性强的代码可以不用注释，自己设计的算法类代码在自解释性弱的时候**一定要加注释**。
1. 维护代码的同时，也要**维护注释**，避免出现注释与代码不符的情况，误导开发者。
1. 注释时，使用//注释号，**不使用/\*\*/**。
1. 文件的头部需加注释，包括记录描述，版权等。
    * 示例：

    ```C++
    //********************************************************
    //* @Copyright 2021 Shenzhen Zhiyi Technology Co., Ltd
    //* @Description: about map information
    //*******************************************************/
    ```

1. 函数的头部应进行注释，列出:函数的目的/功能，输入、输出参数。
    * 示例：

    ```C++
    //************************************************
    // Description: Check if the coordinate cell is in 
    // the map.
    // Parameter:
    //     @x coordinate cell x. 
    //     @y coordinate cell y.
    // Return: 0 Not in the map
    //         1 in the map.
    //************************************************
    uint8_t Map::IsCellInMap(int16_t x, int16_t y)
    {
        // program code
    }
    ```

## 调试打印

在高级语言程序设计中，我们根据对程序运行信息的打印类型进行分级设计把控，可以把日志分别5个级别，分别为TRACE,DEBUG,INFO,WARN,ERROR,FATAL。可根据以下介绍，选择合适的打印等级。
TRACE:Release版本不会消除此Level的打印，但一般不启用，可以当做Release版本的后门打印，可用于产品售后或返厂检测时使用。
DEBUG:主要用在程序开发调试阶段的打印输出，用于验证程序的设计逻辑是否满足上层应用的设计需求，在经过测验核验之后的发布程序之前可以把它关掉，只有Debug版本编译时才生效。
INFO:这个级别的打印输出是用来告诉测试人员或者开发人员的一些特殊的信息，比如一些关键数据的操作、状态，位置等。
WARN:这个一种警告的打印输出，它一般用来输出程序异常情况但不严重的警告打印，后期程序发布前后需维护追踪此等级的打印。
ERROR:运行出错的打印，优秀的程序设计应用具有很好的容错性能，在程序运行出错的情况下能自己修正回来。在发布版本之前，需重点留意此等级的打印，消除所有的错误才能正确发布版本。
FATAL: 程序遇到面临错误且无法自修正的情况下，需要打印此log，以分析到程序异常的原因。
