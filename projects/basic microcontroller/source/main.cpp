#include <cstdint>
#include "Bus.hpp"

Bus bus;

void irs()
{
  LOG_INFO("Interrupt Detected\n");
  uint8_t data5;
  uint8_t address10;
  uint8_t address11;
  address10 = 0;
  address11 = 1;
  bus.polling(address10, address11);
  data5 = 15;
  bus.io_write(address10, data5);
}

int main()
{
  sjsu::lpc40xx::Gpio interrupt = sjsu::lpc40xx::Gpio(0,11);
  uint8_t address1;
  uint8_t address2;
  uint8_t address3;
  uint8_t data1;
  uint8_t data2;

  int n = 0;
  address1 = 3;
  address2 = 0;
  address3 = 1;
  data1 = 134;
  bus.Initialize();
  bus.io_write(address1, data1);
  data1 = 5; // Used to set INTE B which was 0b 0000 0101  

  bus.io_write(address1, data1);
  data2 = 15;
  bus.io_write(address2, data2);
  interrupt.GetPin().SetPull(sjsu::Pin::Resistor::kPullUp);
  interrupt.AttachInterrupt(irs, sjsu::Gpio::Edge::kEdgeRising);  
  
  while(n == 0)
  {
  }
  
  
  return 0;
  

}
