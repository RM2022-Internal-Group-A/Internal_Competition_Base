#include <main.h>

#include <FreeRTOS.h>
#include <task.h>
#include "function.h"

static StaticTask_t blinkTask;
static StackType_t blinkTaskStack[128];

//static StaticTask_t ReceiveTask;
//static StackType_t ReceiveTaskStack[128];

void blinkTaskFunc(void *param);
//void ReceiveFunc(void *param);

extern "C"
{
    void startUserTasks()
    {
        xTaskCreateStatic(blinkTaskFunc, "blink", 128, NULL, 1, blinkTaskStack, &blinkTask);
        //xTaskCreateStatic(ReceiveFunc, "receive", 128, NULL, 1, ReceiveTaskStack, &ReceiveTask);
    }
}

static volatile uint32_t blinkCounter = 1;
void blinkTaskFunc(void *param)
{
    static CAN_RxHeaderTypeDef rxHeader;                         // CAN Bus Transmit Header
    CAN_TxHeaderTypeDef txHeader;                                // CAN Bus Receive Header
    static volatile uint8_t canRX[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // CAN Bus Receive Buffer
    static volatile uint16_t encoder = 0;
    CAN_FilterTypeDef canfil; // CAN Bus Filter
    uint32_t canMailbox;

    canfil.FilterBank = 0;
    canfil.FilterMode = CAN_FILTERMODE_IDMASK;
    canfil.FilterFIFOAssignment = CAN_RX_FIFO0;
    canfil.FilterIdHigh = 0;
    canfil.FilterIdLow = 0;
    canfil.FilterMaskIdHigh = 0;
    canfil.FilterMaskIdLow = 0;
    canfil.FilterScale = CAN_FILTERSCALE_32BIT;
    canfil.FilterActivation = ENABLE;
    canfil.SlaveStartFilterBank = 14;

    txHeader.DLC = 8; // Number of bites to be transmitted max- 8
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.StdId = 0x200;
    txHeader.ExtId = 0x0;
    txHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_ConfigFilter(&hcan, &canfil);
    HAL_CAN_Start(&hcan);

    uint8_t msg[8] = {0, 255, 0, 0, 0, 0, 0, 0};
    static volatile HAL_StatusTypeDef canTxStatus = HAL_OK;
    while (1)
    {
        canTxStatus = HAL_CAN_AddTxMessage(&hcan, &txHeader, msg, &canMailbox);

        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, (uint8_t *)canRX);
        encoder = canRX[0] << 8 | canRX[1];
        blinkCounter++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

uint8_t data_in[16];

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    RemoteDataProcess(data_in);
    USART2_IRQHandler();
    HAL_UART_Receive_IT(&huart2, data_in, sizeof(data_in));

}

/*void ReceiveFunc(void *param)
{
    RC_Init();
    uint8_t data_in[16];

        RemoteDataProcess(data_in);
        USART2_IRQHandler(void);
        HAL_UART_Receive_IT(&huart2, data_in, sizeof(data_in));
    

    

        HAL_UART_Receive_IT(&huart2, data_in, sizeof(data_in));
    
}
*/