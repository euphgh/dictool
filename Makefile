BUILD_DIR := build

init:
	cmake -B$(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=1

clean:
	rm -rf $(BUILD_DIR)