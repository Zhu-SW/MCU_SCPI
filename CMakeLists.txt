#cmake最低版本需求，不加入此行会受到警告信息，输入下载安装的cmake版本。
cmake_minimum_required(VERSION 4.0)
#项目名称,即生成的.exe文件名称,这里的文件名称必需与launch.json的参数"program"的一致。
#这个文件是保存在tasks.json的指定的目录，在本例中是build文件夹。
project(main)
# 添加头文件目录
include_directories(./SCPI ./example)
#把当前目录.下所有源文件加入变量SRC_LIST，
#这里的.代表当前目录，也即当前workspace(工作区、工作空间)，本例中cc1文件夹位置。
aux_source_directory(./example SRC_LIST)
aux_source_directory(./SCPI SCPI_LIST)
#生成应用程序main，在windows下会自动生成main.exe。
add_executable(main ${SRC_LIST} ${SCPI_LIST})
#设置可执行文件输出路径
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin)

