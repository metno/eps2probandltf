eps2probandltf_SOURCES += \
	src/dataTypes.h \
	src/main.cpp \
	src/localtime.h \
	src/localtime.cpp \
	src/ForecastCollection.h \
	src/WeatherData.h

include src/configuration/probabilityForecast.mk
include src/container/probabilityForecast.mk
include src/generator/probabilityForecast.mk
include src/data_access/probabilityForecast.mk

