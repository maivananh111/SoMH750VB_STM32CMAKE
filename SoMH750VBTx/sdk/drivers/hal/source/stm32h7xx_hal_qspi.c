/**
  ******************************************************************************
  * @file    stm32h7xx_hal_qspi.c
  * @author  MCD Application Team
  * @brief   QSPI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the QuadSPI interface (QSPI).
  *           + Initialization and de-initialization functions
  *           + Indirect functional mode management
  *           + Memory-mapped functional mode management
  *           + Auto-polling functional mode management
  *           + Interrupts and flags management
  *           + MDMA channel configuration for indirect functional mode
  *           + Errors management and abort functionality
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
 ===============================================================================
                        ##### How to use this driver #####
 ===============================================================================
  [..]
    *** Initialization ***
    ======================
    [..]
      (#) As prerequisite, fill in the HAL_QSPI_MspInit() :
        (++) Enable QuadSPI clock interface with __HAL_RCC_QSPI_CLK_ENABLE().
        (++) Reset QuadSPI Peripheral with __HAL_RCC_QSPI_FORCE_RESET() and __HAL_RCC_QSPI_RELEASE_RESET().
        (++) Enable the clocks for the QuadSPI GPIOS with __HAL_RCC_GPIOx_CLK_ENABLE().
        (++) Configure these QuadSPI pins in alternate mode using HAL_GPIO_Init().
        (++) If interrupt mode is used, enable and configure QuadSPI global
            interrupt with HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ().
        (++) If DMA mode is used, enable the clocks for the QuadSPI MDMA
            with __HAL_RCC_MDMA_CLK_ENABLE(), configure MDMA with HAL_MDMA_Init(),
            link it with QuadSPI handle using __HAL_LINKDMA(), enable and configure
            MDMA global interrupt with HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ().
      (#) Configure the flash size, the clock prescaler, the fifo threshold, the
          clock mode, the sample shifting and the CS high time using the HAL_QSPI_Init() function.

    *** Indirect functional mode ***
    ================================
    [..]
      (#) Configure the command sequence using the HAL_QSPI_Command() or HAL_QSPI_Command_IT()
          functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and if present the size and the address value.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used and if present the number of bytes.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
      (#) If no data is required for the command, it is sent directly to the memory :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_CmdCpltCallback() will be called when the transfer is complete.
      (#) For the indirect write mode, use HAL_QSPI_Transmit(), HAL_QSPI_Transmit_DMA() or
          HAL_QSPI_Transmit_IT() after the command configuration :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_FifoThresholdCallback() will be called when the fifo threshold
             is reached and HAL_QSPI_TxCpltCallback() will be called when the transfer is complete.
         (++) In DMA mode,HAL_QSPI_TxCpltCallback() will be called when the transfer is complete.
      (#) For the indirect read mode, use HAL_QSPI_Receive(), HAL_QSPI_Receive_DMA() or
          HAL_QSPI_Receive_IT() after the command configuration :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_FifoThresholdCallback() will be called when the fifo threshold
             is reached and HAL_QSPI_RxCpltCallback() will be called when the transfer is complete.
	     (++) In DMA mode,HAL_QSPI_RxCpltCallback() will be called when the transfer is complete.

    *** Auto-polling functional mode ***
    ====================================
    [..]
      (#) Configure the command sequence and the auto-polling functional mode using the
          HAL_QSPI_AutoPolling() or HAL_QSPI_AutoPolling_IT() functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and if present the size and the address value.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
         (++) The size of the status bytes, the match value, the mask used, the match mode (OR/AND),
             the polling interval and the automatic stop activation.
      (#) After the configuration :
         (++) In polling mode, the output of the function is done when the status match is reached. The
             automatic stop is activated to avoid an infinite loop.
         (++) In interrupt mode, HAL_QSPI_StatusMatchCallback() will be called each time the status match is reached.

    *** MDMA functional mode ***
    ====================================
    [..]
      (#) Configure the SourceInc and DestinationInc of MDMA parameters in the HAL_QSPI_MspInit() function :
         (++) MDMA settings for write operation :
          (+) The DestinationInc should be MDMA_DEST_INC_DISABLE
          (+) The SourceInc must be a value of MDMA_Source_increment_mode (Except the MDMA_SRC_INC_DOUBLEWORD).
          (+) The SourceDataSize must be a value of MDMA Source data size (Except the MDMA_SRC_DATASIZE_DOUBLEWORD)
              aligned with MDMA_Source_increment_mode .
          (+) The DestDataSize must be a value of MDMA Destination data size (Except the MDMA_DEST_DATASIZE_DOUBLEWORD)
         (++) MDMA settings for read operation :
          (+) The SourceInc should be MDMA_SRC_INC_DISABLE
          (+) The DestinationInc must be a value of MDMA_Destination_increment_mode (Except the MDMA_DEST_INC_DOUBLEWORD).
          (+) The SourceDataSize must be a value of MDMA Source data size (Except the MDMA_SRC_DATASIZE_DOUBLEWORD) .
          (+) The DestDataSize must be a value of MDMA Destination data size (Except the MDMA_DEST_DATASIZE_DOUBLEWORD)
              aligned with MDMA_Destination_increment_mode.
         (++)The buffer Transfer Length (BufferTransferLength) = number of bytes in the FIFO (FifoThreshold) of the Quadspi.
      (#)In case of wrong MDMA setting
        (++) For write operation :
         (+) If the DestinationInc is different to MDMA_DEST_INC_DISABLE , it will be disabled by the HAL_QSPI_Transmit_DMA().
        (++) For read operation :
         (+) If the SourceInc is not set to MDMA_SRC_INC_DISABLE , it will be disabled by the HAL_QSPI_Receive_DMA().

    *** Memory-mapped functional mode ***
    =====================================
    [..]
      (#) Configure the command sequence and the memory-mapped functional mode using the
          HAL_QSPI_MemoryMapped() functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and the size.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
         (++) The timeout activation and the timeout period.
      (#) After the configuration, the QuadSPI will be used as soon as an access on the AHB is done on
          the address range. HAL_QSPI_TimeOutCallback() will be called when the timeout expires.

    *** Errors management and abort functionality ***
    =================================================
    [..]
      (#) HAL_QSPI_GetError() function gives the error raised during the last operation.
      (#) HAL_QSPI_Abort() and HAL_QSPI_Abort_IT() functions aborts any on-going operation and
          flushes the fifo :
         (++) In polling mode, the output of the function is done when the transfer
              complete bit is set and the busy bit cleared.
         (++) In interrupt mode, HAL_QSPI_AbortCpltCallback() will be called when
              the transfer complete bit is set.

    *** Control functions ***
    =========================
    [..]
      (#) HAL_QSPI_GetState() function gives the current state of the HAL QuadSPI driver.
      (#) HAL_QSPI_SetTimeout() function configures the timeout value used in the driver.
      (#) HAL_QSPI_SetFifoThreshold() function configures the threshold on the Fifo of the QSPI IP.
      (#) HAL_QSPI_GetFifoThreshold() function gives the current of the Fifo's threshold
      (#) HAL_QSPI_SetFlashID() function configures the index of the flash memory to be accessed.

    *** Callback registration ***
    =============================================
    [..]
      The compilation define  USE_HAL_QSPI_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.

      Use Functions HAL_QSPI_RegisterCallback() to register a user callback,
      it allows to register following callbacks:
        (+) ErrorCallback : callback when error occurs.
        (+) AbortCpltCallback : callback when abort is completed.
        (+) FifoThresholdCallback : callback when the fifo threshold is reached.
        (+) CmdCpltCallback : callback when a command without data is completed.
        (+) RxCpltCallback : callback when a reception transfer is completed.
        (+) TxCpltCallback : callback when a transmission transfer is completed.
        (+) StatusMatchCallback : callback when a status match occurs.
        (+) TimeOutCallback : callback when the timeout perioed expires.
        (+) MspInitCallback    : QSPI MspInit.
        (+) MspDeInitCallback  : QSPI MspDeInit.
      This function takes as parameters the HAL peripheral handle, the Callback ID
      and a pointer to the user callback function.

      Use function HAL_QSPI_UnRegisterCallback() to reset a callback to the default
      weak (surcharged) function. It allows to reset following callbacks:
        (+) ErrorCallback : callback when error occurs.
        (+) AbortCpltCallback : callback when abort is completed.
        (+) FifoThresholdCallback : callback when the fifo threshold is reached.
        (+) CmdCpltCallback : callback when a command without data is completed.
        (+) RxCpltCallback : callback when a reception transfer is completed.
        (+) TxCpltCallback : callback when a transmission transfer is completed.
        (+) StatusMatchCallback : callback when a status match occurs.
        (+) TimeOutCallback : callback when the timeout perioed expires.
        (+) MspInitCallback    : QSPI MspInit.
        (+) MspDeInitCallback  : QSPI MspDeInit.
      This function) takes as parameters the HAL peripheral handle and the Callback ID.

      By default, after the HAL_QSPI_Init and if the state is HAL_QSPI_STATE_RESET
      all callbacks are reset to the corresponding legacy weak (surcharged) functions.
      Exception done for MspInit and MspDeInit callbacks that are respectively
      reset to the legacy weak (surcharged) functions in the HAL_QSPI_Init
      and  HAL_QSPI_DeInit only when these callbacks are null (not registered beforehand).
      If not, MspInit or MspDeInit are not null, the HAL_QSPI_Init and HAL_QSPI_DeInit
      keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

      Callbacks can be registered/unregistered in READY state only.
      Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
      in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
      during the Init/DeInit.
      In that case first register the MspInit/MspDeInit user callbacks
      using HAL_QSPI_RegisterCallback before calling HAL_QSPI_DeInit
      or HAL_QSPI_Init function.

      When The compilation define USE_HAL_QSPI_REGISTER_CALLBACKS is set to 0 or
      not defined, the callback registering feature is not available
      and weak (surcharged) callbacks are used.

    *** Workarounds linked to Silicon Limitation ***
    ====================================================
    [..]
      (#) Workarounds Implemented inside HAL Driver
         (++) Extra data written in the FIFO at the end of a read transfer

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#if defined(QUADSPI)

/** @addtogroup STM32H7xx_HAL_Driver
  * @{
  */

