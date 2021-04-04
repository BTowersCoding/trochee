ARDUINO=${HOME}/Downloads/arduino-1.8.7
INCLUDES=-I${ARDUINO}/hardware/teensy/avr/cores/teensy \
         -I${ARDUINO}/hardware/teensy/avr/cores/teensy3

all: controller_test vi_command_test vi_translator_test dbg_translator_test

controller_test: controller_test.cc controller.h vi_translator.h \
                 events.h key_map.h layout.h matrix.h \
                 fake_clock.h fake_scanner.h fake_sender.h
	g++ -g -std=c++0x -o $@ $(filter %.cc,$^) ${INCLUDES}

vi_command_test: vi_command_test.cc vi_command.h
	g++ -g -std=c++0x -o $@ $(filter %.cc,$^) ${INCLUDES}

vi_translator_test: vi_translator_test.cc vi_translator.h translator.h \
                    events.h key_map.h \
                    fake_clock.h fake_sender.h
	g++ -g -std=c++0x -o $@ $(filter %.cc,$^) ${INCLUDES}

dbg_translator_test: dbg_translator_test.cc dbg_translator.h translator.h \
                    events.h fake_sender.h \
                    layout.h layouts/darknight2.h
	g++ -g -std=c++0x -o $@ $(filter %.cc,$^) ${INCLUDES}
