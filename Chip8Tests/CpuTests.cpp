#include <gtest\gtest.h>
#include "../Chip8/Cpu.h"
#include <vector>

using DataRegisters = Cpu::DataRegisters;

// Opcode 00E0
TEST(Cpu, ClearScreen)
{
		Cpu cpu;
		u8 expected_value[Cpu::screen_size] = { 0 };
		
		cpu.ClearScreen();

		EXPECT_EQ(0, memcmp(cpu.GetScreen(), expected_value, Cpu::screen_size));
}

// Opcode 00EE
TEST(Cpu, Return)
{
		Cpu cpu;
		u16 address = 0x123;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.Call(address);
		EXPECT_EQ(address, cpu.GetProgramCounter());
		EXPECT_EQ(program_counter, cpu.GetStack());

		cpu.Return();
		EXPECT_EQ(program_counter, cpu.GetProgramCounter());
}

// Opcode 1NNN
TEST(Cpu, Jump)
{
		Cpu cpu;
		u16 address = 0x1234;
		u16 expected_value = address & 0xFFF;

		cpu.Jump(address);
		EXPECT_EQ(expected_value, cpu.GetProgramCounter());
}

// Opcode 2NNN
TEST(Cpu, Call)
{
		Cpu cpu;
		u16 address = 0x1234;
		u16 program_counter = cpu.GetProgramCounter();
		u16 expected_value = address & 0xFFF;

		cpu.Call(address);
		EXPECT_EQ(expected_value, cpu.GetProgramCounter());
		EXPECT_EQ(program_counter, cpu.GetStack());
}

// Opcode 3XNN - Values Match
TEST(Cpu, SkipEqualByte_ValuesMatch)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 byte = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register, byte);
		cpu.SkipEqualByte(data_register, byte);
		EXPECT_EQ(program_counter + 2, cpu.GetProgramCounter());
}

// Opcode 3XNN - Values Don't Match
TEST(Cpu, SkipEqualByte_ValuesDontMatch)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 byte = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register, NULL);
		cpu.SkipEqualByte(data_register, byte);
		EXPECT_EQ(program_counter, cpu.GetProgramCounter());
}

// Opcode 4XNN - Values Match
TEST(Cpu, SkipNotEqualByte_ValuesMatch)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 byte = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register, byte);
		cpu.SkipNotEqualByte(data_register, byte);
		EXPECT_EQ(program_counter, cpu.GetProgramCounter());
}

// Opcode 4XNN - Values Don't Match
TEST(Cpu, SkipNotEqualByte_ValuesDontMatch)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 byte = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register, NULL);
		cpu.SkipNotEqualByte(data_register, byte);
		EXPECT_EQ(program_counter + 2, cpu.GetProgramCounter());
}

// Opcode 5XY0 - Values Match
TEST(Cpu, SkipEqualRegister_ValuesMatch)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x12;
		u8 data_register_y_value = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SkipEqualRegister(data_register_x, data_register_y);
		EXPECT_EQ(program_counter + 2, cpu.GetProgramCounter());
}

// Opcode 5XY0 - Values Don't Match
TEST(Cpu, SkipEqualRegister_ValuesDontMatch)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x12;
		u8 data_register_y_value = 0x34;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SkipEqualRegister(data_register_x, data_register_y);
		EXPECT_EQ(program_counter, cpu.GetProgramCounter());
}

// Opcode 6XNN
TEST(Cpu, StoreByte)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 byte = 0x12;

		cpu.SetDataRegister(data_register, byte);
		EXPECT_EQ(byte, cpu.GetDataRegister(data_register));
}

// Opcode 7XNN
TEST(Cpu, AddByte)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0x12;
		u8 byte = 0x34;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.AddByte(data_register, byte);
		EXPECT_EQ(data_register_value + byte, cpu.GetDataRegister(data_register));
}

// Opcode 8XY0
TEST(Cpu, StoreDataRegister)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x12;
		u8 data_register_y_value = 0x34;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.StoreDataRegister(data_register_x, data_register_y);
		EXPECT_EQ(data_register_y_value, cpu.GetDataRegister(data_register_x));
}

// Opcode 8XY1
TEST(Cpu, OrRegisters)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x0F;
		u8 data_register_y_value = 0xF0;
		u8 expected_value = 0xFF;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.OrRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
}

// Opcode 8XY2
TEST(Cpu, AndRegisters)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0xF0;
		u8 data_register_y_value = 0x3C;
		u8 expected_value = 0x30;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.AndRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
}

