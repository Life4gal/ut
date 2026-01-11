// This file is part of prometheus
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <source_location>

#include <prometheus/ut/def.hpp>

namespace prometheus::ut::events
{
	class Event {};

	template<typename E>
	constexpr auto is_event_v = std::is_base_of_v<Event, E>;
	template<typename E>
	concept event_t = is_event_v<E>;

#if defined(PROMETHEUS_COMPILER_GNU) or defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
	PROMETHEUS_COMPILER_DISABLE_WARNING_PUSH

	// struct bar {};
	// struct foo : bar
	// {
	//	int a;
	// };
	// foo f{.a = 42}; // <-- warning: missing initializer for member `foo::<anonymous>` [-Wmissing-field-initializers]
	// clang-format off
	PROMETHEUS_COMPILER_DISABLE_WARNING(-Wmissing-field-initializers)
	// clang-format on
#endif

	// =========================================
	// SUITE
	// =========================================

	class PROMETHEUS_COMPILER_EMPTY_BASE EventSuiteBegin final : public Event
	{
	public:
		suite_name_view_type name;
	};

	class PROMETHEUS_COMPILER_EMPTY_BASE EventSuiteEnd final : public Event
	{
	public:
		suite_name_view_type name;
	};

	class PROMETHEUS_COMPILER_EMPTY_BASE EventSuite final : public Event
	{
	public:
		using suite_type = void (*)();

		suite_name_view_type name;
		suite_type suite;

		// throwable: end suite
		constexpr auto operator()() noexcept(false) -> void
		{
			std::invoke(suite);
		}

		// throwable: end suite
		constexpr auto operator()() const noexcept(false) -> void
		{
			std::invoke(suite);
		}

	private:
		[[nodiscard]] constexpr explicit operator EventSuiteBegin() const noexcept
		{
			return {.name = name};
		}

		[[nodiscard]] constexpr explicit operator EventSuiteEnd() const noexcept
		{
			return {.name = name};
		}

	public:
		[[nodiscard]] constexpr auto begin() const noexcept -> EventSuiteBegin
		{
			return this->operator EventSuiteBegin();
		}

		[[nodiscard]] constexpr auto end() const noexcept -> EventSuiteEnd
		{
			return this->operator EventSuiteEnd();
		}
	};

	// =========================================
	// TEST
	// =========================================

	class PROMETHEUS_COMPILER_EMPTY_BASE EventTestBegin final : public Event
	{
	public:
		test_name_view_type name;
	};

	class PROMETHEUS_COMPILER_EMPTY_BASE EventTestSkip final : public Event
	{
	public:
		test_name_view_type name;
	};

	class PROMETHEUS_COMPILER_EMPTY_BASE EventTestEnd final : public Event
	{
	public:
		test_name_view_type name;
	};

	struct none {};

	template<typename InvocableType, typename Arg = none>
		requires std::is_invocable_v<InvocableType> or std::is_invocable_v<InvocableType, Arg>
	class PROMETHEUS_COMPILER_EMPTY_BASE EventTest final : public Event
	{
	public:
		using invocable_type = InvocableType;
		using arg_type = Arg;

		test_name_view_type name;
		test_categories_type categories;

		mutable invocable_type invocable;
		mutable arg_type arg;

		// throwable: end test
		constexpr auto operator()() const noexcept(false) -> void
		{
			return []<typename I, typename A>(I&& i, A&& a) noexcept(false) -> void
			{
				if constexpr (requires { std::invoke(std::forward<I>(i)); })
				{
					std::invoke(std::forward<I>(i));
				}
				else if constexpr (requires { std::invoke(std::forward<I>(i), std::forward<A>(a)); })
				{
					std::invoke(std::forward<I>(i), std::forward<A>(a));
				}
				else if constexpr (requires { std::invoke(i.template operator()<A>()); })
				{
					std::invoke(i.template operator()<A>());
				}
				else
				{
					PROMETHEUS_SEMANTIC_STATIC_UNREACHABLE();
				}
			}(invocable, arg);
		}