/** @defgroup QSPI QSPI
  * @brief QSPI HAL module driver
  * @{
  */
#ifdef HAL_QSPI_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/** @defgroup QSPI_Private_Constants QSPI Private Constants
  * @{
  */
#define QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE 0x00000000U                     /*!<Indirect write mode*/
#define QSPI_FUNCTIONAL_MODE_INDIRECT_READ  ((uint32_t)QUADSPI_CCR_FMODE_0) /*!<Indirect read mode*/
#define QSPI_FUNCTIONAL_MODE_AUTO_POLLING   ((uint32_t)QUADSPI_CCR_FMODE_1) /*!<Automatic polling mode*/
#define QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED  ((uint32_t)QUADSPI_CCR_FMODE)   /*!<Memory-mapped mode*/
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup QSPI_Private_Macros QSPI Private Macros
  * @{
  */
#define IS_QSPI_FUNCTIONAL_MODE(MODE) (((MODE) == QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE) || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_INDIRECT_READ)  || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_AUTO_POLLING)   || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED))
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef QSPI_WaitFlagStateUntilTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Flag, FlagStatus State, uint32_t Tickstart, uint32_t Timeout) __attribute__((section(".ram_d3_section")));
static void QSPI_Config(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t FunctionalMode) __attribute__((section(".ram_d3_section")));

/* Exported functions --------------------------------------------------------*/

/** @defgroup QSPI_Exported_Functions QSPI Exported Functions
  * @{
  */

/** @defgroup QSPI_Exported_Functions_Group1 Initialization/de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Initialize the QuadSPI.
      (+) De-initialize the QuadSPI.

@endverbatim
  * @{
  */


/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions_Group2 Input and Output operation functions
  *  @brief QSPI Transmit/Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Handle the interrupts.
      (+) Handle the command sequence.
      (+) Transmit data in blocking, interrupt or DMA mode.
      (+) Receive data in blocking, interrupt or DMA mode.
      (+) Manage the auto-polling functional mode.
      (+) Manage the memory-mapped functional mode.

@endverbatim
  * @{
  */

/**
  * @brief Set the command configuration.
  * @param hqspi QSPI handle
  * @param cmd : structure that contains the command configuration information
  * @param Timeout Timeout duration
  * @note   This function is used only in Indirect Read or Write Modes
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Command(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t Timeout)
{
  HAL_StatusTypeDef status;
  uint32_t tickstart = HAL_GetTick_From_RAM();

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)
  {
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));
  }

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)
  {
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));
  }

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
  {
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));
  }

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;

    /* Update QSPI state */
    hqspi->State = HAL_QSPI_STATE_BUSY;

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, Timeout);

    if (status == HAL_OK)
    {
      /* Call the configuration function */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);

      if (cmd->DataMode == QSPI_DATA_NONE)
      {
        /* When there is no data phase, the transfer start as soon as the configuration is done
        so wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);

        if (status == HAL_OK)
        {
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);

          /* Update QSPI state */
          hqspi->State = HAL_QSPI_STATE_READY;
        }
      }
      else
      {
        /* Update QSPI state */
        hqspi->State = HAL_QSPI_STATE_READY;
      }
    }
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  /* Return function status */
  return status;
}

