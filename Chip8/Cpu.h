#pragma once

#include <cstdint>
#include <stack>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;

class Cpu
{
public:
		static const u8 data_registers	= 0x10;
		static const u8 stack_entries		= 0x10;
		static const u8 screen_width		= 0x40;
		static const u8 screen_height		= 0x20;
		static const u8 font_length			= 0x05;

		static const u16 ram_size				= 0xFFF;
		static const u16 screen_size		= screen_width * screen_height;

		enum class DataRegisters
				: u8
		{
				v0, v1, v2, v3, 
				v4, v5, v6, v7, 
				v8, v9, vA, vB, 
				vC, vD, vE, vF
		};

		Cpu();
		~Cpu();

		u8 GetDataRegister(DataRegisters data_register);
		void SetDataRegister(DataRegisters data_register, u8 byte);
		const u8 *GetRam();
		const u8 *GetDataRegisters();
		const u8 *GetScreen();

		u8 GetDelayTimer();
		u8 GetSoundTimer();

		u16 GetProgramCounter();
		u16 GetIndex();
		u16 GetStack();

		void AddByte(DataRegisters data_register, u8 byte);
		void AddIndex(DataRegisters data_register);
		void AddRegisters(DataRegisters data_register_x, DataRegisters data_register_y);
		void AndRegisters(DataRegisters data_register_x, DataRegisters data_register_y);
		void Call(u16 address);
		void ClearScreen();
		void Jump(u16 address);
		void JumpPlus(u16 address);
		void OrRegisters(DataRegisters data_register_x, DataRegisters data_register_y);
		void Return();
		void SetDataRegisters(DataRegisters data_register);
		void SetDelayTimer(DataRegisters data_register);
		void SetSoundTimer(DataRegisters data_register);
		void SetTextCharacter(DataRegisters data_register);
		void ShiftRegisterLeft(DataRegisters data_register_x, DataRegisters data_register_y);
		void ShiftRegisterRight(DataRegisters data_register_x, DataRegisters data_register_y);
		void SkipEqualByte(DataRegisters data_register, u8 byte);
		void SkipEqualRegister(DataRegisters data_register_x, DataRegisters data_register_y);
		void SkipNotEqualByte(DataRegisters data_register, u8 byte);
		void SkipNotEqualRegister(DataRegisters data_register_x, DataRegisters data_register_y);
		void StoreAddress(u16 address);
		void StoreBinaryCodedDecimal(DataRegisters data_register);
		void StoreDelayTimer(DataRegisters data_register);
		void StoreRandomNumber(DataRegisters data_register, u8 mask);
		void StoreDataRegister(DataRegisters data_register_x, DataRegisters data_register_y);
		void StoreDataRegisters(DataRegisters data_register);
		void SubtractRegister(DataRegisters data_register_x, DataRegisters data_register_y);
		void SubtractRegisters(DataRegisters data_register_x, DataRegisters data_register_y);
		void XorRegisters(DataRegisters data_register_x, DataRegisters data_register_y);

private:
		u8 m_ram[ram_size];
		u8 m_screen[screen_size];
		u8 m_data_registers[data_registers];
		u8 m_delay_timer;
		u8 m_sound_timer;

		u16 m_pc;
		u16 m_i;

		std::stack<u16> m_stack;

		u16 ConvertAddress(u16 address);
		void LoadFont();
		void SetIndex(u16 address);
};