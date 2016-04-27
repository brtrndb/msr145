#include "stdafx.h"
#include "Msr145.h"

Msr145::Msr145()
{
	this->funcGetDLLVersion = nullptr;
	this->funcOpenComport = nullptr;
	this->funcCloseComport = nullptr;
	this->funcWakeUp = nullptr;
	this->funcTxRx = nullptr;
	this->portNumber = (UCHAR) Msr145::PORT;
	this->portName = (char*) malloc(5 * sizeof(char));
	this->portName[0] = 'C';
	this->portName[1] = 'O';
	this->portName[2] = 'M';
	this->portName[3] = '4';
	this->portName[4] = '\0';
	this->output[7] = { 0 };
}

Msr145::Msr145(int portNumber, char *portName)
{
	this->funcGetDLLVersion = nullptr;
	this->funcOpenComport = nullptr;
	this->funcCloseComport = nullptr;
	this->funcWakeUp = nullptr;
	this->funcTxRx = nullptr;
	this->portNumber = (UCHAR) portNumber;
	strcpy_s(this->portName, std::strlen(portName), portName);
	this->output[7] = { 0 };
}

Msr145::~Msr145()
{
	this->close_com_port();
	free(this->portName);
}

int Msr145::load_dll()
{
	this->hinstLib = LoadLibrary(L"./dll/MSR145_1.dll");
	if (!(this->hinstLib))
		return (Msr145::FAIL_DLL);

	this->funcGetDLLVersion = (GetDLLVersion*) GetProcAddress(hinstLib, "GetDLLVersion");
	this->funcOpenComport = (OpenComport*) GetProcAddress(hinstLib, "OpenComport");
	this->funcCloseComport = (CloseComport*) GetProcAddress(hinstLib, "CloseComport");
	this->funcWakeUp = (WakeUp*) GetProcAddress(hinstLib, "WakeUp");
	this->funcTxRx = (TxRx*) GetProcAddress(hinstLib, "TxRx");

	if (!(this->funcGetDLLVersion && this->funcOpenComport && this->funcCloseComport && this->funcWakeUp && this->funcTxRx))
		return (Msr145::FAIL_FUNCTION);
	return (Msr145::SUCCESS);
}

int Msr145::get_dll_version() const
{
	int res = Msr145::SUCCESS;
	if (this->funcGetDLLVersion)
		res = (*(this->funcGetDLLVersion)) ();
	return (res);
}

int Msr145::open_com_port() const
{
	int res = Msr145::SUCCESS;
	if (this->funcOpenComport)
		res = (*(this->funcOpenComport)) (this->portNumber, this->portName);
	return (res);
}

int Msr145::close_com_port() const
{
	int res = Msr145::SUCCESS;
	if (this->funcCloseComport)
		res = (*(this->funcCloseComport)) (this->portNumber);
	return (res);
}

int Msr145::wake_up() const
{
	int res = Msr145::SUCCESS;
	if (this->funcWakeUp)
		res = (*(this->funcWakeUp)) (this->portNumber);
	return (res);
}

int Msr145::txrx(UCHAR in0, UCHAR in1, UCHAR in2, UCHAR in3, UCHAR in4, UCHAR in5, UCHAR in6, UCHAR *out0, UCHAR *out1, UCHAR *out2, UCHAR *out3, UCHAR *out4, UCHAR *out5, UCHAR *out6) const
{
	int res = -1;
	if (this->funcTxRx)
		res = (*(this->funcTxRx)) (this->portNumber, in0, in1, in2, in3, in4, in5, in6, out0, out1, out2, out3, out4, out5, out6);
	return (res);
}

int Msr145::init_measurement(UCHAR out[7]) const
{
	int res = Msr145::SUCCESS;
	res = txrx(6, 3, 0, 0xFF, 0xFF, 0, 0, &out[0], &out[1], &out[2], &out[3], &out[4], &out[5], &out[6]);
	Sleep(500);
	return (res);
}

int Msr145::read_measurement(int sensor1, int sensor2, int sensor3, UCHAR *out) const
{
	int res = Msr145::SUCCESS;
	res = txrx((UCHAR) 2, (UCHAR) 2, (UCHAR) sensor1, (UCHAR) sensor2, (UCHAR) sensor3, 0, 0, &out[0], &out[1], &out[2], &out[3], &out[4], &out[5], &out[6]);
	return (res);
}

int Msr145::read_acceleration(UCHAR out[7]) const
{
	int res = Msr145::SUCCESS;
	res = txrx(2, 2, Msr145::ACCX, Msr145::ACCY, Msr145::ACCZ, 0, 0, &out[0], &out[1], &out[2], &out[3], &out[4], &out[5], &out[6]);
	return (res);
}

float Msr145::get_pressure(UCHAR outL, UCHAR outH)
{
	float pressure = (float) ((((WORD) outH) << 8) + (WORD) outL) / 10;
	return (pressure);
}

float Msr145::get_temp(UCHAR outL, UCHAR outH)
{
	float temp = (float) ((((WORD) outH) << 8) + (WORD) outL) / 100;
	return (temp);
}

float Msr145::get_hygro(UCHAR outL, UCHAR outH)
{
	float hygro = (float) ((((WORD) outH) << 8) + (WORD) outL) / 100;
	return (hygro);
}

float Msr145::get_acceleration(UCHAR out)
{
	float acc = (float) ((((WORD) out))) * 0.004f;
	return (acc);
}