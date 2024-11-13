/*******************************************************************/
#include "main.h"



/*******************************************************************/
#define BUFFER_SIZE		(uint16_t)64 // ascii table in 32-bit words length




uint32_t writeBuffer[BUFFER_SIZE]; 
uint32_t readBuffer[BUFFER_SIZE];

uint8_t readData_8[BUFFER_SIZE * 4];
uint8_t writeBuffer_bytes[BUFFER_SIZE * 4];



SD_CardInfo SDCardInfo;







FRESULT SD_CardMount(void){
	FRESULT res;
	FATFS fs;
    res = f_mount(&fs, "/", 1);
    
	if(res != FR_OK) {
        printf("---- f_mount() failed, res = %d\r\n", res);
        return FR_INT_ERR;
    }
    printf("++++ f_mount() done!\r\n");
	return FR_OK;
}





FRESULT SD_CardFileOpen(void){
	
	const unsigned char fname[12] = "fstest00.txt";
	uint8_t readed_data[128];
	unsigned int BytesReaded = 0;


	printf("---- opening TXT-file %s... \n ", fname);
	
	FIL file;
    FRESULT res = f_open(&file, fname, FA_READ);
    if(res != FR_OK) {
		printf("--- Error opening file. ErrorCode res = %d \n", res);
    }
	else{ 
		printf("+++ opening file complete sucessfully. ErrorCode res = %d \n", res);
		
		printf("--- Starting file reading... \n");
		res = f_read(&file, readed_data, sizeof(readed_data), &BytesReaded);
		if(res != FR_OK){
			printf("---- reading file FAILED! ErrorCode = %d \n", res);
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
	const char fname[12] = "crt_test.txt";
    FRESULT res = f_open(&file, fname, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	if (res != FR_OK) printf("--- Creating file %s FAILED! Error code = %d \n", fname, res);
	else{
		printf("++++ file %s was created successfully \n", fname);
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
	

	
	printf("---- System started! ---- \n");

	printf("----- SD-card initialization started! ---- \n");
    // Иницилизация карты
	SD_ErrorState = SD_Init();

	if (SD_ErrorState == SD_OK) {
	
		printf("----- SD-card Getting information! ---- \n");
    	// Получаем информацию о карте
    	SD_GetCardInfo(&SDCardInfo);
    	
		printf("----- SD-card Selecting! ---- \n");
    	// Выбор карты и настройка режима работы
    	SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    	SD_SetDeviceMode(SD_POLLING_MODE);

		
		res = SD_CardMount();
		if (res == FR_OK){
			res = SD_CardFileOpen();

			//res = SD_CardCreateFile();
			
		}
		else{
			printf("---- SD-card mounting failed... \n");
		}

    	
	}
	else{
		printf("----- SD-card not found!! ---- \n");
	}

	
	while(1)
	{
	}

	return 0;
}



/*******************************************************************/

	