#include "io_expander.h"

// use IO_EXPANDER_GPIO_BASE as base

/*******************************************************************************
* Function Name: right_button_pressed
********************************************************************************
* Summary: Returns true if the right button was pressed and we want the bear to jump
*  Returns: true if the button was pressed, false otherwise
*******************************************************************************/

bool right_button_pressed(void){
	return io_expander_read_reg(MCP23017_GPIOB_R);
	
}

/*******************************************************************************
* Function Name: configure_buttons
********************************************************************************
* Summary: Allows the IO Expander to read input from the push buttons
* Returns:
*  i2c_status_t the status of the I2C peripheral
*******************************************************************************/
i2c_status_t config_buttons(void){
	uint8_t readReg; //temp variable used when clearing interrupts 
  i2c_status_t status; //just so we can set when we get byte
	
	
	//Setup GPIO Port F
	gpio_enable_port(IO_EXPANDER_IRQ_GPIO_BASE);
	gpio_config_digital_enable(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	gpio_config_enable_input(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	gpio_config_falling_edge_irq(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);

	NVIC_SetPriority(gpio_get_irq_num(IO_EXPANDER_IRQ_GPIO_BASE),1);

	NVIC_EnableIRQ(gpio_get_irq_num(IO_EXPANDER_IRQ_GPIO_BASE));
 
	//Clear Port A and setup Port B to be an input
	io_expander_write_reg(MCP23017_IODIRA_R, 0x00);
	io_expander_write_reg(MCP23017_IODIRB_R, 0x0F);
	
	//Enable Pull-ups.
  io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);
  
  //Clear Interrupts
  readReg = io_expander_read_reg(MCP23017_INTCAPB_R);
	readReg = io_expander_read_reg(MCP23017_GPIOB_R);

  //Configure pins 0-3 to handle interrupts
  io_expander_write_reg(MCP23017_GPINTENB_R, 0x0F);
  io_expander_write_reg(MCP23017_INTCONB_R, 0x00);

  //Clear interrupts in GPIO B
  readReg = io_expander_read_reg(MCP23017_GPIOB_R);    
	
  return status;
}


/*******************************************************************************
* Function Name: io_expander_init
********************************************************************************
* Summary: Initializes our I2C peripheral for the IO Expander
* Returns:
*  True if successful, false otherwise
*******************************************************************************/

bool io_expander_init(void) {
	if(!gpio_enable_port(IO_EXPANDER_GPIO_BASE))
		return false;
	
	//Part 1: SCL Configuration
	if(!gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN))
		return false;
	if(!gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN))
    return false;  
  if(!gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL))
    return false;
	
	//Part 2. SDA Configuration
	if(!gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN))
		return false;
	if(!gpio_config_open_drain(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN))
    return false;
	if(!gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN))
    return false;  
  if(!gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL))
    return false;
	
	//Part 3. I2C Configuration
	if(initializeI2CMaster(IO_EXPANDER_I2C_BASE) != I2C_OK)
		return false;
	
	return true;
}

/*******************************************************************************
* Function Name: io_expander_write_reg
********************************************************************************
* Summary: Writes the specified data to the specified register address in the I2C
* Returns:
*  Nothing
*******************************************************************************/

void io_expander_write_reg(uint8_t reg, uint8_t data) {
  
	//Change I2C address
	i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

  //Send the register address
  i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START|I2C_MCS_RUN);

  //Send the data
  i2cSendByte(IO_EXPANDER_I2C_BASE, data, I2C_MCS_RUN|I2C_MCS_STOP);
}

/*******************************************************************************
* Function Name: io_expander_read_reg
********************************************************************************
* Summary: Reads the specified data at the specified reg in the I2C
* Returns:
*  uint8_t the data at the reg
*******************************************************************************/

uint8_t io_expander_read_reg(uint8_t reg) {
	uint8_t data; //to be read
	i2c_status_t status; //just so we can set when we get byte
	
	//Change I2C address
	i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE); 

	//Send the register address 
	i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START|I2C_MCS_RUN);

	//Set I2C address
	i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_READ);

	//Read from register
	status = i2cGetByte(IO_EXPANDER_I2C_BASE, &data, I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_STOP);
	
	return data;
}