/**
  * @brief Transmit an amount of data in blocking mode.
  * @param hqspi QSPI handle
  * @param pData pointer to data buffer
  * @param Timeout Timeout duration
  * @note   This function is used only in Indirect Write Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Transmit(QSPI_HandleTypeDef *hqspi, uint8_t *pData, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tickstart = HAL_GetTick_From_RAM();
  __IO uint32_t *data_reg = &hqspi->Instance->DR;

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;

    if(pData != NULL )
    {
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_TX;

      /* Configure counters and size of the handle */
      hqspi->TxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;
      hqspi->TxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;
      hqspi->pTxBuffPtr = pData;

      /* Configure QSPI: CCR register with functional as indirect write */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);

      while(hqspi->TxXferCount > 0U)
      {
        /* Wait until FT flag is set to send data */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_FT, SET, tickstart, Timeout);

        if (status != HAL_OK)
        {
          break;
        }

        *((__IO uint8_t *)data_reg) = *hqspi->pTxBuffPtr;
        hqspi->pTxBuffPtr++;
        hqspi->TxXferCount--;
      }

      if (status == HAL_OK)
      {
        /* Wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);

        if (status == HAL_OK)
        {
          /* Clear Transfer Complete bit */
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);

        }
      }

      /* Update QSPI state */
      hqspi->State = HAL_QSPI_STATE_READY;
    }
    else
    {
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;
      status = HAL_ERROR;
    }
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  return status;
}


