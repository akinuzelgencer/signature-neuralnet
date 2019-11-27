PROJ=Signature
DIR="build"
PROG="main.cpp"
BIN="runme.exe"
OTHERS="lib/NeuralNet.cpp lib/Image.cpp lib/BackProp.cpp"

# CMAKE
mkdir "./$DIR"
echo "cmake_minimum_required(VERSION 2.8)
project( $PROJ )
find_package( OpenCV REQUIRED )
include_directories( \${OpenCV_INCLUDE_DIRS} )
add_executable( $BIN $PROG $OTHERS )
target_link_libraries( $BIN \${OpenCV_LIBS} )" > "CMakeLists.txt"
cd "./$DIR" 
cmake ..
cd ..

# MAKE
cd "./$DIR" 
make
cd ..
mv "./$DIR/$BIN" .

# EXECUTE
"./$BIN" > output.json



# CLEANUP
rm "./CMakeLists.txt"
rm -rf "./$DIR" 