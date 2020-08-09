# CSV-Reader
## Requirements
* Git
* CMake

## How to build?

```shell
git clone https://github.com/PatifonKakao/CSV-Reader
cd CSV-Reader
```
If you want use gtest
```shell
git checkout gtests
git submodule init
git submodule update
```


### Linux

```shell
mkdir build
cd build
cmake ..
make
cd bin
./csvreader
```

### Windows (MinGW)

```shell
mingw.bat
cd output_64
mingw32-make
cd bin
csvreader.exe
```

### Windows (Visual Studio)
* Edit vs.bat
* Run vs.bat
* Set as StartUp Project csvreader
