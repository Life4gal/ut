#include <prometheus/ut/unit_test.hpp>

using namespace prometheus;

namespace
{
	ut::suite<"suite"> s1 = []noexcept -> void
	{
		using namespace ut;

		"test"_test = [] noexcept -> void
		{
			expect(1 + 1 == 2_i) << "never output" << fatal;
			expect(1 + 1 == 2_i) << fatal << "never output";

			expect(1 + 1 == 3_i) << "error message...";

			expect(1 + 1 == 2_i) << "never output" << fatal;
			expect(1 + 1 == 2_i) << fatal << "never output";

			"nested_test"_test = [] noexcept -> void
			{
				expect(2 * 2 == 4_i) << "never output" << fatal;
				expect(2 * 2 == 4_i) << fatal << "never output";

				expect(2 * 2 == 5_i) << "nested error message...";

				expect(2 * 2 == 4_i) << "never output" << fatal;
				expect(2 * 2 == 4_i) << fatal << "never output";
			};
		};
	};

	ut::suite<"suite"> s2 = [] noexcept -> void
	{
		using namespace ut;

		"test"_test = [] noexcept -> void
		{
			expect(1 + 1 == 2_i) << "never output" << fatal;
			expect(1 + 1 == 2_i) << fatal << "never output";

			expect(1 + 1 == 3_i) << "error message...";

			expect(1 + 1 == 2_i) << "never output" << fatal;
			expect(1 + 1 == 2_i) << fatal << "never output";

			"nested_test"_test = [] noexcept -> void
			{
				expect(2 * 2 == 4_i) << "never output" << fatal;
				expect(2 * 2 == 4_i) << fatal << "never output";

				expect(2 * 2 == 5_i) << "nested error message...";

				expect(2 * 2 == 4_i) << "never output" << fatal;
				expect(2 * 2 == 4_i) << fatal << "never output";
			};
		};
	};

	ut::suite<"suite"> s3 = [] noexcept -> void
	{
		using namespace ut;

		"test"_test = [] noexcept -> void
		{
			expect(1 + 1 == 2_i) << "never output" << fatal;
			expect(1 + 1 == 2_i) << fatal << "never output";

			"nested_test"_test = [] noexcept -> void
			{
				expect(2 * 2 == 4_i) << "never output" << fatal;
				expect(2 * 2 == 4_i) << fatal << "never output";
			};
		};
	};
}


auto main() noexcept -> int
{
	ut::get_config().report_level = ut::config_type::ReportLevel::ALL;
}
