// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include <stdio.h>
#include <iostream>
#include <chrono>
//#include "framework.h"

#define IPADDRESS_SENSOR_KYENCE "100.0.0.3"
#define IPADDRESS_SENSOR_KAIS	
#define IPADDRESS_MOTION_PAS "192.168.0.2"
#define IPADDRESS_MOTION_INA


#define WM_FORWARD	WM_USER+100
#define WM_BACKWARD	WM_USER+101

enum motionSelected
{
	Kyence,
	Kais,
	Linear
};

enum AXIS
{
	x,
	y
};

typedef struct _Sensor {
	double top = 0;
	double btm = 0;
	double ths = 0;
}Sensor;
typedef struct _Sensor1 {
	double top[10000];
	double btm[10000];
	double ths[10000];
}Sensor1;


#endif //PCH_H