/**
  * @brief Receive an amount of data in blocking mode.
  * @param hqspi QSPI handle
  * @param pData pointer to data buffer
  * @param Timeout Timeout duration
  * @note   This function is used only in Indirect Read Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Receive(QSPI_HandleTypeDef *hqspi, uint8_t *pData, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tickstart = HAL_GetTick_From_RAM();
  uint32_t addr_reg = READ_REG(hqspi->Instance->AR);
  __IO uint32_t *data_reg = &hqspi->Instance->DR;

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;

    if(pData != NULL )
    {
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_RX;

      /* Configure counters and size of the handle */
      hqspi->RxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;
      hqspi->RxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;
      hqspi->pRxBuffPtr = pData;

      /* Configure QSPI: CCR register with functional as indirect read */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_READ);

      /* Start the transfer by re-writing the address in AR register */
      WRITE_REG(hqspi->Instance->AR, addr_reg);

      while(hqspi->RxXferCount > 0U)
      {
        /* Wait until FT or TC flag is set to read received data */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, (QSPI_FLAG_FT | QSPI_FLAG_TC), SET, tickstart, Timeout);

        if  (status != HAL_OK)
        {
          break;
        }

        *hqspi->pRxBuffPtr = *((__IO uint8_t *)data_reg);
        hqspi->pRxBuffPtr++;
        hqspi->RxXferCount--;
      }

      if (status == HAL_OK)
      {
        /* Wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);

        if  (status == HAL_OK)
        {
          /* Clear Transfer Complete bit */
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);

        }
      }

      /* Update QSPI state */
      hqspi->State = HAL_QSPI_STATE_READY;
    }
    else
    {
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;
      status = HAL_ERROR;
    }
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  return status;
}

