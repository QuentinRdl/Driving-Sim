#! /bin/sh

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
  echo "Usage: $0 <Release | Debug | Both | -h>"
  echo " => -h\t\t print this."
  echo " => Release\t Build the executable Release."
  echo " => Debug\t Build the executable as Debug."
  echo " => Both\t Build two times, one as Debug and one as Release."
  exit 0
fi

BASE_FOLDER="/Users/romain/CLionProjects/Driving-Sim"

cd $BASE_FOLDER

BUILD_TYPE=${1:-Debug}
BUILD_TYPE_LOWER=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
BUILD_DIR=$BASE_FOLDER/build/$BUILD_TYPE
COMPILE_DIR=$BASE_FOLDER/build/$BUILD_TYPE/build/$BUILD_TYPE

cd $BASE_FOLDER
if [ "$BUILD_TYPE" != "Release" ] && [ "$BUILD_TYPE" != "Debug" ] && [ "$BUILD_TYPE" != "Both" ]; then
  echo "Usage: $0 <Release | Debug | Both>" >&2
  exit 1
fi

cd $BASE_FOLDER
rm -rf $BUILD_DIR

if [ "$BUILD_TYPE" == "Both" ]; then
  /bin/sh "$0" "Release"
  /bin/sh "$0" "Debug"
  exit 0
fi

echo "Build Type: $BUILD_TYPE \nBuild directory: $BUILD_DIR"

conan install . --output-folder="$BUILD_DIR" --build=missing -s build_type=$BUILD_TYPE
cmake --preset conan-$BUILD_TYPE_LOWER
cmake --build "$COMPILE_DIR"
