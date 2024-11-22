/*******************************************************************/
#include "main.h"



/*******************************************************************/
#define BUFFER_SIZE		(uint16_t)64 // ascii table in 32-bit words length




uint32_t writeBuffer[BUFFER_SIZE]; 
uint32_t readBuffer[BUFFER_SIZE];

uint8_t readData_8[BUFFER_SIZE * 4];
uint8_t writeBuffer_bytes[BUFFER_SIZE * 4];

//FATFS fs;
//FRESULT res;
//FIL file;
//FILINFO file_info;

SD_CardInfo SDCardInfo;



FRESULT SD_CardMount(void){
	FRESULT res;
	FATFS fs;
    res = f_mount(&fs, "/", 1);
    
	if(res != FR_OK) {
        printf("--- f_mount() failed, res = %d\r\n", res);
        return FR_INT_ERR;
    }
    printf("+++ f_mount() done!\r\n");
	return FR_OK;
}





FRESULT SD_CardFileOpen(void){
	
	//const char file_name[12] = "fstest00.txt";
	//const char file_name[12] = "crt0.txt";
	const char file_name[12+4] = "fstest00.txt";

	uint8_t readed_data[128];
	unsigned int BytesReaded = 0;
	FRESULT res;
	FIL file;
	FILINFO file_info;
	DIR dir;

	printf("--- Opening Root directory... \n");

    	// ======== f_opendir() return res = FR_OK ==========
	res = f_opendir(&dir, "/");
	if(res != FR_OK){
		printf("--- Opening Root directory FAILED. ErrorCode = %d " , res);
		return res;
	}
	
	// ======= if use f_stat() before f_open(), f_open() return error code 6 (FR_INVALID_NAME)
	//printf("--- Checking for existed file %s on SD-card \n", file_name);
	//res = f_stat(file_name, &file_info);
	//if (res != FR_OK){
	//	printf("--- File %s does not exist on SD-card \n" , file_name);
	//	return res;
	//}
	//printf("+++ FOUND file %s \n", file_info.fname);
	//printf("+++ file SIZE = %d bytes \n", (uint32_t)file_info.fsize);
	////===== till here it's OK



	printf("--- opening TXT-file %s ... \n", file_name);

    res = f_open(&file, file_name, FA_READ); //TODO:  <<-- f_open() return ErrorCode = 6 (FR_INVALID_NAME).   
    if(res != FR_OK) {
		printf("--- Error opening file. ErrorCode res = %d \n", res);
    }
	else{ 
		printf("+++ opening file complete sucessfully. ErrorCode res = %d \n", res);
		
		
		
		printf("--- Starting file reading... \n");

		res = f_read(&file, readed_data, sizeof(readed_data), &BytesReaded);	

		if(res != FR_OK){
			printf("--- reading file FAILED! ErrorCode = %d \n", res);  // return error code = 9 (FR_INVALID_OBJECT)
			f_close(&file);
			return res;
		}
		else{
			printf("+++ File reading successfully! Readed string: \n");
			printf("%s \n", readed_data);
			printf("--- Readed bytes number = %d \n" , BytesReaded );
			f_close(&file);
		}
	}	
	
	return res;
}




FRESULT SD_CardCreateFile(void){
	FIL file;
	FRESULT res;
	DIR dir;
	const char fl_name[12] = "crt0.txt";
	
	// ======== f_opendir() return res = FR_OK ==========
	res = f_opendir(&dir, "/");
	if(res != FR_OK){
		printf("--- Opening Root directory FAILED. ErrorCode = %d " , res);
		return res;
	}

	//======= Error HERE: Error Code = FR_INVALID_NAME ==============
    res = f_open(&file, fl_name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);

	if (res != FR_OK) printf("--- Creating file %s FAILED! Error code = %d \n", fl_name, res);
	else{
		printf("+++ file %s was created successfully \n", fl_name);
		f_close(&file);
	}
	return res;
}




/*******************************************************************/
int main(){

	SD_Error SD_ErrorState = SD_OK;	// SD SDIO error status
	
	FATFS fs;
	FRESULT res;

	RCC_Init();
	APP_GPIO_Init();
	USART1_Init();		// for debug baudrate = 115200

	uint8_t chr = 0x21;	// character ASCII-code for '!'
	

	
	printf("--- System started! ---- \n");

	printf("--- SD-card initialization started! ---- \n");
    // Иницилизация карты
	SD_ErrorState = SD_Init();

	if (SD_ErrorState == SD_OK) {
	
		printf("--- SD-card Getting information! ---- \n");
    	// Получаем информацию о карте
    	SD_GetCardInfo(&SDCardInfo);
    	
		printf("--- SD-card Selection! ---- \n");
    	// Выбор карты и настройка режима работы
    	SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    	SD_SetDeviceMode(SD_POLLING_MODE);

		
		res = SD_CardMount();
		if (res == FR_OK){
			res = SD_CardFileOpen();

			//res = SD_CardCreateFile();
			//if (res == FR_OK) res = SD_CardFileOpen();
			
		}
		else{
			printf("--- SD-card mounting failed... \n");
		}

    	
	}
	else{
		printf("--- SD-card not found!! ---- \n");
	}

	
	while(1)
	{
	}

	return 0;
}



/*******************************************************************/

	