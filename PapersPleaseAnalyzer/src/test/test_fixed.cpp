#include "pch.h"
#include "test_fixed.h"

#include "paplease/core/fixed.h"

namespace test {

	using namespace paplease;

	static int float_as_hex(float value)
	{
		return *(int*)(&value);
	}

	struct RGB
	{
		int r, g, b;
	};

	void test_fixed_hash_table()
	{
		using FloatHex = int;
		constexpr size_t Count = 20;

		core::FixedHashTable<FloatHex, RGB, Count> my_table{};

		my_table.Set(float_as_hex(-1.0f), RGB{ 0, 0, 0 });
		my_table.Set(float_as_hex(0.0f), RGB{ 127, 127, 127 });
		my_table.Set(float_as_hex(1.0f), RGB{ 255, 255, 255 });

		for (const auto& entry : my_table)
		{

		}
	}

}  // namespace test