/**
  * @brief  Configure the QSPI Automatic Polling Mode in blocking mode.
  * @param  hqspi QSPI handle
  * @param  cmd structure that contains the command configuration information.
  * @param  cfg structure that contains the polling configuration information.
  * @param  Timeout Timeout duration
  * @note   This function is used only in Automatic Polling Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_AutoPolling(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, QSPI_AutoPollingTypeDef *cfg, uint32_t Timeout)
{
  HAL_StatusTypeDef status;
  uint32_t tickstart = HAL_GetTick_From_RAM();

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)
  {
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));
  }

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)
  {
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));
  }

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
  {
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));
  }

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));

  assert_param(IS_QSPI_INTERVAL(cfg->Interval));
  assert_param(IS_QSPI_STATUS_BYTES_SIZE(cfg->StatusBytesSize));
  assert_param(IS_QSPI_MATCH_MODE(cfg->MatchMode));

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;

    /* Update state */
    hqspi->State = HAL_QSPI_STATE_BUSY_AUTO_POLLING;

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, Timeout);

    if (status == HAL_OK)
    {
      /* Configure QSPI: PSMAR register with the status match value */
      WRITE_REG(hqspi->Instance->PSMAR, cfg->Match);

      /* Configure QSPI: PSMKR register with the status mask value */
      WRITE_REG(hqspi->Instance->PSMKR, cfg->Mask);

      /* Configure QSPI: PIR register with the interval value */
      WRITE_REG(hqspi->Instance->PIR, cfg->Interval);

      /* Configure QSPI: CR register with Match mode and Automatic stop enabled
      (otherwise there will be an infinite loop in blocking mode) */
      MODIFY_REG(hqspi->Instance->CR, (QUADSPI_CR_PMM | QUADSPI_CR_APMS),
               (cfg->MatchMode | QSPI_AUTOMATIC_STOP_ENABLE));

      /* Call the configuration function */
      cmd->NbData = cfg->StatusBytesSize;
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_AUTO_POLLING);

      /* Wait until SM flag is set to go back in idle state */
      status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_SM, SET, tickstart, Timeout);

      if (status == HAL_OK)
      {
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_SM);

        /* Update state */
        hqspi->State = HAL_QSPI_STATE_READY;
      }
    }
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  /* Return function status */
  return status;
}

/**
  * @brief  Configure the Memory Mapped mode.
  * @param  hqspi QSPI handle
  * @param  cmd structure that contains the command configuration information.
  * @param  cfg structure that contains the memory mapped configuration information.
  * @note   This function is used only in Memory mapped Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_MemoryMapped(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, QSPI_MemoryMappedTypeDef *cfg)
{
  HAL_StatusTypeDef status;
  uint32_t tickstart = HAL_GetTick_From_RAM();

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)
  {
  assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));
  }

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)
  {
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));
  }

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
  {
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));
  }

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));

  assert_param(IS_QSPI_TIMEOUT_ACTIVATION(cfg->TimeOutActivation));

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;

    /* Update state */
    hqspi->State = HAL_QSPI_STATE_BUSY_MEM_MAPPED;

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);

    if (status == HAL_OK)
    {
      /* Configure QSPI: CR register with timeout counter enable */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_TCEN, cfg->TimeOutActivation);

    if (cfg->TimeOutActivation == QSPI_TIMEOUT_COUNTER_ENABLE)
      {
        assert_param(IS_QSPI_TIMEOUT_PERIOD(cfg->TimeOutPeriod));

        /* Configure QSPI: LPTR register with the low-power timeout value */
        WRITE_REG(hqspi->Instance->LPTR, cfg->TimeOutPeriod);

        /* Clear interrupt */
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TO);

        /* Enable the QSPI TimeOut Interrupt */
        __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TO);
      }

      /* Call the configuration function */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED);
    }
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  /* Return function status */
  return status;
}


/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions_Group3 Peripheral Control and State functions
  *  @brief   QSPI control and State functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral Control and State functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Check in run-time the state of the driver.
      (+) Check the error code set during last operation.
      (+) Abort any operation.


