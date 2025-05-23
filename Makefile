include LED/aaplus/Makefile

CXX = g++

OBJ = build/sound/SoundServer.o \
build/sound/Sound.o \
build/sound/MaryClient.o \
build/onkyo/OnkyoServer.o \
build/onkyo/OnkyoManager.o \
build/sensors/MotionSensor.o \
build/sensors/PCSensor.o \
build/sensors/Switch.o \
build/hmi/HMI.o \
build/video/KodiController.o \
build/video/VideoPlayer.o \
build/userModes/ModeManager.o \
build/userModes/UserMode.o \
build/userModes/modes/IlluminateMode.o \
build/userModes/modes/KodiMode.o \
build/userModes/modes/OffMode.o \
build/userModes/modes/PCMode.o \
build/userModes/modes/ReadingMode.o \
build/userModes/modes/CookingMode.o \
build/userModes/modes/IlluminateAscheberg.o \
build/userModes/modes/TVMode.o \
build/LED/LEDController.o \
build/LED/LEDManager.o \
build/LED/Rayleigh.o \
build/CEC/CECControl.o \
build/MQTT/MQTT.o\
build/MQTT/devices/MotionSensor.o\
build/MQTT/devices/Switch.o

FLAGS = -std=c++17 -Wall -Wextra -I /usr/local/include/boost/boost_1_78_0 -MD -g

LDLIBS = -lasound -pthread -lvlc -lcec -ldl -lstdc++fs -L/usr/local/lib -lpaho-mqttpp3 -lpaho-mqtt3a -lpaho-mqtt3c -lssl -lcrypto

all: build/smartHomeServer.bin

build/smartHomeServer.bin : $(OBJ) main.cpp $(AAPP_OBJS)
	$(CXX) main.cpp $(OBJ) $(AAPP_OBJS) -o build/smartHomeServer.bin $(FLAGS) $(LDLIBS)

build/sound/%.o: sound/%.cpp
	mkdir -p build/sound/
	$(CXX) -c $(FLAGS) $< -o $@

build/onkyo/%.o: onkyo/%.cpp
	mkdir -p build/onkyo/
	$(CXX) -c $(FLAGS) $< -o $@

build/sensors/%.o: sensors/%.cpp
	mkdir -p build/sensors/
	$(CXX) -c $(FLAGS) $< -o $@

build/hmi/%.o: hmi/%.cpp
	mkdir -p build/hmi/
	$(CXX) -c $(FLAGS) $< -o $@

build/video/%.o: video/%.cpp
	mkdir -p build/video/
	$(CXX) -c $(FLAGS) $< -o $@

build/userModes/%.o: userModes/%.cpp
	mkdir -p build/userModes/
	$(CXX) -c $(FLAGS) $< -o $@

build/userModes/modes/%.o: userModes/modes/%.cpp
	mkdir -p build/userModes/modes/
	$(CXX) -c $(FLAGS) $< -o $@

build/LED/%.o: LED/%.cpp
	mkdir -p build/LED/
	$(CXX) -c $(FLAGS) $< -o $@

build/LED/aaplus/%.o: LED/aaplus/%.cpp
	mkdir -p build/LED/aaplus/
	$(CXX) -c $(FLAGS) $< -o $@

build/CEC/%.o: CEC/%.cpp
	mkdir -p build/CEC/
	$(CXX) -c $(FLAGS) $< -o $@

build/MQTT/%.o: MQTT/%.cpp
	mkdir -p build/MQTT/
	$(CXX) -c $(FLAGS) $< -o $@

build/MQTT/devices/%.o: MQTT/devices/%.cpp
	mkdir -p build/MQTT/devices/
	$(CXX) -c $(FLAGS) $< -o $@

-include $(OBJ:.o=.d)

clean:
	rm -fdr build
