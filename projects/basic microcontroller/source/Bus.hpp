#include <cstdint>

#include "L1_Peripheral/lpc40xx/gpio.hpp"
class Bus	
{
public:
	void Initialize()
	{
		//SettingeachpinasOpenDrain
		ad[0].GetPin().SetAsOpenDrain();
		ad[1].GetPin().SetAsOpenDrain();
		ad[2].GetPin().SetAsOpenDrain();
		ad[3].GetPin().SetAsOpenDrain();
		ad[4].GetPin().SetAsOpenDrain();
		ad[5].GetPin().SetAsOpenDrain();
		ad[6].GetPin().SetAsOpenDrain();
		ad[7].GetPin().SetAsOpenDrain();
		write_enable.SetAsOutput();
		ale.SetAsOutput();
		m_io.SetAsOutput();
		write.SetAsOutput();
		read.SetAsOutput();
		interrupt.SetAsInput();
	}

	void put_address(uint8_t address)
	{
		write_enable.SetHigh();
		for(int i = 7; i >= 0; i--)
		{
			ad[i].SetAsOutput();
   			if(address & (1 << (i))) 
			{
				ad[i].SetHigh();
			} 
    		else
			{
				ad[i].SetLow();
			}
		}
		sjsu::Delay(80ns);
		ale.SetHigh();
		sjsu::Delay(80ns);
		ale.SetLow();
	}

	void put_data(uint8_t data)
	{
		write_enable.SetHigh();

		for(int i = 7; i >= 0; i--)
		{
			ad[i].SetAsOutput();
   			if(data & (1 << (i))) 
			{
				ad[i].SetHigh();
			} 
    		else
			{
				ad[i].SetLow();
			}
		}
		sjsu::Delay(80ns);		
	}
	
	void put_data_address(uint8_t address, uint8_t data) 
	{
		write_enable.SetHigh();
		for(int i = 7; i >= 0; i--)
		{
			ad[i].SetAsOutput();
   			if(address & (1 << (i))) 
			{
				ad[i].SetHigh();
			} 
    		else
			{
				ad[i].SetLow();
			}
		}
		sjsu::Delay(80ns);
		ale.SetHigh(ns);
		sjsu::Delay(80ns);
		ale.SetLow();

		for(int i = 7; i >= 0; i--)
		{
			ad[i].SetAsOutput();		
   			if(data & (1 << (i))) 
			{
				ad[i].SetHigh();
			} 
    		else
			{
				ad[i].SetLow();
			}
		}
	}
 
	void io_write(uint8_t address, uint8_t data)
	{
		m_io.SetLow();
		write.SetHigh();
		read.SetHigh();
		put_data_address(address, data);
		sjsu::Delay(80ns);
		write.SetLow();
		sjsu::Delay(80ns);
		write.SetHigh();
		sjsu::Delay(80ns);		
	}

	void test()
	{
		read.SetLow();
		read.SetHigh();
	}

	uint8_t io_read(uint8_t address)
	{
		m_io.SetLow();
		write.SetHigh();
		read.SetHigh();
		uint8_t read_data;
		put_address(address);
		sjsu::Delay(80ns);
		write_enable.SetLow();
		sjsu::Delay(100ns);
		ad[0].SetAsInput();
		ad[1].SetAsInput();
		ad[2].SetAsInput();
		ad[3].SetAsInput();
		ad[4].SetAsInput();	
		ad[5].SetAsInput();
		ad[6].SetAsInput();
		ad[7].SetAsInput();
		read.SetLow();
		sjsu::Delay(80ns);
		for(int i = 0; i < 8; i++)
		{
			read_data = read_data | (ad[i].Read() << i);
		}		
		read.SetHigh();
		sjsu::Delay(80ns);
		return read_data;
		sjsu::Delay(80ns);
	}

	void polling(uint8_t address1, uint8_t address2)
	{
		uint8_t read_data;
		uint8_t key;
		int i = 0;
		bool n;
		n = true;
		while(n == true)
		{
    		if(i == 0)
    		{
      			io_write(address1, 1);
      			read_data = io_read(address2);
      			key = check_key(1, read_data); 
    		}
    		else if(i == 1)
    		{
      			io_write(address1, 2);
     			read_data = io_read(address2);
      			key = check_key(2, read_data);     
    		}
			else if(i == 2)
			{
				io_write(address1, 4);
     			read_data = io_read(address2);
      			key = check_key(4, read_data); 
			} 			
			else if(i == 3)
			{
				io_write(address1, 8);
     			read_data = io_read(address2);
      			key = check_key(8, read_data); 
			}
			if(key != 255)
			{
				n = false;
			}
			else
			{
				i = i + 1;
				if(i == 4)
				{
					i = 0;
				}
			}
		}
		switch(key)
    	{
      	case 0:
        	printf("Pressed Key: 0\n");
        	break;
      	case 1:
        	printf("Pressed Key: 1\n");
        	break;
      	case 2:
        	printf("Pressed Key: 2\n");
        	break;
      	case 3:
        	printf("Pressed Key: 3\n");
        	break;
      	case 4:
        	printf("Pressed Key: 4\n");
        	break;
      	case 5:
        	printf("Pressed Key: 5\n");
        	break;
      	case 6:
        	printf("Pressed Key: 6\n");
        	break;
      	case 7:
        	printf("Pressed Key: 7\n");
        	break;
      	case 8:
        	printf("Pressed Key: 8\n");
        	break;
      	case 9:
        	printf("Pressed Key: 9\n");
        	break;
      	case 10:
        	printf("Pressed Key: A\n");
        	break;
      	case 11:
        	printf("Pressed Key: B\n");
        	break;
      	case 12:
        	printf("Pressed Key: C\n");
        	break;
      	case 13:
        	printf("Pressed Key: D\n");
        	break;
      	case 14:
        	printf("Pressed Key: *\n");
        	break;
      	case 15:
        	printf("Pressed Key: #\n");
        	break;      
   		}		
	}


