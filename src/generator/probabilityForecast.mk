eps2probandltf_SOURCES += \
	src/generator/ProbabilityGeneratorControl.cpp \
	src/generator/ProbabilityGeneratorControl.h \
	src/generator/ProbabilityGenerator.cpp \
	src/generator/ProbabilityGenerator.h \
	src/generator/algorithm.h \
	src/generator/bits/algorithm.t.h
	

include src/generator/simple/probabilityForecast.mk
include src/generator/percentile/probabilityForecast.mk
include src/generator/symbol/probabilityForecast.mk
include src/generator/temperature/probabilityForecast.mk
include src/generator/wind/probabilityForecast.mk
include src/generator/precipitation/probabilityForecast.mk
include src/generator/airPressure/probabilityForecast.mk
include src/generator/algorithms/probabilityForecast.mk