	private:
		[[nodiscard]] constexpr explicit operator EventTestBegin() const noexcept
		{
			return {.name = name};
		}

		[[nodiscard]] constexpr explicit operator EventTestEnd() const noexcept
		{
			return {.name = name};
		}

		[[nodiscard]] constexpr explicit operator EventTestSkip() const noexcept
		{
			return {.name = name};
		}

	public:
		[[nodiscard]] constexpr auto begin() const noexcept -> EventTestBegin
		{
			return this->operator EventTestBegin();
		}

		[[nodiscard]] constexpr auto end() const noexcept -> EventTestEnd
		{
			return this->operator EventTestEnd();
		}

		[[nodiscard]] constexpr auto skip() const noexcept -> EventTestSkip
		{
			return this->operator EventTestSkip();
		}
	};

	// =========================================
	// ASSERTION
	// =========================================

	template<expression_t Expression>
	class PROMETHEUS_COMPILER_EMPTY_BASE EventAssertionPass final : public Event
	{
	public:
		using expression_type = Expression;

		expression_type expression;
		std::source_location location;
	};

	template<expression_t Expression>
	class PROMETHEUS_COMPILER_EMPTY_BASE EventAssertionFail final : public Event
	{
	public:
		using expression_type = Expression;

		expression_type expression;
		std::source_location location;
	};

	class PROMETHEUS_COMPILER_EMPTY_BASE EventAssertionFatal final : public Event
	{
	public:
		std::source_location location;
	};

	template<expression_t Expression>
	class PROMETHEUS_COMPILER_EMPTY_BASE EventAssertion final : public Event
	{
	public:
		using expression_type = Expression;

		expression_type expression;
		std::source_location location;

	private:
		[[nodiscard]] constexpr explicit operator EventAssertionPass<expression_type>() const noexcept
		{
			return {.expression = expression, .location = location};
		}

		[[nodiscard]] constexpr explicit operator EventAssertionFail<expression_type>() const noexcept
		{
			return {.expression = expression, .location = location};
		}

		[[nodiscard]] constexpr explicit operator EventAssertionFatal() const noexcept
		{
			return {.location = location};
		}

	public:
		[[nodiscard]] constexpr auto pass() const noexcept -> EventAssertionPass<expression_type>
		{
			// fixme: Compiler Error: C2273
			// return this->operator EventAssertionPass<expression_type>();
			return operator EventAssertionPass<expression_type>();
		}

		[[nodiscard]] constexpr auto fail() const noexcept -> EventAssertionFail<expression_type>
		{
			// fixme: Compiler Error: C2273
			// return this->operator EventAssertionFail<expression_type>();
			return operator EventAssertionFail<expression_type>();
		}

		[[nodiscard]] constexpr auto fatal() const noexcept -> EventAssertionFatal
		{
			// fixme: Compiler Error: C2273
			// return this->operator EventAssertionFatal();
			return operator EventAssertionFatal();
		}
	};

	// =========================================
	// UNEXPECTED
	// =========================================

	class PROMETHEUS_COMPILER_EMPTY_BASE EventUnexpected final : public Event
	{
	public:
		std::string_view message;

		[[nodiscard]] constexpr auto what() const noexcept -> std::string_view
		{
			return message;
		}
	};

	// =========================================
	// LOG
	// =========================================

	class PROMETHEUS_COMPILER_EMPTY_BASE EventLog final : public Event
	{
	public:
		std::string_view message;
	};

	// =========================================
	// SUMMARY
	// =========================================

	class PROMETHEUS_COMPILER_EMPTY_BASE EventSummary final : public Event {};

#if defined(PROMETHEUS_COMPILER_GNU) or defined(PROMETHEUS_COMPILER_APPLE_CLANG) or defined(PROMETHEUS_COMPILER_CLANG_CL) or defined(PROMETHEUS_COMPILER_CLANG)
	PROMETHEUS_COMPILER_DISABLE_WARNING_PUSH
#endif
}
