# Compiler and flags
# 定义编译器
CC = mpicxx

# 定义编译选项，这里可以根据需要添加优化选项等
CCFLAGS = -O3 -march=native -funroll-loops -ffast-math -flto -Wunused-result -Wno-return-type -fprefetch-loop-arrays -I./source -I./include -I/home/xinxiangsama/lib/hdf5-1.14.6/include -DH5_BUILD_AS_DYNAMIC_LIB
# CCFLAGS = -I./source -I./include -I/home/xinxiangsama/lib/hdf5-1.14.6/include -DH5_BUILD_AS_DYNAMIC_LIB

# 定义链接选项
# LDFLAGS = -lboost_system -lboost_filesystem -L/home/xinxiangsama/lib/hdf5-1.14.6/lib -lhdf5_cpp -lhdf5
LDFLAGS = -L/home/xinxiangsama/lib/hdf5-1.14.6/lib -lhdf5_cpp -lhdf5

# 定义源文件
SOURCES = main.cpp ./source/parallel/Parallel.cpp ./source/parallel/CartesianParallel.cpp ./source/model/EulerPhase.cpp ./source/meshes/Cell.cpp ./source/meshes/CellInterface.cpp ./source/meshes/CartesianElement.cpp ./source/meshes/CartesianMesh.cpp ./source/meshes/CartesianFace.cpp ./source/interpolation/Interpolation.cpp ./source/interpolation/Weno5th.cpp ./source/initialcondition/Reimman2D.cpp ./source/initialcondition/Cylinder.cpp ./source/boundary/NeummanBoundary.cpp ./source/boundary/DirichletBoundary.cpp ./source/flux/EulerFlux.cpp ./source/model/EulerMod.cpp  ./source/flux/RiemmanSolverHLL.cpp ./source/io/Output.cpp ./source/gradient/FourthOrderCS.cpp ./source/gradient/EulerGradient.cpp ./source/timeadvance/TVD2rdRK.cpp ./source/timeadvance/TVD3rdRK.cpp ./source/object/Segment.cpp ./source/object/Circle.cpp ./source/object/ImagePoint.cpp Run.cpp

# 定义目标文件
OBJECTS = $(patsubst %.cpp, build/%.o, $(SOURCES))

# 定义最终的可执行文件名
EXECUTABLE = runsim

# 默认目标
all: $(EXECUTABLE)

# 链接目标文件生成可执行文件
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

# 从.cpp文件编译出.o文件
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

# 清理规则
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f DSMC_OUT/*.csv
	rm -f DSMC_OUT/*.json
	rm -f ./force_data.csv
	rm -f ./*.json
	rm -rf ./*.txt
	rm -rf ./run/*.h5

# 防止make自动生成*.o文件的规则
.PHONY: all clean