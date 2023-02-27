
# #######################################################
# LINUX
# #######################################################
# install python dev from source
# version not less 3.8.0 
# enable shared option is ON

cd Python-3.8.0/
./configure --enable-optimizations --enable-shared
make -j 8
python3.8 --version

ldconfig

# links
https://linuxize.com/post/how-to-install-python-3-8-on-ubuntu-18-04/
https://github.com/docker-library/python/issues/21
https://gist.github.com/zhensongren/811dcf2471f663ed3148a272f1faa957?permalink_comment_id=3930240


# install gcc-7 g++-7
https://gist.github.com/jlblancoc/99521194aba975286c80f93e47966dc5 

# install cmake version not less 3.10


# build
mkdir build_unit
cmake ../unit
make

# run python test
cd build_unit/bin
python3 pycore_test.py

# #######################################################
# WINDOWS
# #######################################################

# MSVC not less Microsoft Visual C++ 2019
# install cmake version not less 3.10
# install python3 version not less 3.8.0

# build
mkdir build_unit
cmake ../unit
cmake --build . --config Release

# run python test
cd build_unit/bin
python3 pycore_test.py

