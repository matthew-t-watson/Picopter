#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-bcm2708hardfp-linux-gnueabi-gcc
CCC=arm-bcm2708hardfp-linux-gnueabi-g++
CXX=arm-bcm2708hardfp-linux-gnueabi-g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=RPiGNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AHRS.o \
	${OBJECTDIR}/CommandLineInterface.o \
	${OBJECTDIR}/HMC5883L.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/I2CInterface.o \
	${OBJECTDIR}/PICInterface.o \
	${OBJECTDIR}/Control.o \
	${OBJECTDIR}/MPU6050.o \
	${OBJECTDIR}/ConfigFile.o \
	${OBJECTDIR}/Logger.o \
	${OBJECTDIR}/Timer.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-lrt -lpthread
CXXFLAGS=-lrt -lpthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/picopter

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/picopter: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/picopter ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/AHRS.o: nbproject/Makefile-${CND_CONF}.mk AHRS.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/AHRS.o AHRS.cpp

${OBJECTDIR}/CommandLineInterface.o: nbproject/Makefile-${CND_CONF}.mk CommandLineInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/CommandLineInterface.o CommandLineInterface.cpp

${OBJECTDIR}/HMC5883L.o: nbproject/Makefile-${CND_CONF}.mk HMC5883L.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/HMC5883L.o HMC5883L.cpp

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/I2CInterface.o: nbproject/Makefile-${CND_CONF}.mk I2CInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/I2CInterface.o I2CInterface.cpp

${OBJECTDIR}/PICInterface.o: nbproject/Makefile-${CND_CONF}.mk PICInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/PICInterface.o PICInterface.cpp

${OBJECTDIR}/Control.o: nbproject/Makefile-${CND_CONF}.mk Control.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Control.o Control.cpp

${OBJECTDIR}/MPU6050.o: nbproject/Makefile-${CND_CONF}.mk MPU6050.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/MPU6050.o MPU6050.cpp

${OBJECTDIR}/ConfigFile.o: nbproject/Makefile-${CND_CONF}.mk ConfigFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/ConfigFile.o ConfigFile.cpp

${OBJECTDIR}/Logger.o: nbproject/Makefile-${CND_CONF}.mk Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Logger.o Logger.cpp

${OBJECTDIR}/Timer.o: nbproject/Makefile-${CND_CONF}.mk Timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Timer.o Timer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/picopter

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