	uint8_t check_key(uint8_t row_data, uint8_t col_data)
	{
		uint8_t key = 255;
		if(row_data == 1)
		{
			if(col_data == 1)
			{
				key = 1;
			}
			else if(col_data == 2)
			{
				key = 2;
			}
			else if(col_data == 4)
			{
				key = 3;
			}
			else if(col_data == 8)
			{
				key = 10;
			}
		}
		else if(row_data == 2)
		{
			if(col_data == 1)
			{
				key = 4;
			}
			else if(col_data == 2)
			{
				key = 5;
			}
			else if(col_data == 4)
			{
				key = 6;
			}
			else if(col_data == 8)
			{
				key = 11;
			}		
		}
		else if(row_data == 4)
		{
			if(col_data == 1)
			{
				key = 7;
			}
			else if(col_data == 2)
			{
				key = 8;
			}
			else if(col_data == 4)
			{
				key = 9;
			}
			else if(col_data == 8)
			{
				key = 12;
			}
		}
		else if(row_data == 8)
		{
			if(col_data == 1)
			{
				key = 14;
			}
			else if(col_data == 2)
			{
				key = 0;
			}
			else if(col_data == 4)
			{
				key = 15;
			}
			else if(col_data == 8)
			{
				key = 13;
			}		
		}
		return key;
	}


	void MemWrite(uint8_t address, uint8_t data)
	{
		bool secondRam;
		write_enable.SetHigh();
   		if(address & (1 << (7))) 
		{
			secondRam = true;
		} 
    	else
		{
			secondRam = false;
		}
		bool firstRam;
		if(address & (1 << (6))) 
		{
			firstRam = true;
		} 
    	else
		{
			firstRam = false;
		}
		if((secondRam == true) & (firstRam == true))
		{
	
		}
		else if((secondRam == false) & (firstRam == false))
		{
			
		}
		else
		{
			m_io.SetAsOutput();
			m_io.SetHigh();
			read.SetAsOutput();
			read.SetHigh();
			write.SetAsOutput();
			write.SetHigh();
			put_data_address(address, data);
			sjsu::Delay(1ms);
			write.SetLow();
			sjsu::Delay(1ms);
			write.SetHigh();
			sjsu::Delay(1ms);
		}
	}
	
	uint8_t MemRead(uint8_t address)
	{
		uint8_t read_data;
		read_data = 0;
		bool secondRam;
   		if(address & (1 << (8 - 1))) 
		{
			secondRam = true;
		} 
    	else
		{
			secondRam = false;
		}
		bool firstRam;
		if(address & (1 << (7 - 1))) 
		{
			firstRam = true;
		} 
    	else
		{
			firstRam = false;
		}
		if((secondRam == true) & (firstRam == true))
		{
			
		}
		else if((secondRam == false) & (firstRam == false))
		{
			
		}
		else
		{
			m_io.SetAsOutput();
			m_io.SetHigh();
			read.SetAsOutput();
			read.SetHigh();
			write.SetAsOutput();
			write.SetHigh();
			put_address(address);
			sjsu::Delay(1ms);	
			write_enable.SetLow();
			sjsu::Delay(1ms);	
			ad[0].SetAsInput();
			ad[1].SetAsInput();
			ad[2].SetAsInput();
			ad[3].SetAsInput();
			ad[4].SetAsInput();	
			ad[5].SetAsInput();
			ad[6].SetAsInput();
			ad[7].SetAsInput();
			read.SetLow();
			sjsu::Delay(1ms);
			for(int i = 0; i < 8; i++)
			{
				read_data = read_data | (ad[i].Read() << i);
			}
			read.SetHigh();
			return read_data;
			sjsu::Delay(1ms);
		}
	}
private:
	sjsu::lpc40xx::Gpio ad[8] = {
		sjsu::lpc40xx::Gpio(2,2),
		sjsu::lpc40xx::Gpio(2,5),
		sjsu::lpc40xx::Gpio(2,7),
		sjsu::lpc40xx::Gpio(2,9),
		sjsu::lpc40xx::Gpio(0,15),
		sjsu::lpc40xx::Gpio(0,18),
		sjsu::lpc40xx::Gpio(0,1),
		sjsu::lpc40xx::Gpio(0,10),
		};
	sjsu::lpc40xx::Gpio write_enable = sjsu::lpc40xx::Gpio(0,17);
	sjsu::lpc40xx::Gpio ale = sjsu::lpc40xx::Gpio(0,22);
	sjsu::lpc40xx::Gpio m_io = sjsu::lpc40xx::Gpio(0,0);
	sjsu::lpc40xx::Gpio interrupt = sjsu::lpc40xx::Gpio(0,11);
	sjsu::lpc40xx::Gpio write = sjsu::lpc40xx::Gpio(0,16);
	sjsu::lpc40xx::Gpio read = sjsu::lpc40xx::Gpio(2,8);
};