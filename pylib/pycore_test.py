
import unit_core

if __name__ == "__main__":

    unit_core.test_func_c(1, 2)

    unit_core.test_func_cpp(1, 2)

    unit_core.test_func_eigen(2, 2)

    unit_core.test_func_xtensor(2, 2)

    unit_core.test_mavlink(str("serial:///dev/ttyUSB0:57600"))

    unit_core.test_mavlink_v2(str("serial:///dev/ttyUSB0:57600"))

    unit_core.test_opencv(str("data/test.jpg"))

    unit_core.test_onnx(str("data/test_training_model_2.onnx"))

