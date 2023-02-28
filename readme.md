
### #######################################################
## LINUX Ubuntu 20.04.3 LTS
### #######################################################
### install python dev from source
### version not less 3.8.0 
### enable shared option is ON

### optional install from source
cd Python-3.8.0/
./configure --enable-optimizations --enable-shared
make -j 8
python3.8 --version
ldconfig

### links
https://linuxize.com/post/how-to-install-python-3-8-on-ubuntu-18-04/
https://github.com/docker-library/python/issues/21
https://gist.github.com/zhensongren/811dcf2471f663ed3148a272f1faa957?permalink_comment_id=3930240


### install gcc g++ not less  gcc-7 g++-7
sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa
sudo apt update
sudo apt install g++-10

### install cmake version not less 3.10

### mavlink (mavsdk)
wget https://github.com/mavlink/MAVSDK/releases/download/v0.37.0/mavsdk_0.37.0_ubuntu20.04_amd64.deb
sudo dpkg -i mavsdk_0.37.0_ubuntu20.04_amd64.deb

### mavlink
Clone https://github.com/mavlink/mavlink
make with cmake


### build
mkdir build_unit
cmake ../unit
make

### run python test
cd build_unit/bin
python3 pycore_test.py

### #######################################################
### WINDOWS
### #######################################################

### MSVC not less Microsoft Visual C++ 2019
### install cmake version not less 3.10
### install python3 version not less 3.8.0

wsl --install -d Ubuntu-20.04 --web-download

### build
mkdir build_unit
cmake ../unit
cmake --build . --config Release

### run python test
cd build_unit/bin
python3 pycore_test.py

