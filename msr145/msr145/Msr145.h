#pragma once

#include "stdafx.h"

typedef LRESULT APIENTRY GetDLLVersion(void);
typedef LRESULT APIENTRY OpenComport(UCHAR PortIDX, LPSTR name);
typedef LRESULT APIENTRY CloseComport(UCHAR PortIDX);
typedef LRESULT APIENTRY WakeUp(UCHAR PortIDX);
typedef LRESULT APIENTRY TxRx(UCHAR PortIDX, UCHAR in0, UCHAR in1, UCHAR in2, UCHAR in3, UCHAR in4, UCHAR in5, UCHAR in6, UCHAR* out0, UCHAR* out1, UCHAR* out2, UCHAR* out3, UCHAR* out4, UCHAR* out5, UCHAR* out6);

class Msr145
{
private:
	UCHAR				portNumber;
	char				*portName;
	HINSTANCE			hinstLib;
	GetDLLVersion		*funcGetDLLVersion;
	OpenComport			*funcOpenComport;
	CloseComport		*funcCloseComport;
	WakeUp				*funcWakeUp;
	TxRx				*funcTxRx;

public:
	static const int	PORT = 0;
	static const int	PRESSURE = 0;
	static const int	TEMP_P = 1;
	static const int	ACCX = 2;
	static const int	ACCY = 3;
	static const int	ACCZ = 4;
	static const int	HYGRO = 5;
	static const int	TEMP_RH = 6;
	static const int	TEMP = 7;
	static const int	BATTERY = 8;
	static const int	PRESSURE2 = 9;
	static const int	TEMP2 = 10;
	UCHAR				output[7];

	static const int	SUCCESS = 0;
	static const int	FAIL = -1;
	static const int	FAIL_DLL = -2;
	static const int	FAIL_FUNCTION = -3;
	static const int	SENSOR_READY = 6;

public:
	Msr145();
	Msr145(int portNumber, char *portName);
	~Msr145();

	int				load_dll();
	int				get_dll_version() const;
	int				open_com_port() const;
	int				close_com_port() const;
	int				wake_up() const;
	int				txrx(UCHAR in0, UCHAR in1, UCHAR in2, UCHAR in3, UCHAR in4, UCHAR in5, UCHAR in6, UCHAR *out0, UCHAR *out1, UCHAR *out2, UCHAR *out3, UCHAR *out4, UCHAR *out5, UCHAR *out6) const;

	int				init_measurement();
	int				read_measurement(int sensor1, int sensor2, int sensor3);
	int				read_acceleration();

	static float	get_pressure(UCHAR outL, UCHAR outH);
	static float	get_temp_rh(UCHAR outL, UCHAR outH);
	static float	get_hygro(UCHAR outL, UCHAR outH);
	static float	get_acceleration(UCHAR out);
};