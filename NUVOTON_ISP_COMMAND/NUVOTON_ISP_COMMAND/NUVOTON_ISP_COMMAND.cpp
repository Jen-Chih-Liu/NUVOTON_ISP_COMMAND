#include "stdafx.h"
#include "ISP_COMMAND.h"
#include <time.h>
#include "cmdline.h"
#include "stdio.h"
#define bin_inside 
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start_time, end_time;
	float total_time = 0;
	cmdline::parser theArgs;
#ifdef bin_inside	

#else
	theArgs.add<string>("AFN", 'f', "AP file name", false, "");
#endif
	theArgs.add<string>("COM", 'c', "com port number", false, ""); 
	theArgs.add<bool>("USB", 'u', "USB port", false, false);
	theArgs.add<bool>("Time", 't', "show progamming", false, false);
	theArgs.set_program_name(argv[0]);

	theArgs.parse_check(argc, &argv[0]);

#ifdef bin_inside	

#else
	try {
		printf("APROM FILE NAME: %s\n", theArgs.get<string>("AFN").c_str());
	}
	catch (const std::exception &e) {}
#endif
#if 0
	try {
		printf("COM PORT NAME: %s\n", theArgs.get<string>("COM").c_str());
	}
	catch (const std::exception &e) {}

	try {
		printf("USB: %d\n", theArgs.get<bool>("USB"));
	}
	catch (const std::exception &e) {}

	try {
		printf("Time: %d\n", theArgs.get<bool>("Time"));
	}
	catch (const std::exception &e) {}

	if (theArgs.get<bool>("Time") == 1)
	{
		clock_t start_time, end_time;
		float total_time = 0;
		start_time = clock(); /* mircosecond */
	}
#endif
	if (theArgs.get<bool>("Time") == 1)
	{

		start_time = clock(); /* mircosecond */
	}

	if ((theArgs.get<bool>("USB") == 0) && (theArgs.get<string>("COM") == ""))
	{
		printf("please input -u 1 or -c com[n] \n\r");
		return 0;
	}
	if ((theArgs.get<bool>("USB") == 1) && (theArgs.get<string>("COM") != ""))
	{
		printf("please donn't input -u 1 and -c com[n] in same time\n\r");
		return 0;
	}
	 
	ISP_COMMAND *ISP = new ISP_COMMAND();

#ifdef bin_inside	
	ISP->Copy_BIN_Array();
#else
	if (theArgs.get<string>("AFN") != "")
	{
		std::string AFNStr = theArgs.get<std::string>("AFN");
		_TCHAR fileanme[256]; // 設定一個足夠大的緩衝區
		_tcscpy(fileanme, _T(AFNStr.c_str()));
		if (ISP->File_Open_APROM(fileanme) == RES_FILE_NO_FOUND)
		{
			printf("FILE NO FOUND\n\r");
			delete ISP;
			return 0;
		}
		printf("File name: %s\n\r", fileanme);
		printf("File size: %d\n\r", ISP->file_size);
		printf("File checksum: %d\n\r", ISP->file_checksum);
	}
	else {
		printf("Please input -AFN [FILE NAME]\n\r");
		delete ISP;
	}
#endif

//uart interface
	if (theArgs.get<string>("COM") != "")
	{
		std::string comPortStr = theArgs.get<std::string>("COM");
		_TCHAR comPort[256]; // 設定一個足夠大的緩衝區

		_tcscpy(comPort, _T(comPortStr.c_str()));
		if (ISP->OPEN_COMPORT(comPort) != RES_CONNECT)
		{
			printf("COMPORT NO FOUND, plase check your comport\n\r");
			goto UART_EXIT;
		}
		printf("Polling MCU!!\n\r");
		if (ISP->CHECK_UART_LINK() == RES_DETECT_MCU)
			printf("Detect MCU!!\n\r");
		else
		{
			printf("No detect MCU!!\n\r");
			goto UART_EXIT;
		}
		ISP->SN_PACKAGE_UART();
		ISP->SN_PACKAGE_UART();
		unsigned int temp = ISP->READFW_VERSION_UART();

		printf("FW version: 0x%x \n\r", temp);

		if (ISP->READ_PID_UART() == RES_FALSE)
		{
			printf("CHIP NO FOUND\n\r");
			goto UART_EXIT;
		}

		//READ CONFIG
		printf("config \n\r");
		ISP->READ_CONFIG_UART();

		//test updata aprom
		ISP->UPDATE_APROM_UART();

		//reboot mcu to aprom
		ISP->RUN_TO_APROM_UART();

	UART_EXIT:
		ISP->CLOSE_UART_PORT();
		delete ISP;
	}
	if (theArgs.get<bool>("USB") == 1)
	{

		if (ISP->OPEN_USBPORT() != RES_CONNECT)
		{
			printf("USB NO FOUND\n\r");
			goto USB_EXIT;
		}

		//START ;
		ISP->SN_PACKAGE_USB();
		ISP->SN_PACKAGE_USB();
		//CHECK FW
		printf("FW version: 0x%x \n\r", ISP->READFW_VERSION_USB());
		//CHECK PID
		if (ISP->READ_PID_USB() == RES_FALSE)
		{
			printf("CHIP NO FOUND\n\r");
			goto USB_EXIT;
		}
		//READ CONFIG
		printf("config \n\r");
		ISP->READ_CONFIG_USB();

		//test updata aprom
		ISP->UPDATE_APROM_USB();

		//reboot mcu to aprom
		ISP->RUN_TO_APROM_USB();

	USB_EXIT:
		//close usb port
		ISP->CLOSE_USB_PORT();

		delete ISP;
	}

	if (theArgs.get<bool>("Time") == 1)
	{
		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		total_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;

		printf("Time : %f sec \n", total_time);
	}
	return 0;
	
}
