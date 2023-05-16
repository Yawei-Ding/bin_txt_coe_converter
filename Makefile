# 1. config path:
TXT_PATH := ${PWD}/input/snn/map.txt
BIN_PATH := ${PWD}/output/snn/map.bin
COE_PATH := ${PWD}/output/snn/map.coe

TXT_DATA_W := 21
BIN_DATA_W := 32 # mind: bin file data width must align to 8 bits.

CPP_FILE +=  $(shell find $(abspath ./source) -name "*.cpp")
BIN += ${PWD}/build/converter

com: ${CPP_FILE}
	g++ ${CPP_FILE} -o ${BIN}

bin2txt: com
	${BIN} --input=${BIN_PATH} --output=${TXT_PATH} --in_w=${BIN_DATA_W}  --out_w=${TXT_DATA_W}

txt2bin: com
	${BIN} --input=${TXT_PATH} --output=${BIN_PATH} --in_w=${TXT_DATA_W}  --out_w=${BIN_DATA_W}

bin2coe: 
	bin2coe -i ${BIN_PATH} -w ${BIN_DATA_W} -o ${COE_PATH}

txt2coe: com
	${BIN} --input=${TXT_PATH} --output=${BIN_PATH} --in_w=${TXT_DATA_W}  --out_w=${BIN_DATA_W}
	bin2coe -i ${BIN_PATH} -w ${BIN_DATA_W} -o ${COE_PATH}

clean:
	rm ${BIN}