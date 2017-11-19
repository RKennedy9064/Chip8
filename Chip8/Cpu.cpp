#include "Cpu.h"
#include <memory>
#include <random>

Cpu::Cpu()
{
		LoadFont();
		ClearScreen();
}

Cpu::~Cpu()
{
}

void Cpu::AddByte(DataRegisters data_register, u8 byte)
{
		u8 result = GetDataRegister(data_register) + byte;

		SetDataRegister(data_register, result);
}

void Cpu::AddIndex(DataRegisters data_register)
{
		u16 address = GetIndex() + GetDataRegister(data_register);

		SetIndex(address);
}

void Cpu::AddRegisters(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 data_register_x_value = GetDataRegister(data_register_x);
		u8 data_register_y_value = GetDataRegister(data_register_y);
		u16 result = data_register_x_value + data_register_y_value;
		u8 carry_flag = result > UCHAR_MAX ? 1 : 0;

		SetDataRegister(data_register_x, static_cast<u8>(result));
		SetDataRegister(DataRegisters::vF, carry_flag);
}

void Cpu::AndRegisters(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 result = GetDataRegister(data_register_x) & GetDataRegister(data_register_y);
		
		SetDataRegister(data_register_x, result);
}

void Cpu::Call(u16 address)
{
		m_stack.push(m_pc);
		m_pc = ConvertAddress(address);
}

void Cpu::ClearScreen()
{
		memset(m_screen, NULL, sizeof m_screen);
}

u16 Cpu::ConvertAddress(u16 address)
{
		return address & 0xFFF;
}

u8 Cpu::GetDataRegister(DataRegisters data_register)
{
		return m_data_registers[static_cast<u8>(data_register)];
}

const u8 *Cpu::GetDataRegisters()
{
		return m_data_registers;
}

u8 Cpu::GetDelayTimer()
{
		return m_delay_timer;
}

u16 Cpu::GetIndex()
{
		return m_i;
}

const u8 *Cpu::GetRam()
{
		return m_ram;
}

const u8 *Cpu::GetScreen()
{
		return m_screen;
}

u16 Cpu::GetProgramCounter()
{
		return m_pc;
}

u8 Cpu::GetSoundTimer()
{
		return m_sound_timer;
}

u16 Cpu::GetStack()
{
		return m_stack.top();
}

void Cpu::Jump(u16 address)
{
		m_pc = ConvertAddress(address);
}

void Cpu::JumpPlus(u16 address)
{
		m_pc = address + GetDataRegister(DataRegisters::v0);
}

void Cpu::LoadFont()
{
		static u8 font[] =
		{
				0xF0, 0x90, 0x90, 0x90, 0xF0,
				0x20, 0x60, 0x20, 0x20, 0x70,
				0xF0, 0x10, 0xF0, 0x80, 0xF0,
				0xF0, 0x10, 0xF0, 0x10, 0xF0,
				0x90, 0x90, 0xF0, 0x10, 0x10,
				0xF0, 0x80, 0xF0, 0x10, 0xF0,
				0xF0, 0x80, 0xF0, 0x90, 0xF0,
				0xF0, 0x10, 0x20, 0x40, 0x40,
				0xF0, 0x90, 0xF0, 0x90, 0xF0,
				0xF0, 0x90, 0xF0, 0x10, 0xF0,
				0xF0, 0x90, 0xF0, 0x90, 0x90,
				0xE0, 0x90, 0xE0, 0x90, 0xE0,
				0xF0, 0x80, 0x80, 0x80, 0xF0,
				0xE0, 0x90, 0x90, 0x90, 0xE0,
				0xF0, 0x80, 0xF0, 0x80, 0xF0,
				0xF0, 0x80, 0xF0, 0x80, 0x80,
		};

		memcpy(m_ram, font, sizeof font);
}

void Cpu::OrRegisters(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 result = GetDataRegister(data_register_x) | GetDataRegister(data_register_y);

		SetDataRegister(data_register_x, result);
}

void Cpu::Return()
{
		m_pc = m_stack.top();
		m_stack.pop();
}

void Cpu::SetDataRegister(DataRegisters data_register, u8 byte)
{
		m_data_registers[static_cast<u8>(data_register)] = byte;
}

void Cpu::SetDataRegisters(DataRegisters data_register)
{
		for (u8 current_data_register = 0; current_data_register <= static_cast<u8>(data_register); ++current_data_register)
		{
				m_data_registers[current_data_register] = m_ram[m_i++];
		}
}

void Cpu::SetDelayTimer(DataRegisters data_register)
{
		m_delay_timer = GetDataRegister(data_register);
}

