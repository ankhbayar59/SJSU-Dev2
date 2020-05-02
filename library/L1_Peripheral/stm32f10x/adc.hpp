#pragma once

#include <cstdint>

#include "L1_Peripheral/adc.hpp"

#include "L0_Platform/stm32f10x/stm32f10x.h"
#include "L1_Peripheral/stm32f10x/pin.hpp"
#include "L1_Peripheral/system_controller.hpp"
#include "utility/log.hpp"
#include "utility/status.hpp"
#include "utility/units.hpp"

namespace sjsu
{
namespace stm32f10x
{
/// ADC driver for implementation for the stm32f10x microcontrollers.
/// Activating a single channel will activate and enable sampling for all
/// channel.
///
/// DMA channels used are:
///    - DMA 1 Channel 1
///    - DMA 2 Channel 5
///
/// Only supports ADC1 and ADC2 and thus does not support usage of ADC3.
class Adc final : public sjsu::Adc
{
 public:
  /// Namespace containing the bitmask to manipulate the control registers ()
  struct Control  // NOLINT
  {
    /// In burst mode, sets the ADC channels to be automatically converted.
    /// It bit position represents 1 channel with this 8 channel ADC.
    /// In software mode, this should hold only a single 1 for the single
    /// channel to be converted.
    static constexpr bit::Mask kChannelSelect = bit::CreateMaskFromRange(0, 7);
    /// Sets the channel's clock divider. Potentially saving power if clock is
    /// reduced further.
    static constexpr bit::Mask kClockDivider = bit::CreateMaskFromRange(8, 15);
    /// Enable Burst Mode for the ADC. See BurstMode() method of this class to
    /// learn more about what it is and how it works.
    static constexpr bit::Mask kBurstEnable = bit::CreateMaskFromRange(16);
    /// Power on the ADC
    static constexpr bit::Mask kPowerEnable = bit::CreateMaskFromRange(21);
    /// In order to start a conversion a start code must be inserted into this
    /// bit location.
    static constexpr bit::Mask kStartCode = bit::CreateMaskFromRange(24, 26);
    /// Not used in this driver, but allows the use of an external pins to
    /// trigger a conversion. This flag indicates if rising or falling edges
    /// trigger the conversion.
    /// 1 = falling, 0 = rising.
    static constexpr bit::Mask kStartEdge = bit::CreateMaskFromRange(27);
  };
  /// Namespace containing the bitmask objects that are used to manipulate the
  /// stm32f10x ADC Global Data register.
  struct DataRegister  // NOLINT
  {
    /// Result mask holds the latest result from the last ADC that was converted
    static constexpr bit::Mask kResult = bit::CreateMaskFromRange(4, 15);
    /// Converted channel mask indicates which channel was converted in the
    /// latest conversion.
    static constexpr bit::Mask kConvertedChannel =
        bit::CreateMaskFromRange(24, 26);
    /// Holds whether or not the ADC overran its conversion.
    static constexpr bit::Mask kOverrun = bit::CreateMaskFromRange(30);
    /// Indicates when the ADC conversion is complete.
    static constexpr bit::Mask kDone = bit::CreateMaskFromRange(31);
  };
  /// Structure that defines a channel's pin, pin's function code and channel
  /// number.
  ///
  /// Usage:
  ///
  /// ```
  /// sjsu::stm32f10x::Pin pin(/* adc port number */, /* adc pin number */);
  /// const sjsu::stm32f10x::Adc::Channel_t kCustomChannelX = {
  ///   .pin      = pin,
  ///   .channel      = /* insert correct channel here */,
  ///   .pin_function = 0b101,
  /// };
  /// sjsu::stm32f10x::Adc channelX_adc(kCustomChannelX);
  /// ```
  struct Channel_t
  {
    /// Reference to the pin associated with the adc channel.
    const sjsu::Pin & pin;

    /// Channel number
    uint8_t channel;
  };

  /// Namespace containing predefined Channel_t description objects. These
  /// objects can be passed directly to the constructor of an stm32f10x::Adc
  /// object.
  ///
  /// Usage:
  ///
  /// ```
  /// sjsu::stm32f10x::Adc adc(sjsu::stm32f10x::Adc::Channel::kChannel0);
  /// ```
  struct Channel  // NOLINT
  {
   private:
    inline static const Pin kChannel0Pin = Pin('A', 0);
    inline static const Pin kChannel1Pin = Pin('A', 1);
    inline static const Pin kChannel2Pin = Pin('A', 2);
    inline static const Pin kChannel3Pin = Pin('A', 3);
    inline static const Pin kChannel4Pin = Pin('A', 4);
    inline static const Pin kChannel5Pin = Pin('A', 5);
    inline static const Pin kChannel6Pin = Pin('A', 6);
    inline static const Pin kChannel7Pin = Pin('A', 7);
    inline static const Pin kChannel8Pin = Pin('B', 0);
    inline static const Pin kChannel9Pin = Pin('B', 1);
    inline static const Pin kChannel10Pin = Pin('C', 0);
    inline static const Pin kChannel11Pin = Pin('C', 1);
    inline static const Pin kChannel12Pin = Pin('C', 2);
    inline static const Pin kChannel13Pin = Pin('C', 3);
    inline static const Pin kChannel14Pin = Pin('C', 4);
    inline static const Pin kChannel15Pin = Pin('C', 5);