// Opcode 8XY3
TEST(Cpu, XorRegisters)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0xFF;
		u8 data_register_y_value = 0x0F;
		u8 expected_value = 0xF0;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.XorRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
}

// Opcode 8XY4 - Result carries
TEST(Cpu, AddRegister_ResultCarries)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0xFF;
		u8 data_register_y_value = 0x01;
		u8 expected_value = data_register_x_value + data_register_y_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.AddRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(1, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY4 - Result doesn't carry
TEST(Cpu, AddRegisters_ResultDoesntCarry)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x0F;
		u8 data_register_y_value = 0x0F;
		u8 expected_value = data_register_x_value + data_register_y_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.AddRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(0, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY5 - Result borrows
TEST(Cpu, SubtractRegister_ResultBorrows)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x00;
		u8 data_register_y_value = 0x01;
		u8 expected_value = data_register_x_value - data_register_y_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SubtractRegister(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(0, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY5 - Result doesn't borrow
TEST(Cpu, SubtractRegister_ResultDoesntBorrow)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x01;
		u8 data_register_y_value = 0x01;
		u8 expected_value = data_register_x_value - data_register_y_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SubtractRegister(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(1, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY6 - Least significant bit is zero
TEST(Cpu, ShiftRegisterRight_LSB_Zero)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x00;
		u8 data_register_y_value = 0x02;
		u8 expected_value = data_register_y_value >> 1;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.ShiftRegisterRight(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(0, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY6 - Least significant bit is one
TEST(Cpu, ShiftRegisterRight_LSB_One)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x00;
		u8 data_register_y_value = 0x03;
		u8 expected_value = data_register_y_value >> 1;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.ShiftRegisterRight(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(1, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY7 - Result borrows
TEST(Cpu, SubtractRegisters_ResultBorrows)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x02;
		u8 data_register_y_value = 0x01;
		u8 expected_value = data_register_y_value - data_register_x_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SubtractRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(0, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XY7 - Result doesn't borrow
TEST(Cpu, SubtractRegisters_ResultDoesntBorrow)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x01;
		u8 data_register_y_value = 0x02;
		u8 expected_value = data_register_y_value - data_register_x_value;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.SubtractRegisters(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(1, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XYE - Most significant bit is one
TEST(Cpu, ShiftRegisterLeft_MSB_1)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x00;
		u8 data_register_y_value = 0xFF;
		u8 expected_value = data_register_y_value << 1;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.ShiftRegisterLeft(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(1, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 8XYE - Most significant bit is zero
TEST(Cpu, ShiftRegisterLeft_MSB_0)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_x_value = 0x00;
		u8 data_register_y_value = 0x78;
		u8 expected_value = data_register_y_value << 1;

		cpu.SetDataRegister(data_register_x, data_register_x_value);
		cpu.SetDataRegister(data_register_y, data_register_y_value);
		cpu.ShiftRegisterLeft(data_register_x, data_register_y);
		EXPECT_EQ(expected_value, cpu.GetDataRegister(data_register_x));
		EXPECT_EQ(0, cpu.GetDataRegister(DataRegisters::vF));
}

// Opcode 9XY0 - Values match
TEST(Cpu, SkipNotEqualRegister_ValuesMatch)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_value = 0x12;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register_x, data_register_value);
		cpu.SetDataRegister(data_register_y, data_register_value);
		cpu.SkipNotEqualRegister(data_register_x, data_register_y);
		EXPECT_EQ(program_counter, cpu.GetProgramCounter());
}

// Opcode 9XY0 - Values don't match
TEST(Cpu, SkipNotEqualRegister_ValuesDontMatch)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 data_register_value_x = 0x12;
		u8 data_register_value_y = 0x34;
		u16 program_counter = cpu.GetProgramCounter();

		cpu.SetDataRegister(data_register_x, data_register_value_x);
		cpu.SetDataRegister(data_register_y, data_register_value_y);
		cpu.SkipNotEqualRegister(data_register_x, data_register_y);
		EXPECT_EQ(program_counter + 2, cpu.GetProgramCounter());
}

// Opcode ANNN
TEST(Cpu, StoreAddress)
{
		Cpu cpu;
		u16 address = 0x1234;
		u16 index = cpu.GetIndex();
		u16 expected_value = address & 0xFFF;

		cpu.StoreAddress(address);
		EXPECT_EQ(expected_value, cpu.GetIndex());
}

// Opcode BNNN
TEST(Cpu, JumpPlus)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0x0F;
		u16 address = 0x123;
		u16 expected_value = address + data_register_value;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.JumpPlus(address);
		EXPECT_EQ(expected_value, cpu.GetProgramCounter());
}

// Opcode FX07
TEST(Cpu, StoreDelayTimer)
{
		Cpu cpu;
		DataRegisters data_register_x = DataRegisters::v0;
		DataRegisters data_register_y = DataRegisters::v1;
		u8 delay_timer = 0x12;

		cpu.SetDataRegister(data_register_x, delay_timer);
		cpu.SetDelayTimer(data_register_x);
		cpu.StoreDelayTimer(data_register_y);
		EXPECT_EQ(delay_timer, cpu.GetDataRegister(data_register_y));
}

// Opcode FX15
TEST(Cpu, SetDelayTimer)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0x12;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.SetDelayTimer(data_register);
		EXPECT_EQ(data_register_value, cpu.GetDelayTimer());
}

// Opcode FX18
TEST(Cpu, SetSoundTimer)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0x12;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.SetSoundTimer(data_register);
		EXPECT_EQ(data_register_value, cpu.GetSoundTimer());
}

// Opcode FX1E
TEST(Cpu, AddIndex)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0xFF;
		u16 address = 0xFFFF;
		u16 expected_value = (address & 0xFFF) + data_register_value;

		cpu.StoreAddress(address);
		cpu.SetDataRegister(data_register, data_register_value);
		cpu.AddIndex(data_register);
		EXPECT_EQ(expected_value, cpu.GetIndex());
}

// Opcode FX29
TEST(Cpu, SetTextCharacter)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		u8 data_register_value = 0x0F;
		u8 expected_value = data_register_value * Cpu::font_length;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.SetTextCharacter(data_register);
		EXPECT_EQ(expected_value, cpu.GetIndex());
}

// Opcode FX33
TEST(Cpu, StoreBinaryCodedDecimal)
{
		Cpu cpu;
		DataRegisters data_register = DataRegisters::v0;
		const u8 *ram = cpu.GetRam();
		u8 data_register_value = 0x12;
		u16 address = 0x200;

		cpu.SetDataRegister(data_register, data_register_value);
		cpu.StoreAddress(address);
		cpu.StoreBinaryCodedDecimal(data_register);
		EXPECT_EQ(ram[address], 0);
		EXPECT_EQ(ram[address + 1], 1);
		EXPECT_EQ(ram[address + 2], 8);
}

// Opcode FX55
TEST(Cpu, StoreDataRegisters)
{
		Cpu cpu;
		std::vector<u8> data_register_values;
		const u8 *ram = cpu.GetRam();
		u16 address = 0x200;

		for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
		{
				u8 data_register_value = data_register * 2;

				data_register_values.push_back(data_register_value);
				cpu.SetDataRegister(static_cast<DataRegisters>(data_register), data_register_value);
		}

		for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
		{
				std::vector<u8> ram_values;
				const u8 *data_register_location = ram + address;
				u16 expected_address = address + data_register + 1;

				cpu.StoreAddress(address);
				cpu.StoreDataRegisters(static_cast<DataRegisters>(data_register));
				ram_values.assign(data_register_location, data_register_location + data_register + 1);

				ASSERT_EQ(expected_address, cpu.GetIndex());
				ASSERT_EQ(true, std::equal(ram_values.begin(), ram_values.end(), data_register_values.begin()));
		}
}

// Opcode FX65
TEST(Cpu, SetRegisters)
{
		Cpu cpu;
		std::vector<u8> data_register_values;
		const u8 *ram = cpu.GetRam();
		u16 address = 0x200;

		for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
		{
				u8 data_register_value = data_register * 2;

				data_register_values.push_back(data_register_value);
		}

		for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
		{
				std::vector<u8> expected_data_register_values;
				const u8 *data_register_location = cpu.GetDataRegisters();
				u16 expected_address = address + data_register + 1;

				for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
				{
						u8 data_register_value = data_register * 2;

						cpu.SetDataRegister(static_cast<DataRegisters>(data_register), data_register_value);
				}

				cpu.StoreAddress(address);
				cpu.StoreDataRegisters(static_cast<DataRegisters>(data_register));

				for (u8 data_register = 0; data_register < Cpu::data_registers; ++data_register)
				{
						cpu.SetDataRegister(static_cast<DataRegisters>(data_register), NULL);
				}

				cpu.StoreAddress(address);
				cpu.SetDataRegisters(static_cast<DataRegisters>(data_register));
				expected_data_register_values.assign(data_register_location, data_register_location + data_register + 1);

				ASSERT_EQ(expected_address, cpu.GetIndex());
				ASSERT_EQ(true, std::equal(expected_data_register_values.begin(), expected_data_register_values.end(), data_register_values.begin()));
		}
}