@endverbatim
  * @{
  */

/**
  * @brief  Return the QSPI handle state.
  * @param  hqspi QSPI handle
  * @retval HAL state
  */
HAL_QSPI_StateTypeDef HAL_QSPI_GetState(QSPI_HandleTypeDef *hqspi)
{
  /* Return QSPI handle state */
  return hqspi->State;
}

/**
* @brief  Return the QSPI error code.
* @param  hqspi QSPI handle
* @retval QSPI Error Code
*/
uint32_t HAL_QSPI_GetError(QSPI_HandleTypeDef *hqspi)
{
  return hqspi->ErrorCode;
}

/**
* @brief  Abort the current transmission.
* @param  hqspi QSPI handle
* @retval HAL status
*/
HAL_StatusTypeDef HAL_QSPI_Abort(QSPI_HandleTypeDef *hqspi)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tickstart = HAL_GetTick_From_RAM();

  /* Check if the state is in one of the busy states */
  if (((uint32_t)hqspi->State & 0x2U) != 0U)
  {
    /* Process unlocked */
    __HAL_UNLOCK(hqspi);

    if (__HAL_QSPI_GET_FLAG(hqspi, QSPI_FLAG_BUSY) != RESET)
    {
      /* Configure QSPI: CR register with Abort request */
      SET_BIT(QUADSPI->CR, QUADSPI_CR_ABORT);
      
      /* Wait until TC flag is set to go back in idle state */
      status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, hqspi->Timeout);
      
      if (status == HAL_OK)
      {
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);
        
        /* Wait until BUSY flag is reset */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);
      }

      if (status == HAL_OK)
      {
        /* Reset functional mode configuration to indirect write mode by default */
        CLEAR_BIT(hqspi->Instance->CCR, QUADSPI_CCR_FMODE);
        
        /* Update state */
        hqspi->State = HAL_QSPI_STATE_READY;
      }
    }
    else
    {
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_READY;
    }
  }

  return status;
}


/** @brief Set QSPI timeout.
  * @param  hqspi QSPI handle.
  * @param  Timeout Timeout for the QSPI memory access.
  * @retval None
  */
void HAL_QSPI_SetTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
  hqspi->Timeout = Timeout;
}

/** @brief Set QSPI Fifo threshold.
  * @param  hqspi QSPI handle.
  * @param  Threshold Threshold of the Fifo (value between 1 and 16).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_SetFifoThreshold(QSPI_HandleTypeDef *hqspi, uint32_t Threshold)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    /* Synchronize init structure with new FIFO threshold value */
    hqspi->Init.FifoThreshold = Threshold;

    /* Configure QSPI FIFO Threshold */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_FTHRES,
               ((hqspi->Init.FifoThreshold - 1U) << QUADSPI_CR_FTHRES_Pos));
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  /* Return function status */
  return status;
}

/** @brief Get QSPI Fifo threshold.
  * @param  hqspi QSPI handle.
  * @retval Fifo threshold (value between 1 and 16)
  */
uint32_t HAL_QSPI_GetFifoThreshold(QSPI_HandleTypeDef *hqspi)
{
  return ((READ_BIT(hqspi->Instance->CR, QUADSPI_CR_FTHRES) >> QUADSPI_CR_FTHRES_Pos) + 1U);
}

