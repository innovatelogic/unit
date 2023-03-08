echo "unit linux install"

SRC_DIR=$1
OUT_DIR=$2

cp ${SRC_DIR}/pylib/pycore_test.py ${OUT_DIR}
cp ${SRC_DIR}/third_party/onnxruntime/Linux/libonnxruntime.so.1.15.0 ${OUT_DIR}
cp ${SRC_DIR}/third_party/georgraphiclib/lib/libGeographicLib.so.23.1.0 ${OUT_DIR}/libGeographicLib.so.23

cp -R ${SRC_DIR}/data ${OUT_DIR}