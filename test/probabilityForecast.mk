TESTS = eps2probandltf_test

check_PROGRAMS = eps2probandltf_test

eps2probandltf_test_CPPFLAGS = $(AM_CPPFLAGS) $(gtest_CFLAGS)
eps2probandltf_test_LDADD = $(eps2probandltf_LDADD) $(gtest_LIBS) -lgtest_main

eps2probandltf_test_SOURCES = \
	$(eps2probandltf_SOURCES:src/main.cpp=) \
	test/CrossContainerTest.cpp \
	test/SymbolGenerateTest.cpp \
	test/temperatureTest.cpp \
	test/windTest.cpp \
	test/RealWorldTest.cpp \
	test/timeTest.cpp \
	test/probabilityAboveTresholdTest.cpp

include mk/gtest.mk
