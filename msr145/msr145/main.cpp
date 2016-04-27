#include "stdafx.h"
#include "Msr145.h"

int main()
{
	UCHAR output[7] = { 0 };

	// Create Msr145 object.
	Msr145 m = Msr145();
	int res = m.load_dll();
	if (res == Msr145::FAIL_DLL)
	{
		std::cout << "Loading DLL failed. Please check if the DLL is in the same folder as the exe." << std::endl;
		system("pause");
		return (-1);
	}
	else if (res == Msr145::FAIL_FUNCTION)
	{
		std::cout << "Loading DLL symbols failed. Maybe the DLL is corrupted." << std::endl;
		system("pause");
		return (-1);
	}

	std::cout << "DLL Version: " << m.get_dll_version() << std::endl;
	std::cout << "DLL Open Port: " << m.open_com_port() << std::endl;
	std::cout << "DLL Wake up: " << m.wake_up() << std::endl;
	std::cout << "Start measures: " << m.init_measurement(output) << std::endl;
	std::cout << "DLL Start measures output: " << (int) output[0] << std::endl;

	for (int i = 0; i < 10; i++)
	{
		int res = m.read_measurement(Msr145::ACCX, Msr145::ACCY, Msr145::ACCZ, output);
		float x = Msr145::get_acceleration(output[1]);
		float y = Msr145::get_acceleration(output[2]);
		float z = Msr145::get_acceleration(output[3]);
		res = m.read_measurement(Msr145::PRESSURE, Msr145::TEMP, Msr145::HYGRO, output);
		float p = Msr145::get_pressure(output[1], output[2]);
		float t = Msr145::get_temp(output[3], output[4]);
		float h = Msr145::get_hygro(output[5], output[6]);
		std::cout << "x=" << x << " y=" << y << " z=" << z << " p=" << p << " t=" << t << " h=" << h << std::endl;
	}

	std::cout << "DLL Close Port: " << m.close_com_port() << std::endl;
	system("pause");
	return (0);
}

