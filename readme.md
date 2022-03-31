# 图着色问题 (GCP)
## 编译
 - os: Windows 10 x64
 - compiler: gcc  6.3.0 (MinGW.org GCC-6.3.0-1)
 - cmake: cmake version 3.23.0-rc3
 - make: GNU Make 3.82.90
```shell script
cd build
cmake -G"MinGW Makefiles" ..
mingw32-make
```
## 运行
```shell script
gcp.exe -p ../data/dsjc250.5.col -c 30 -i 1000000 -o output.txt
```

## 历史最佳/最佳结果
- dsjc250.5.col: 30/28
