#include <bitset>
#include <iomanip>
#include <iostream>

struct desc_struct
{
	union
	{
		struct
		{
			uint64_t desc;
		};

		struct
		{
			uint16_t limit0;
			uint16_t base0;
			unsigned base1 : 8, type : 4, s : 1, dpl : 2, p : 1;
			unsigned limit1 : 4, avl : 1, l : 1, d : 1, g : 1, base2 : 8;
		};
	};
};

uint32_t
get_user_input(const uint32_t max_size, const bool hex)
{
	uint32_t value;

	if (hex)
		std::cin >> std::hex >> value;
	else
		std::cin >> value;

	if (!std::cin.fail() && value <= max_size)
		return value;

	throw std::exception("Invalid input!");
};

int
main()
{
	try
	{
		desc_struct desc{};

		std::cout << "Please, type base addr(hex)\n";
		auto full_base_addr = get_user_input(UINT32_MAX, true);
		desc.base0 = (full_base_addr & 0x0000ffff);
		desc.base1 = (full_base_addr & 0x00ff0000) >> 16;
		desc.base2 = (full_base_addr & 0xff000000) >> 24;

		std::cout << "Please, type Segment Limit(hex)\n";
		auto limit = get_user_input(UINT16_MAX * 0x10 + 0xF, true);
		desc.limit0 = limit & 0x0ffff;
		desc.limit1 = (limit & 0xf0000) >> 16;

		std::cout << "Please, type Granularity bit(0-1)\n";
		desc.g = get_user_input(1, false);

		std::cout << "Please, type Bit rate flag(0-1)\n";
		desc.d = get_user_input(1, false);;

		std::cout << "Please, type available bit flag(0-1)\n";
		desc.p = get_user_input(1, false);

		std::cout << "Please, type DPL flag(0-3)\n";
		desc.dpl = get_user_input(3, false);

		std::cout << "Please, type of segment (system or no) (0-1)\n";
		desc.s = get_user_input(1, false);

		std::cout << "Please, type of segment (other settings) (hex, 0 - F)\n";
		desc.type = get_user_input(0xF, false);

		std::bitset<sizeof(uint64_t) * 8> desc_bits(desc.desc);
		std::cout << "\nLITTLE-ENDIAN:";
		std::cout << "\nHEX DUMP: " << std::setfill('0') << std::setw(16) << std::hex
			<< std::uppercase << desc.desc;
		std::cout << "\nBINARY DUMP: " << desc_bits;

		std::cout << "\n\nBIG-ENDIAN:";
		desc_bits = _byteswap_uint64(desc.desc); // _byteswap_uint64 only msvc
		std::cout << "\nHEX DUMP: " << std::setfill('0') << std::setw(16) << std::hex
			<< std::uppercase << _byteswap_uint64(desc.desc); // _byteswap_uint64 only msvc
		std::cout << "\nBINARY DUMP: " << desc_bits;

		std::cout << "\n\n";
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}