/** @brief  Set FlashID.
  * @param  hqspi QSPI handle.
  * @param  FlashID Index of the flash memory to be accessed.
  *                   This parameter can be a value of @ref QSPI_Flash_Select.
  * @note   The FlashID is ignored when dual flash mode is enabled.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_SetFlashID(QSPI_HandleTypeDef *hqspi, uint32_t FlashID)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameter */
  assert_param(IS_QSPI_FLASH_ID(FlashID));

  /* Process locked */
  __HAL_LOCK(hqspi);

  if(hqspi->State == HAL_QSPI_STATE_READY)
  {
    /* Synchronize init structure with new FlashID value */
    hqspi->Init.FlashID = FlashID;

    /* Configure QSPI FlashID */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_FSEL, FlashID);
  }
  else
  {
    status = HAL_BUSY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);

  /* Return function status */
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup QSPI_Private_Functions QSPI Private Functions
  * @{
  */

/**
  * @brief  Wait for a flag state until timeout.
  * @param  hqspi QSPI handle
  * @param  Flag Flag checked
  * @param  State Value of the flag expected
  * @param  Tickstart Tick start value
  * @param  Timeout Duration of the timeout
  * @retval HAL status
  */
static HAL_StatusTypeDef QSPI_WaitFlagStateUntilTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Flag,
                                                        FlagStatus State, uint32_t Tickstart, uint32_t Timeout)
{
  /* Wait until flag is in expected state */
  while((__HAL_QSPI_GET_FLAG(hqspi, Flag)) != State)
  {
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)
    {
      if(((HAL_GetTick_From_RAM() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        hqspi->State     = HAL_QSPI_STATE_ERROR;
        hqspi->ErrorCode |= HAL_QSPI_ERROR_TIMEOUT;

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  Configure the communication registers.
  * @param  hqspi QSPI handle
  * @param  cmd structure that contains the command configuration information
  * @param  FunctionalMode functional mode to configured
  *           This parameter can be one of the following values:
  *            @arg QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE: Indirect write mode
  *            @arg QSPI_FUNCTIONAL_MODE_INDIRECT_READ: Indirect read mode
  *            @arg QSPI_FUNCTIONAL_MODE_AUTO_POLLING: Automatic polling mode
  *            @arg QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED: Memory-mapped mode
  * @retval None
  */
static void QSPI_Config(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t FunctionalMode)
{
  assert_param(IS_QSPI_FUNCTIONAL_MODE(FunctionalMode));

  if ((cmd->DataMode != QSPI_DATA_NONE) && (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED))
  {
    /* Configure QSPI: DLR register with the number of data to read or write */
    WRITE_REG(hqspi->Instance->DLR, (cmd->NbData - 1U));
  }

  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)
  {
    if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
    {
      /* Configure QSPI: ABR register with alternate bytes value */
      WRITE_REG(hqspi->Instance->ABR, cmd->AlternateBytes);

      if (cmd->AddressMode != QSPI_ADDRESS_NONE)
      {
        /*---- Command with instruction, address and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |
                                         cmd->AddressSize | cmd->AddressMode | cmd->InstructionMode |
                                         cmd->Instruction | FunctionalMode));

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
        {
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);
        }
      }
      else
      {
        /*---- Command with instruction and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |
                                         cmd->AddressMode | cmd->InstructionMode |
                                         cmd->Instruction | FunctionalMode));
      }
    }
    else
    {
      if (cmd->AddressMode != QSPI_ADDRESS_NONE)
      {
        /*---- Command with instruction and address ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateByteMode | cmd->AddressSize | cmd->AddressMode |
                                         cmd->InstructionMode | cmd->Instruction | FunctionalMode));

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
        {
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);
        }
      }
      else
      {
        /*---- Command with only instruction ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateByteMode | cmd->AddressMode |
                                         cmd->InstructionMode | cmd->Instruction | FunctionalMode));
      }
    }
  }
  else
  {
    if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
    {
      /* Configure QSPI: ABR register with alternate bytes value */
      WRITE_REG(hqspi->Instance->ABR, cmd->AlternateBytes);

      if (cmd->AddressMode != QSPI_ADDRESS_NONE)
      {
        /*---- Command with address and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |
                                         cmd->AddressSize | cmd->AddressMode |
                                         cmd->InstructionMode | FunctionalMode));

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
        {
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);
        }
      }
      else
      {
        /*---- Command with only alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |
                                         cmd->AddressMode | cmd->InstructionMode | FunctionalMode));
      }
    }
    else
    {
      if (cmd->AddressMode != QSPI_ADDRESS_NONE)
      {
        /*---- Command with only address ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                         cmd->AlternateByteMode | cmd->AddressSize |
                                         cmd->AddressMode | cmd->InstructionMode | FunctionalMode));

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
        {
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);
        }
      }
      else
      {
        /*---- Command with only data phase ----*/
        if (cmd->DataMode != QSPI_DATA_NONE)
        {
          /* Configure QSPI: CCR register with all communications parameters */
          WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |
                                           cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |
                                           cmd->AlternateByteMode | cmd->AddressMode |
                                           cmd->InstructionMode | FunctionalMode));
        }
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_QSPI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(QUADSPI) */
