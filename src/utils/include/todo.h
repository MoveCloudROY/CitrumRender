#ifndef EG_TODO_H__
#define EG_TODO_H__

#include <iostream> 
#include <string>
#include <cstring>
#include <cstdlib>


namespace Task {
	#if defined(__GNUC__)
	#	define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
	#elif defined(_MSC_VER)
	#	define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
	#else
	#	error This compiler is not supported
	#endif

	#define PP_CAT(x,y) PP_CAT1(x,y)
	#define PP_CAT1(x,y) x##y

	namespace detail
	{
	    struct true_type {};
	    struct false_type {};
	    template <int test> struct converter : public true_type {};
	    template <> struct converter<0> : public false_type {};
	}

	#define STATIC_WARNING(cond, msg) 										\
	struct PP_CAT(static_warning,__LINE__) { 								\
		DEPRECATE(void _(detail::false_type const& ),msg) {}; 				\
		void _(detail::true_type const& ) {}; 								\
		PP_CAT(static_warning,__LINE__)() {_(detail::converter<(cond)>());} \
	}

	// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
	// It introduces a member/variable declaration.  This means at least one byte of space
	// in each structure/class instantiation.  STATIC_WARNING should be preferred in any 
	// non-template situation.
	//  'token' must be a program-wide unique identifier.
	#define STATIC_WARNING_TEMPLATE(token, cond, msg) \
	    STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)
	constexpr static const int N_YEAR =
		(__DATE__[ 7] - '0') * 1000							\
	  + (__DATE__[ 8] - '0') * 100 							\
	  + (__DATE__[ 9] - '0') * 10 							\
	  + (__DATE__[10] - '0');

	constexpr static const int N_MONTH = 					\
		  __DATE__[2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6)\
		: __DATE__[2] == 'b' ? 2 							\
		: __DATE__[2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4)\
		: __DATE__[2] == 'y' ? 5 							\
		: __DATE__[2] == 'l' ? 7 							\
		: __DATE__[2] == 'g' ? 8 							\
		: __DATE__[2] == 'p' ? 9 							\
		: __DATE__[2] == 't' ? 10 							\
		: __DATE__[2] == 'v' ? 11 							\
		: 12;

	constexpr static const int N_DAY =						\
	    (__DATE__[4] == ' '? 0 : (__DATE__[4] - '0')) * 10 	\
	  + (__DATE__[5] - '0');

	enum Month {
		JAN = 1, FEB, MAR, APR,
		MAY, 	 JUN, JUL, AUG,
		SEP, 	 OCT, NOV, DEC,
	};

	// Returns number of days since civil 1970-01-01.  Negative values indicate
	//    days prior to 1970-01-01.
	// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
	//                 m is in [1, 12]
	//                 d is in [1, last_day_of_month(y, m)]
	//                 y is "approximately" in
	//                   [numeric_limits<Int>::min()/366, numeric_limits<Int>::max()/366]
	//                 Exact range of validity is:
	//                 [civil_from_days(numeric_limits<Int>::min()),
	//                  civil_from_days(numeric_limits<Int>::max()-719468)]
	template <class Int>
	constexpr Int days_from_civil(Int y, unsigned m, unsigned d) noexcept
	{
	    y -= m <= 2;
	    const Int era = (y >= 0 ? y : y-399) / 400;
	    const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
	    const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
	    const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
	    return era * 146097 + static_cast<Int>(doe) - 719468;
	}

	template <int YEAR, int MONTH, int DAY, int WARNING_DAY = 14>
	constexpr void todo_deadline() {
		constexpr int n_days = days_from_civil(N_YEAR, N_MONTH, N_DAY);
		constexpr int set_days = days_from_civil(YEAR, MONTH, DAY);
		static_assert(set_days >= n_days, "The Deadline for this todo has dued!\n");
		STATIC_WARNING(set_days < n_days || set_days >= n_days + WARNING_DAY, 
			"The Deadline for this todo will due after two weeks\n"
		);
	}
}

#endif /* EG_TODO_H__ */