   public:
    /// Channel 0 definition.
    inline static const Channel_t kChannel0 = {
      .pin     = kChannel0Pin,
      .channel = 0,
    };

    /// Channel 1 definition.
    inline static const Channel_t kChannel1 = {
      .pin     = kChannel1Pin,
      .channel = 1,
    };

    /// Channel 2 definition.
    inline static const Channel_t kChannel2 = {
      .pin     = kChannel2Pin,
      .channel = 2,
    };

    /// Channel 3 definition.
    inline static const Channel_t kChannel3 = {
      .pin     = kChannel3Pin,
      .channel = 3,
    };

    /// Channel 4 definition.
    inline static const Channel_t kChannel4 = {
      .pin     = kChannel4Pin,
      .channel = 4,
    };

    /// Channel 5 definition.
    inline static const Channel_t kChannel5 = {
      .pin     = kChannel5Pin,
      .channel = 5,
    };

    /// Channel 6 definition.
    inline static const Channel_t kChannel6 = {
      .pin     = kChannel6Pin,
      .channel = 6,
    };

    /// Channel 7 definition.
    inline static const Channel_t kChannel7 = {
      .pin     = kChannel7Pin,
      .channel = 7,
    };

    /// Channel 8 definition.
    inline static const Channel_t kChannel8 = {
      .pin     = kChannel8Pin,
      .channel = 0,
    };

    /// Channel 9 definition.
    inline static const Channel_t kChannel9 = {
      .pin     = kChannel9Pin,
      .channel = 1,
    };

    /// Channel 10 definition.
    inline static const Channel_t kChannel10 = {
      .pin     = kChannel10Pin,
      .channel = 2,
    };

    /// Channel 11 definition.
    inline static const Channel_t kChannel11 = {
      .pin     = kChannel11Pin,
      .channel = 3,
    };

    /// Channel 12 definition.
    inline static const Channel_t kChannel12 = {
      .pin     = kChannel12Pin,
      .channel = 4,
    };

    /// Channel 13 definition.
    inline static const Channel_t kChannel13 = {
      .pin     = kChannel13Pin,
      .channel = 5,
    };

    /// Channel 14 definition.
    inline static const Channel_t kChannel14 = {
      .pin     = kChannel14Pin,
      .channel = 6,
    };

    /// Channel 15 definition.
    inline static const Channel_t kChannel15 = {
      .pin     = kChannel15Pin,
      .channel = 7,
    };
  };

  /// The default and highest frequency that the ADC can operate at.
  static constexpr units::frequency::hertz_t kClockFrequency = 1_MHz;

  /// A pointer holding the address to the stm32f10x ADC peripheral.
  /// This variable is a dependency injection point for unit testing thus it is
  /// public and mutable. This is needed to perform the "test by side effect"
  /// technique for this class.
  inline static ADC_TypeDef * adc1 = ADC1;
  inline static ADC_TypeDef * adc2 = ADC2;

  /// Number of active bits of the ADC. The ADC is a 12-bit ADC meaning that the
  /// largest value it can have is 2^12 = 4096
  static constexpr uint8_t kActiveBits = 12;

  /// @param channel: Passed channel descriptor object. See Channel_t and
  ///        Channel documentation for more details about how to use this.
  explicit constexpr Adc(const Channel_t & channel) : channel_(channel) {}
  Status Initialize() const override
  {
    auto & system      = sjsu::SystemController::GetPlatformController();
    const auto kAdcId1 = SystemController::Peripherals::kAdc1;
    const auto kAdcId2 = SystemController::Peripherals::kAdc2;

    system.PowerUpPeripheral(kAdcId1);
    system.PowerUpPeripheral(kAdcId2);

    // It is required for proper operation of analog pins for the stm32f10x that
    // the pins be floating.
    channel_.pin.SetAsAnalogMode(true);

    adc_base->CR = control;

    return Status::kSuccess;
  }

  uint32_t Read() const override
  {
    // Convert analog value from analog to a digital value
    Conversion();
    uint32_t result =
        bit::Extract(adc_base->DR[channel_.channel], DataRegister::kResult);
    return result;
  }

  uint8_t GetActiveBits() const override
  {
    return kActiveBits;
  }

 private:
  bool HasConversionFinished() const
  {
    return bit::Read(adc_base->DR[channel_.channel], DataRegister::kDone);
  }

  void Conversion() const
  {
    if (BurstModeIsEnabled())
    {
      // NOTE: If burst mode is enabled, conversion start must be set 0
      adc_base->CR = bit::Insert(adc_base->CR, 0, Control::kStartCode);
    }
    else
    {
      uint32_t channel_select = (1 << channel_.channel);
      adc_base->CR =
          bit::Insert(adc_base->CR, channel_select, Control::kChannelSelect);
      // Start ADC conversion with start code 0x01
      adc_base->CR = bit::Insert(adc_base->CR, 0x01, Control::kStartCode);
      while (!HasConversionFinished())
      {
        continue;
      }
    }
  }

  const Channel_t & channel_;
};
}  // namespace stm32f10x
}  // namespace sjsu