void Cpu::SetIndex(u16 address)
{
		m_i = address;
}

void Cpu::SetSoundTimer(DataRegisters data_register)
{
		m_sound_timer = GetDataRegister(data_register);
}

void Cpu::SetTextCharacter(DataRegisters data_register)
{
		u8 character = GetDataRegister(data_register);
		u16 character_address = character * font_length;

		SetIndex(ConvertAddress(character_address));
}

void Cpu::ShiftRegisterLeft(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 data_register_y_value = GetDataRegister(data_register_y);
		u8 result = data_register_y_value << 1;
		u8 most_significant_bit = (data_register_y_value & 0x80) >> 7;

		SetDataRegister(data_register_x, result);
		SetDataRegister(DataRegisters::vF, most_significant_bit);
}

void Cpu::ShiftRegisterRight(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 data_register_y_value = GetDataRegister(data_register_y);
		u8 result = data_register_y_value >> 1;
		u8 least_significant_bit = data_register_y_value & 0x01;

		SetDataRegister(data_register_x, result);
		SetDataRegister(DataRegisters::vF, least_significant_bit);
}

void Cpu::SkipEqualByte(DataRegisters data_register, u8 byte)
{
		if (GetDataRegister(data_register) == byte)
				m_pc += 2;
}

void Cpu::SkipEqualRegister(DataRegisters data_register_x, DataRegisters data_register_y)
{
		if (GetDataRegister(data_register_x) == GetDataRegister(data_register_y))
				m_pc += 2;
}

void Cpu::SkipNotEqualByte(DataRegisters data_register, u8 byte)
{
		if (GetDataRegister(data_register) != byte)
				m_pc += 2;
}

void Cpu::SkipNotEqualRegister(DataRegisters data_register_x, DataRegisters data_register_y)
{
		if (GetDataRegister(data_register_x) != GetDataRegister(data_register_y))
				m_pc += 2;
}

void Cpu::StoreAddress(u16 address)
{
		SetIndex(ConvertAddress(address));
}

void Cpu::StoreBinaryCodedDecimal(DataRegisters data_register)
{
		static const u8 digits = 3;
		u8 value = GetDataRegister(data_register);
		u8 digit = 0;
		u8 offset = 2;
		u16 index = GetIndex();

		for (digit; digit < digits; ++digit)
		{
				m_ram[index + offset] = value % 10;
				value /= 10;
				--offset;
		}
}

void Cpu::StoreDelayTimer(DataRegisters data_register)
{
		SetDataRegister(data_register, GetDelayTimer());
}

void Cpu::StoreRandomNumber(DataRegisters data_register, u8 mask)
{
		static std::random_device random_device;
		static std::mt19937 mt19937(random_device());
		static std::uniform_int_distribution<u32> uniform_int_distribution(0, UCHAR_MAX);
		
		u8 random_number = uniform_int_distribution(mt19937) & mask;
		SetDataRegister(data_register, random_number);
}

void Cpu::StoreDataRegister(DataRegisters data_register_x, DataRegisters data_register_y)
{
		SetDataRegister(data_register_x, GetDataRegister(data_register_y));
}

void Cpu::StoreDataRegisters(DataRegisters data_register)
{
		for (u8 current_data_register = 0; current_data_register <= static_cast<u8>(data_register); ++current_data_register)
		{
				m_ram[m_i++] = m_data_registers[current_data_register];
		}
}

void Cpu::SubtractRegister(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 data_register_x_value = GetDataRegister(data_register_x);
		u8 data_register_y_value = GetDataRegister(data_register_y);
		u8 result = data_register_x_value - data_register_y_value;
		u8 borrow_flag = data_register_y_value > data_register_x_value ? 0 : 1;

		SetDataRegister(data_register_x, result);
		SetDataRegister(DataRegisters::vF, borrow_flag);
}

void Cpu::SubtractRegisters(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 data_register_x_value = GetDataRegister(data_register_x);
		u8 data_register_y_value = GetDataRegister(data_register_y);
		u8 result = data_register_y_value - data_register_x_value;
		u8 borrow_flag = data_register_x_value > data_register_y_value ? 0 : 1;

		SetDataRegister(data_register_x, result);
		SetDataRegister(DataRegisters::vF, borrow_flag);
}

void Cpu::XorRegisters(DataRegisters data_register_x, DataRegisters data_register_y)
{
		u8 result = GetDataRegister(data_register_x) ^ GetDataRegister(data_register_y);

		SetDataRegister(data_register_x, result);
}