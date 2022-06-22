/**
 * @file Format.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#define UINT32_MAX (0xffffffff)

template <typename T, typename U>
struct pair
{
    T a;
    U b;
};

struct floating_decimal_32
{
    u32 mantissa;
    i32 exponent;
};

struct floating_decimal_64
{
    u64 mantissa;
    i32 exponent;
};

internal macro u64 div5(u64 value)
{
    return value / 5;
}

internal macro u64 div10(u64 value)
{
    return value / 10;
}

internal macro u64 div100(u64 value)
{
    return value / 100;
}

internal macro u64 div1e8(u64 value)
{
    return value / 100000000;
}

internal macro u64 div1e9(u64 value)
{
    return value / 1000000000;
}

internal macro u32 decimalLength9(u32 value)
{
    // Function precondition: value is not a 10-digit number.
    // (f2s: 9 digits are sufficient for round-tripping.)
    // (d2fixed: We print 9-digit blocks.)
    assert(value < 1000000000);
    if (value >= 100000000)
    {
        return 9;
    }
    if (value >= 10000000)
    {
        return 8;
    }
    if (value >= 1000000)
    {
        return 7;
    }
    if (value >= 100000)
    {
        return 6;
    }
    if (value >= 10000)
    {
        return 5;
    }
    if (value >= 1000)
    {
        return 4;
    }
    if (value >= 100)
    {
        return 3;
    }
    if (value >= 10)
    {
        return 2;
    }
    return 1;
}

internal macro u32 decimalLength17(u64 value)
{
    // This is slightly faster than a loop.
    // The average output length is 16.38 digits, so we check high-to-low.
    // Function precondition: value is not an 18, 19, or 20-digit number.
    // (17 digits are sufficient for round-tripping.)
    assert(value < 100000000000000000u);
    if (value >= 10000000000000000u)
    {
        return 17;
    }
    if (value >= 1000000000000000u)
    {
        return 16;
    }
    if (value >= 100000000000000u)
    {
        return 15;
    }
    if (value >= 10000000000000u)
    {
        return 14;
    }
    if (value >= 1000000000000u)
    {
        return 13;
    }
    if (value >= 100000000000u)
    {
        return 12;
    }
    if (value >= 10000000000u)
    {
        return 11;
    }
    if (value >= 1000000000u)
    {
        return 10;
    }
    if (value >= 100000000u)
    {
        return 9;
    }
    if (value >= 10000000u)
    {
        return 8;
    }
    if (value >= 1000000u)
    {
        return 7;
    }
    if (value >= 100000u)
    {
        return 6;
    }
    if (value >= 10000u)
    {
        return 5;
    }
    if (value >= 1000u)
    {
        return 4;
    }
    if (value >= 100u)
    {
        return 3;
    }
    if (value >= 10u)
    {
        return 2;
    }
    return 1;
}

internal macro u32 pow5Factor(u32 value)
{
    u32 count = 0;
    for (;;)
    {
        assert(value != 0);
        u32 q = value / 5;
        u32 r = value % 5;
        if (r != 0)
        {
            break;
        }
        value = q;
        ++count;
    }
    return count;
}

// Returns e == 0 ? 1 : ceil(log_2(5^e)).
internal macro i32 pow5bits(i32 e)
{
    // This approximation works up to the point that the multiplication overflows at e =
    // 3529. If the multiplication were done in 64 bits, it would fail at 5^4004 which is
    // just greater than 2^9297.
    assert(e >= 0);
    assert(e <= 3528);
    return cast(i32, ((cast(u32, e) * 1217359) >> 19) + 1);
}

// Returns floor(log_10(2^e)).
internal macro u32 log10Pow2(i32 e)
{
    // The first value this approximation fails for is 2^1651 which is just greater than
    // 10^297.
    assert(e >= 0);
    assert(e <= 1650);
    return (cast(u32, e) * 78913) >> 18;
}

// Returns floor(log_10(5^e)).
internal macro u32 log10Pow5(i32 e)
{
    // The first value this approximation fails for is 5^2621 which is just greater than
    // 10^1832.
    assert(e >= 0);
    assert(e <= 2620);
    return (cast(u32, e) * 732923) >> 20;
}

u64 _umul128(u64 a, u64 b, u64 * hi_dest);
u64 __shiftleft128(u64 low_part, u64 high_part, u8 shift);
u64 __shiftright128(u64 low_part, u64 high_part, u8 shift);

internal macro u64 ryu_umul128(u64 a, u64 b, u64 * productHi)
{
    return _umul128(a, b, productHi);
}

internal macro u64 ryu_shiftright128(u64 lo, u64 hi, u32 dist)
{
    // For the shiftright128 intrinsic, the shift value is always
    // modulo 64.
    // In the current implementation of the double-precision version
    // of Ryu, the shift value is always < 64.
    // (The shift value is in the range [49, 58].)
    // Check this here in case a future change requires larger shift
    // values. In this case this function needs to be adjusted.
    assert(dist < 64);
    return __shiftright128(lo, hi, cast(u8, dist));
}

// Returns the low 64 bits of the high 128 bits of the 256-bit product of a and b.
internal macro u64 umul256_hi128_lo64(u64 aHi, u64 aLo, u64 bHi, u64 bLo)
{
    u64 b00Hi;
    u64 b00Lo = ryu_umul128(aLo, bLo, &b00Hi);
    u64 b01Hi;
    u64 b01Lo = ryu_umul128(aLo, bHi, &b01Hi);
    u64 b10Hi;
    u64 b10Lo = ryu_umul128(aHi, bLo, &b10Hi);
    u64 b11Hi;
    u64 b11Lo = ryu_umul128(aHi, bHi, &b11Hi);
    (void)b00Lo; // unused
    (void)b11Hi; // unused
    u64 temp1Lo = b10Lo + b00Hi;
    u64 temp1Hi = b10Hi + (temp1Lo < b10Lo);
    u64 temp2Lo = b01Lo + temp1Lo;
    u64 temp2Hi = b01Hi + (temp2Lo < b01Lo);
    return b11Lo + temp1Hi + temp2Hi;
}

internal macro u32 uint128_mod1e9(u64 vHi, u64 vLo)
{
    // After multiplying, we're going to shift right by 29, then truncate to u32.
    // This means that we need only 29 + 32 = 61 bits, so we can truncate to u64 before shifting.
    u64 multiplied = umul256_hi128_lo64(
        vHi,
        vLo,
        0x89705F4136B4A597u,
        0x31680A88F8953031u
    );

    // For u32 truncation, see the mod1e9() comment in d2s_intrinsics.h.
    u32 shifted = cast(u32, multiplied >> 29);

    return cast(u32, vLo) - 1000000000 * shifted;
}

internal macro u32 mulShift(u32 m, u64 factor, i32 shift)
{
    assert(shift > 32);

    // The casts here help MSVC to avoid calls to the allmul library
    // function.
    u32 factorLo = cast(u32, factor);
    u32 factorHi = cast(u32, factor >> 32);
    u64 bits0 = cast(u64, m) * factorLo;
    u64 bits1 = cast(u64, m) * factorHi;

    u64 sum = (bits0 >> 32) + bits1;
    u64 shiftedSum = sum >> (shift - 32);
    assert(shiftedSum <= UINT32_MAX);
    return cast(u32, shiftedSum);
}

internal macro u64 mulShift(u64 m, u64 const * mul, i32 j)
{
    // m is maximum 55 bits
    u64 high1;                                 // 128
    u64 low1 = ryu_umul128(m, mul[1], &high1); // 64
    u64 high0;                                 // 64
    (void)ryu_umul128(m, mul[0], &high0);      // 0
    u64 sum = high0 + low1;
    if (sum < high0)
    {
        ++high1; // overflow into high1
    }
    return ryu_shiftright128(sum, high1, cast(u32, j - 64));
}

internal macro u64 mulShiftAll( // clang-format is retarded
    u64 m,
    u64 const * mul,
    i32 j,
    u64 * vp,
    u64 * vm,
    u32 mmShift
)
{
    *vp = mulShift(4 * m + 2, mul, j);
    *vm = mulShift(4 * m - 1 - mmShift, mul, j);
    return mulShift(4 * m, mul, j);
}

internal macro u32 mulShift_mod1e9(u64 m, const u64 * mul, i32 j)
{
    u64 high0;                                 // 64
    u64 low0 = ryu_umul128(m, mul[0], &high0); // 0
    u64 high1;                                 // 128
    u64 low1 = ryu_umul128(m, mul[1], &high1); // 64
    u64 high2;                                 // 192
    u64 low2 = ryu_umul128(m, mul[2], &high2); // 128
    u64 s0low = low0;                          // 0
    (void)s0low;                               // unused
    u64 s0high = low1 + high0;                 // 64
    u32 c1 = s0high < low1;
    u64 s1low = low2 + high1 + c1; // 128
    u32 c2 = s1low < low2;         // high1 + c1 can't overflow, so compare against low2
    u64 s1high = high2 + c2;       // 192
    assert(j >= 128);
    assert(j <= 180);
    u32 dist = cast(u32, j - 128); // dist: [0, 52]
    u64 shiftedhigh = s1high >> dist;
    u64 shiftedlow = ryu_shiftright128(s1low, s1high, dist);
    return uint128_mod1e9(shiftedhigh, shiftedlow);
}

internal macro u32 mulPow5InvDivPow2(u32 m, u32 q, i32 j)
{
    return mulShift(m, FLOAT_POW5_INV_SPLIT[q], j);
}

internal macro u32 mulPow5divPow2(u32 m, u32 i, i32 j)
{
    return mulShift(m, FLOAT_POW5_SPLIT[i], j);
}

// Returns true if value is divisible by 5^p.
internal macro bool multipleOfPowerOf5(u32 value, u32 p)
{
    return pow5Factor(value) >= p;
}

// Returns true if value is divisible by 2^p.
internal macro bool multipleOfPowerOf2(u32 value, u32 p)
{
    assert(value != 0);
    assert(p < 32);
    return __builtin_ctz(value) >= p;
    // return (value & ((1u << p) - 1)) == 0;
}

// Returns true if value is divisible by 5^p.
internal macro bool multipleOfPowerOf5(u64 value, u32 p)
{
    // I tried a case distinction on p, but there was no performance difference.
    return pow5Factor(value) >= p;
}

// Returns true if value is divisible by 2^p.
internal macro bool multipleOfPowerOf2(u64 value, u32 p)
{
    assert(value != 0);
    assert(p < 64);
    return __builtin_ctzll(value) >= p;
    // return (value & ((1ull << p) - 1)) == 0;
}

internal macro void append_n_digits(u32 olength, u32 digits, char * result)
{
    u32 i = 0;
    char * start = result + olength;
    while (digits >= 10000)
    {
#ifdef __clang__ // TRANSITION, LLVM-38217
        u32 c = digits - 10000 * (digits / 10000);
#else
        u32 c = digits % 10000;
#endif
        digits /= 10000;
        u32 c0 = (c % 100) << 1;
        u32 c1 = (c / 100) << 1;
        util::raw_copy_range(start - i - 2, &DIGIT_TABLE[c0], 2 * sizeof(char));
        util::raw_copy_range(start - i - 4, &DIGIT_TABLE[c1], 2 * sizeof(char));
        i += 4;
    }
    if (digits >= 100)
    {
        u32 c = (digits % 100) << 1;
        digits /= 100;
        util::raw_copy_range(start - i - 2, &DIGIT_TABLE[c], 2 * sizeof(char));
        i += 2;
    }
    if (digits >= 10)
    {
        u32 c = digits << 1;
        util::raw_copy_range(start - i - 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    else
    {
        result[0] = cast(char, '0' + digits);
        return;
    }
}

internal macro void append_nine_digits(u32 digits, char * result)
{
    if (digits == 0)
    {
        util::raw_fill_range(result, 9, '0');
        return;
    }

    for (u32 i = 0; i < 5; i += 4)
    {
#ifdef __clang__ // TRANSITION, LLVM-38217
        u32 c = digits - 10000 * (digits / 10000);
#else
        u32 c = digits % 10000;
#endif
        digits /= 10000;
        u32 c0 = (c % 100) << 1;
        u32 c1 = (c / 100) << 1;
        util::raw_copy_range(result + 7 - i, &DIGIT_TABLE[c0], 2 * sizeof(char));
        util::raw_copy_range(result + 5 - i, &DIGIT_TABLE[c1], 2 * sizeof(char));
    }
    result[0] = cast(char, '0' + digits);
}

internal macro void append_c_digits(u32 count, u32 digits, char * result)
{
    u32 i = 0;
    for (; i < count - 1; i += 2)
    {
        u32 c = (digits % 100) << 1;
        digits /= 100;
        util::raw_copy_range(result + count - i - 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    if (i < count)
    {
        char c = cast(char, '0' + (digits % 10));
        result[count - i - 1] = c;
    }
}

internal macro u32 indexForExponent(u32 exponent)
{
    return (exponent + 15) / 16;
}

internal macro u32 pow10BitsForIndex(u32 idx)
{
    return 16 * idx + POW10_ADDITIONAL_BITS;
}

internal macro u32 lengthForIndex(u32 idx)
{
    // +1 for ceil, +16 for mantissa, +8 to round up when dividing by 9
    return (log10Pow2(16 * cast(i32, idx)) + 1 + 16 + 8) / 9;
}

internal macro pair<char *, bool> large_integer_to_chars(
    char * first,
    char * last,
    u32 mantissa2,
    i32 exponent2
)
{
    // Print the integer Mantissa2 * 2^Exponent2 exactly.

    // For nonzero integers, Exponent2 >= -23. (The minimum value occurs when Mantissa2 *
    // 2^Exponent2 is 1. In that case, Mantissa2 is the implicit 1 bit followed by 23
    // zeros, so Exponent2 is -23 to shift away the zeros.) The dense range of exactly
    // representable integers has negative or zero exponents (as positive exponents make
    // the range non-dense). For that dense range, Ryu will always be used: every digit is
    // necessary to uniquely identify the value, so Ryu must print them all.

    // Positive exponents are the non-dense range of exactly representable integers.
    // This contains all of the values for which Ryu can't be used (and a few Ryu-friendly
    // values).

    // Performance note: Long division appears to be faster than losslessly widening float
    // to double and calling d2fixed_buffered_n(). If f2fixed_buffered_n() is
    // implemented, it might be faster than long division.

    assert(exponent2 > 0);
    assert(exponent2 <= 104); // because ieeeExponent <= 254

    // Manually represent Mantissa2 * 2^Exponent2 as a large integer. Mantissa2 is always
    // 24 bits (due to the implicit bit), while Exponent2 indicates a shift of at most 104
    // bits. 24 + 104 equals 128 equals 4 * 32, so we need exactly 4 32-bit elements. We
    // use a little-endian representation, visualized like this:

    // << left shift <<
    // most significant
    // data[3] data[2] data[1] data[0]
    //                   least significant
    //                   >> right shift >>

    constexpr u32 data_size = 4;
    u32 data[data_size] {};

    // max_idx is the index of the most significant nonzero element.
    u32 max_idx = ((24 + cast(u32, exponent2) + 31) / 32) - 1;
    assert(max_idx < data_size);

    u32 bit_shift = cast(u32, exponent2) % 32;
    if (bit_shift <= 8)
    {
        // Mantissa2's 24 bits don't cross an element boundary
        data[max_idx] = mantissa2 << bit_shift;
    }
    else
    {
        // Mantissa2's 24 bits cross an element boundary
        data[max_idx - 1] = mantissa2 << bit_shift;
        data[max_idx] = mantissa2 >> (32 - bit_shift);
    }

    // If Ryu hasn't determined the total output length, we need to buffer the digits
    // generated from right to left by long division. The largest possible float is:
    // 340'282346638'528859811'704183484'516925440
    u32 blocks[4];
    i32 filled_blocks = 0;
    // From left to right, we're going to print:
    // data[0] will be [1, 10] digits.
    // Then if filled_blocks > 0:
    // blocks[filled_blocks - 1], ..., blocks[0] will be 0-filled 9-digit blocks.

    if (max_idx != 0)
    { // If the integer is actually large, perform long division.
        // Otherwise, skip to printing data[0].
        for (;;)
        {
            // Loop invariant: max_idx != 0 (i.e. the integer is actually large)

            u32 most_significant_element = data[max_idx];
            u32 initial_remainder = most_significant_element % 1000000000;
            u32 initial_quotient = most_significant_element / 1000000000;
            data[max_idx] = initial_quotient;
            u64 Remainder = initial_remainder;

            // Process less significant elements.
            u32 idx = max_idx;
            do
            {
                --idx; // Initially, Remainder is at most 10^9 - 1.

                // Now, Remainder is at most (10^9 - 1) * 2^32 + 2^32 - 1, simplified to
                // 10^9 * 2^32 - 1.
                Remainder = (Remainder << 32) | data[idx];

                // floor((10^9 * 2^32 - 1) / 10^9) == 2^32 - 1, so u32 Quotient is
                // lossless.
                u32 Quotient = cast(u32, div1e9(Remainder));

                // Remainder is at most 10^9 - 1 again.
                // For u32 truncation, see the mod1e9() comment in d2s_intrinsics.h.
                Remainder = cast(u32, Remainder) - 1000000000u * Quotient;

                data[idx] = Quotient;
            } while (idx != 0);

            // Store a 0-filled 9-digit block.
            blocks[filled_blocks++] = cast(u32, Remainder);

            if (initial_quotient == 0)
            {              // Is the large integer shrinking?
                --max_idx; // log2(10^9) is 29.9, so we can't shrink by more than one
                           // element.
                if (max_idx == 0)
                {
                    break; // We've finished long division. Now we need to print data[0].
                }
            }
        }
    }

    assert(data[0] != 0);
    for (u32 idx = 1; idx < data_size; ++idx)
    {
        assert(data[idx] == 0);
    }

    u32 data_olength = data[0] >= 1000000000 ? 10 : decimalLength9(data[0]);
    u32 total_fixed_length = data_olength + 9 * filled_blocks;

    if (last - first < cast(i64, total_fixed_length))
    {
        return { last, false };
    }

    char * result = first;

    // Print data[0]. While it's up to 10 digits,
    // which is more than Ryu generates, the code below can handle this.
    append_n_digits(data_olength, data[0], result);
    result += data_olength;

    // Print 0-filled 9-digit blocks.
    for (i32 idx = filled_blocks - 1; idx >= 0; --idx)
    {
        append_nine_digits(blocks[idx], result);
        result += 9;
    }

    return { result, true };
}

internal macro floating_decimal_32 f2d(u32 ieeeMantissa, u32 ieeeExponent)
{
    i32 e2;
    u32 m2;
    if (ieeeExponent == 0)
    {
        // We subtract 2 so that the bounds computation has 2 additional bits.
        e2 = 1 - FLOAT_BIAS - FLOAT_MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    }
    else
    {
        e2 = cast(i32, ieeeExponent) - FLOAT_BIAS - FLOAT_MANTISSA_BITS - 2;
        m2 = (1u << FLOAT_MANTISSA_BITS) | ieeeMantissa;
    }
    bool even = (m2 & 1) == 0;
    bool acceptBounds = even;

    // Step 2: Determine the interval of valid decimal representations.
    u32 mv = 4 * m2;
    u32 mp = 4 * m2 + 2;
    // Implicit bool -> int conversion. True is 1, false is 0.
    u32 mmShift = ieeeMantissa != 0 || ieeeExponent <= 1;
    u32 mm = 4 * m2 - 1 - mmShift;

    // Step 3: Convert to a decimal power base using 64-bit arithmetic.
    u32 vr, vp, vm;
    i32 e10;
    bool vmIsTrailingZeros = false;
    bool vrIsTrailingZeros = false;
    u8 lastRemovedDigit = 0;
    if (e2 >= 0)
    {
        u32 q = log10Pow2(e2);
        e10 = cast(i32, q);
        i32 k = FLOAT_POW5_INV_BITCOUNT + pow5bits(cast(i32, q)) - 1;
        i32 i = -e2 + cast(i32, q) + k;
        vr = mulPow5InvDivPow2(mv, q, i);
        vp = mulPow5InvDivPow2(mp, q, i);
        vm = mulPow5InvDivPow2(mm, q, i);
        if (q != 0 && (vp - 1) / 10 <= vm / 10)
        {
            // We need to know one removed digit even if we are not going to loop below.
            // We could use q = X - 1 above, except that would require 33 bits for the
            // result, and we've found that 32-bit arithmetic is faster even on 64-bit
            // machines.
            i32 l = FLOAT_POW5_INV_BITCOUNT + pow5bits(cast(i32, q - 1)) - 1;
            lastRemovedDigit = cast(
                u8,
                mulPow5InvDivPow2(mv, q - 1, -e2 + cast(i32, q) - 1 + l) % 10
            );
        }
        if (q <= 9)
        {
            // The largest power of 5 that fits in 24 bits is 5^10, but q <= 9 seems to be
            // safe as well. Only one of mp, mv, and mm can be a multiple of 5, if any.
            if (mv % 5 == 0)
            {
                vrIsTrailingZeros = multipleOfPowerOf5(mv, q);
            }
            else if (acceptBounds)
            {
                vmIsTrailingZeros = multipleOfPowerOf5(mm, q);
            }
            else
            {
                vp -= multipleOfPowerOf5(mp, q);
            }
        }
    }
    else
    {
        u32 q = log10Pow5(-e2);
        e10 = cast(i32, q) + e2;
        i32 i = -e2 - cast(i32, q);
        i32 k = pow5bits(i) - FLOAT_POW5_BITCOUNT;
        i32 j = cast(i32, q) - k;
        vr = mulPow5divPow2(mv, cast(u32, i), j);
        vp = mulPow5divPow2(mp, cast(u32, i), j);
        vm = mulPow5divPow2(mm, cast(u32, i), j);
        if (q != 0 && (vp - 1) / 10 <= vm / 10)
        {
            j = cast(i32, q) - 1 - (pow5bits(i + 1) - FLOAT_POW5_BITCOUNT);
            lastRemovedDigit = cast(u8, mulPow5divPow2(mv, cast(u32, i + 1), j) % 10);
        }
        if (q <= 1)
        {
            // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
            // mv = 4 * m2, so it always has at least two trailing 0 bits.
            vrIsTrailingZeros = true;
            if (acceptBounds)
            {
                // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit if mmShift == 1.
                vmIsTrailingZeros = mmShift == 1;
            }
            else
            {
                // mp = mv + 2, so it always has at least one trailing 0 bit.
                --vp;
            }
        }
        else if (q < 31)
        { // TRANSITION(ulfjack): Use a tighter bound here.
            vrIsTrailingZeros = multipleOfPowerOf2(mv, q - 1);
        }
    }

    // Step 4: Find the shortest decimal representation in the interval of valid
    // representations.
    i32 removed = 0;
    u32 Output;
    if (vmIsTrailingZeros || vrIsTrailingZeros)
    {
        // General case, which happens rarely (~4.0%).
        while (vp / 10 > vm / 10)
        {
#ifdef __clang__ // TRANSITION, LLVM-23106
            vmIsTrailingZeros &= vm - (vm / 10) * 10 == 0;
#else
            vmIsTrailingZeros &= vm % 10 == 0;
#endif
            vrIsTrailingZeros &= lastRemovedDigit == 0;
            lastRemovedDigit = cast(u8, vr % 10);
            vr /= 10;
            vp /= 10;
            vm /= 10;
            ++removed;
        }
        if (vmIsTrailingZeros)
        {
            while (vm % 10 == 0)
            {
                vrIsTrailingZeros &= lastRemovedDigit == 0;
                lastRemovedDigit = cast(u8, vr % 10);
                vr /= 10;
                vp /= 10;
                vm /= 10;
                ++removed;
            }
        }
        if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0)
        {
            // Round even if the exact number is .....50..0.
            lastRemovedDigit = 4;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        Output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) ||
                       lastRemovedDigit >= 5);
    }
    else
    {
        // Specialized for the common case (~96.0%). Percentages below are relative to
        // this. Loop iterations below (approximately): 0: 13.6%, 1: 70.7%, 2: 14.1%,
        // 3: 1.39%, 4: 0.14%, 5+: 0.01%
        while (vp / 10 > vm / 10)
        {
            lastRemovedDigit = cast(u8, vr % 10);
            vr /= 10;
            vp /= 10;
            vm /= 10;
            ++removed;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        Output = vr + (vr == vm || lastRemovedDigit >= 5);
    }
    i32 exp = e10 + removed;

    floating_decimal_32 fd;
    fd.exponent = exp;
    fd.mantissa = Output;
    return fd;
}

internal macro floating_decimal_64 d2d(u64 ieeeMantissa, u32 ieeeExponent)
{
    i32 e2;
    u64 m2;
    if (ieeeExponent == 0)
    {
        // We subtract 2 so that the bounds computation has 2 additional bits.
        e2 = 1 - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    }
    else
    {
        e2 = cast(i32, ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    }
    bool even = (m2 & 1) == 0;
    bool acceptBounds = even;

    // Step 2: Determine the interval of valid decimal representations.
    u64 mv = 4 * m2;
    // Implicit bool -> int conversion. True is 1, false is 0.
    u32 mmShift = ieeeMantissa != 0 || ieeeExponent <= 1;
    // We would compute mp and mm like this:
    // u64 mp = 4 * m2 + 2;
    // u64 mm = mv - 1 - mmShift;

    // Step 3: Convert to a decimal power base using 128-bit arithmetic.
    u64 vr, vp, vm;
    i32 e10;
    bool vmIsTrailingZeros = false;
    bool vrIsTrailingZeros = false;
    if (e2 >= 0)
    {
        // I tried special-casing q == 0, but there was no effect on performance.
        // This expression is slightly faster than max(0, log10Pow2(e2) - 1).
        u32 q = log10Pow2(e2) - (e2 > 3);
        e10 = cast(i32, q);
        i32 k = DOUBLE_POW5_INV_BITCOUNT + pow5bits(cast(i32, q)) - 1;
        i32 i = -e2 + cast(i32, q) + k;
        vr = mulShiftAll(m2, DOUBLE_POW5_INV_SPLIT[q], i, &vp, &vm, mmShift);
        if (q <= 21)
        {
            // This should use q <= 22, but I think 21 is also safe. Smaller values
            // may still be safe, but it's more difficult to reason about them.
            // Only one of mp, mv, and mm can be a multiple of 5, if any.
            u32 mvMod5 = cast(u32, mv) - 5 * cast(u32, div5(mv));
            if (mvMod5 == 0)
            {
                vrIsTrailingZeros = multipleOfPowerOf5(mv, q);
            }
            else if (acceptBounds)
            {
                // Same as min(e2 + (~mm & 1), pow5Factor(mm)) >= q
                // <=> e2 + (~mm & 1) >= q && pow5Factor(mm) >= q
                // <=> true && pow5Factor(mm) >= q, since e2 >= q.
                vmIsTrailingZeros = multipleOfPowerOf5(mv - 1 - mmShift, q);
            }
            else
            {
                // Same as min(e2 + 1, pow5Factor(mp)) >= q.
                vp -= multipleOfPowerOf5(mv + 2, q);
            }
        }
    }
    else
    {
        // This expression is slightly faster than max(0, log10Pow5(-e2) - 1).
        u32 q = log10Pow5(-e2) - (-e2 > 1);
        e10 = cast(i32, q) + e2;
        i32 i = -e2 - cast(i32, q);
        i32 k = pow5bits(i) - DOUBLE_POW5_BITCOUNT;
        i32 j = cast(i32, q) - k;
        vr = mulShiftAll(m2, DOUBLE_POW5_SPLIT[i], j, &vp, &vm, mmShift);
        if (q <= 1)
        {
            // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
            // mv = 4 * m2, so it always has at least two trailing 0 bits.
            vrIsTrailingZeros = true;
            if (acceptBounds)
            {
                // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit if mmShift == 1.
                vmIsTrailingZeros = mmShift == 1;
            }
            else
            {
                // mp = mv + 2, so it always has at least one trailing 0 bit.
                --vp;
            }
        }
        else if (q < 63)
        { // TRANSITION(ulfjack): Use a tighter bound here.
            // We need to compute min(ntz(mv), pow5Factor(mv) - e2) >= q - 1
            // <=> ntz(mv) >= q - 1 && pow5Factor(mv) - e2 >= q - 1
            // <=> ntz(mv) >= q - 1 (e2 is negative and - e2 >= q)
            // <=> (mv & ((1 << (q - 1)) - 1)) == 0
            // We also need to make sure that the left shift does not overflow.
            vrIsTrailingZeros = multipleOfPowerOf2(mv, q - 1);
        }
    }

    // Step 4: Find the shortest decimal representation in the interval of valid representations.
    i32 removed = 0;
    u8 lastRemovedDigit = 0;
    u64 output;
    // On average, we remove ~2 digits.
    if (vmIsTrailingZeros || vrIsTrailingZeros)
    {
        // General case, which happens rarely (~0.7%).
        for (;;)
        {
            u64 vpDiv10 = div10(vp);
            u64 vmDiv10 = div10(vm);
            if (vpDiv10 <= vmDiv10)
            {
                break;
            }
            u32 vmMod10 = cast(u32, vm) - 10 * cast(u32, vmDiv10);
            u64 vrDiv10 = div10(vr);
            u32 vrMod10 = cast(u32, vr) - 10 * cast(u32, vrDiv10);
            vmIsTrailingZeros &= vmMod10 == 0;
            vrIsTrailingZeros &= lastRemovedDigit == 0;
            lastRemovedDigit = cast(u8, vrMod10);
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        if (vmIsTrailingZeros)
        {
            for (;;)
            {
                u64 vmDiv10 = div10(vm);
                u32 vmMod10 = cast(u32, vm) - 10 * cast(u32, vmDiv10);
                if (vmMod10 != 0)
                {
                    break;
                }
                u64 vpDiv10 = div10(vp);
                u64 vrDiv10 = div10(vr);
                u32 vrMod10 = cast(u32, vr) - 10 * cast(u32, vrDiv10);
                vrIsTrailingZeros &= lastRemovedDigit == 0;
                lastRemovedDigit = cast(u8, vrMod10);
                vr = vrDiv10;
                vp = vpDiv10;
                vm = vmDiv10;
                ++removed;
            }
        }
        if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0)
        {
            // Round even if the exact number is .....50..0.
            lastRemovedDigit = 4;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) ||
                       lastRemovedDigit >= 5);
    }
    else
    {
        // Specialized for the common case (~99.3%). Percentages below are relative to this.
        bool roundUp = false;
        u64 vpDiv100 = div100(vp);
        u64 vmDiv100 = div100(vm);
        if (vpDiv100 > vmDiv100)
        { // Optimization: remove two digits at a time (~86.2%).
            u64 vrDiv100 = div100(vr);
            u32 vrMod100 = cast(u32, vr) - 100 * cast(u32, vrDiv100);
            roundUp = vrMod100 >= 50;
            vr = vrDiv100;
            vp = vpDiv100;
            vm = vmDiv100;
            removed += 2;
        }
        // Loop iterations below (approximately), without optimization above:
        // 0: 0.03%, 1: 13.8%, 2: 70.6%, 3: 14.0%, 4: 1.40%, 5: 0.14%, 6+: 0.02%
        // Loop iterations below (approximately), with optimization above:
        // 0: 70.6%, 1: 27.8%, 2: 1.40%, 3: 0.14%, 4+: 0.02%
        for (;;)
        {
            u64 vpDiv10 = div10(vp);
            u64 vmDiv10 = div10(vm);
            if (vpDiv10 <= vmDiv10)
            {
                break;
            }
            u64 vrDiv10 = div10(vr);
            u32 vrMod10 = cast(u32, vr) - 10 * cast(u32, vrDiv10);
            roundUp = vrMod10 >= 5;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        output = vr + (vr == vm || roundUp);
    }
    i32 exp = e10 + removed;

    floating_decimal_64 fd;
    fd.exponent = exp;
    fd.mantissa = output;
    return fd;
}

internal macro bool d2d_small_int( // clang-format is retarded
    u64 ieeeMantissa,
    u32 ieeeExponent,
    floating_decimal_64 * v
)
{
    u64 m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    u32 e2 = cast(u32, ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS;

    if (e2 > 0)
    {
        // f = m2 * 2^e2 >= 2^53 is an integer.
        // Ignore this case for now.
        return false;
    }

    if (e2 < -52)
    {
        // f < 1.
        return false;
    }

    // Since 2^52 <= m2 < 2^53 and 0 <= -e2 <= 52: 1 <= f = m2 / 2^-e2 < 2^53.
    // Test if the lower -e2 bits of the significand are 0, i.e. whether the fraction is 0.
    u64 mask = (1ull << -e2) - 1;
    u64 fraction = m2 & mask;
    if (fraction != 0)
    {
        return false;
    }

    // f is an integer in the range [1, 2^53).
    // Note: mantissa might contain trailing (decimal) 0's.
    // Note: since 2^53 < 10^16, there is no need to adjust decimalLength17().
    v->mantissa = m2 >> -e2;
    v->exponent = 0;
    return true;
}

internal macro pair<char *, bool> d2fixed_buffered_n(
    char * first,
    char * last,
    f64 value,
    u32 precision
)
{
    char * _Original_first = first;

    u64 bits = *cast(u64 *, &value);

    // Case distinction; exit early for the easy cases.
    if (bits == 0)
    {
        // possible decimal point
        // zeroes after decimal point
        // leading zero
        i32 total_zero_length = (precision != 0) + (precision) + 1;

        if (last - first < total_zero_length)
        {
            return { last, false };
        }

        *first++ = '0';
        if (precision > 0)
        {
            *first++ = '.';
            util::raw_fill_range(first, precision, '0');
            first += precision;
        }
        return { first, true };
    }

    // Decode bits into mantissa and exponent.
    u64 ieeeMantissa = bits & ((1ull << DOUBLE_MANTISSA_BITS) - 1);
    u32 ieeeExponent = cast(u32, bits >> DOUBLE_MANTISSA_BITS);

    i32 e2;
    u64 m2;
    if (ieeeExponent == 0)
    {
        e2 = 1 - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS;
        m2 = ieeeMantissa;
    }
    else
    {
        e2 = cast(i32, ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS;
        m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    }

    bool nonzero = false;
    if (e2 >= -52)
    {
        u32 idx = e2 < 0 ? 0 : indexForExponent(cast(u32, e2));
        u32 p10bits = pow10BitsForIndex(idx);
        i32 len = cast(i32, lengthForIndex(idx));
        for (i32 i = len - 1; i >= 0; --i)
        {
            u32 j = p10bits - e2;
            // Temporary: j is usually around 128, and by shifting a bit, we push it to 128 or above, which is
            // a slightly faster code path in mulShift_mod1e9. Instead, we can just increase the multipliers.
            u32 digits = mulShift_mod1e9(
                m2 << 8,
                POW10_SPLIT[POW10_OFFSET[idx] + i],
                cast(i32, j + 8)
            );
            if (nonzero)
            {
                if (last - first < 9)
                {
                    return { last, false };
                }
                append_nine_digits(digits, first);
                first += 9;
            }
            else if (digits != 0)
            {
                u32 olength = decimalLength9(digits);
                if (last - first < cast(i64, olength))
                {
                    return { last, false };
                }
                append_n_digits(olength, digits, first);
                first += olength;
                nonzero = true;
            }
        }
    }
    if (!nonzero)
    {
        if (first == last)
        {
            return { last, false };
        }
        *first++ = '0';
    }
    if (precision > 0)
    {
        if (first == last)
        {
            return { last, false };
        }
        *first++ = '.';
    }
    if (e2 < 0)
    {
        i32 idx = -e2 / 16;
        u32 blocks = precision / 9 + 1;
        // 0 = don't round up; 1 = round up unconditionally; 2 = round up if odd.
        int roundUp = 0;
        u32 i = 0;
        if (blocks <= MIN_BLOCK_2[idx])
        {
            i = blocks;
            if (last - first < cast(i64, precision))
            {
                return { last, false };
            }
            util::raw_fill_range(first, precision, '0');
            first += precision;
        }
        else if (i < MIN_BLOCK_2[idx])
        {
            i = MIN_BLOCK_2[idx];
            if (last - first < cast(i64, 9 * i))
            {
                return { last, false };
            }
            util::raw_fill_range(first, 9 * i, '0');
            first += 9 * i;
        }
        for (; i < blocks; ++i)
        {
            i32 j = ADDITIONAL_BITS_2 + (-e2 - 16 * idx);
            u32 p = POW10_OFFSET_2[idx] + i - MIN_BLOCK_2[idx];
            if (p >= POW10_OFFSET_2[idx + 1])
            {
                // If the remaining digits are all 0, then we might as well use memset.
                // No rounding required in this case.
                u32 fill = precision - 9 * i;
                if (last - first < cast(i64, fill))
                {
                    return { last, false };
                }
                util::raw_fill_range(first, fill, '0');
                first += fill;
                break;
            }
            // Temporary: j is usually around 128, and by shifting a bit, we push it to 128 or above, which is
            // a slightly faster code path in mulShift_mod1e9. Instead, we can just increase the multipliers.
            u32 digits = mulShift_mod1e9(m2 << 8, POW10_SPLIT_2[p], j + 8);
            if (i < blocks - 1)
            {
                if (last - first < 9)
                {
                    return { last, false };
                }
                append_nine_digits(digits, first);
                first += 9;
            }
            else
            {
                u32 maximum = precision - 9 * i;
                u32 lastDigit = 0;
                for (u32 k = 0; k < 9 - maximum; ++k)
                {
                    lastDigit = digits % 10;
                    digits /= 10;
                }
                if (lastDigit != 5)
                {
                    roundUp = lastDigit > 5;
                }
                else
                {
                    // Is m * 10^(additionalDigits + 1) / 2^(-e2) integer?
                    i32 requiredTwos = -e2 - cast(i32, precision) - 1;
                    bool trailingZeros =
                        (requiredTwos <= 0 ||
                         (requiredTwos < 60 &&
                          multipleOfPowerOf2(m2, cast(u32, requiredTwos))));
                    roundUp = trailingZeros ? 2 : 1;
                }
                if (maximum > 0)
                {
                    if (last - first < cast(i64, maximum))
                    {
                        return { last, false };
                    }
                    append_c_digits(maximum, digits, first);
                    first += maximum;
                }
                break;
            }
        }
        if (roundUp != 0)
        {
            char * round = first;
            char * dot = last;
            while (true)
            {
                if (round == _Original_first)
                {
                    round[0] = '1';
                    if (dot != last)
                    {
                        dot[0] = '0';
                        dot[1] = '.';
                    }
                    if (first == last)
                    {
                        return { last, false };
                    }
                    *first++ = '0';
                    break;
                }
                --round;
                char c = round[0];
                if (c == '.')
                {
                    dot = round;
                }
                else if (c == '9')
                {
                    round[0] = '0';
                    roundUp = 1;
                }
                else
                {
                    if (roundUp == 1 || c % 2 != 0)
                    {
                        round[0] = cast(char, c + 1);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        if (last - first < cast(i64, precision))
        {
            return { last, false };
        }
        util::raw_fill_range(first, precision, '0');
        first += precision;
    }
    return { first, true };
}

internal macro pair<char *, bool> internal_to_chars(
    char * first,
    char * last,
    floating_decimal_32 v,
    u32 ieeeMantissa,
    u32 ieeeExponent
)
{
    // Step 5: Print the decimal representation.
    u32 output = v.mantissa;
    i32 ryu_exponent = v.exponent;
    u32 olength = decimalLength9(output);
    i32 scientific_exponent = ryu_exponent + cast(i32, olength) - 1;
    // Example: output == 1729, olength == 4

    // ryu_exponent  | Printed  | whole_digits  | total_fixed_length   | Notes
    // --------------|----------|---------------|----------------------|---------------------------------------
    //             2 | 172900   |  6            | whole_digits         | Ryu can't be used for printing
    //             1 | 17290    |  5            | (sometimes adjusted) | when the trimmed digits are nonzero.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //             0 | 1729     |  4            | whole_digits         | Unified length cases.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //            -1 | 172.9    |  3            | olength + 1          | This case can't happen for
    //            -2 | 17.29    |  2            |                      | olength == 1, but no additional
    //            -3 | 1.729    |  1            |                      | code is needed to avoid it.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //            -4 | 0.1729   |  0            | 2 - ryu_exponent     | C11 7.21.6.1 "The fprintf function"/8:
    //            -5 | 0.01729  | -1            |                      | "If a decimal-point character appears,
    //            -6 | 0.001729 | -2            |                      | at least one digit appears before it."

    i32 whole_digits = cast(i32, olength) + ryu_exponent;

    u32 total_fixed_length;
    if (ryu_exponent >= 0)
    {
        // cases "172900" and "1729"
        total_fixed_length = cast(u32, whole_digits);
        if (output == 1)
        {
            // Rounding can affect the number of digits.
            // For example, 1e11f is exactly "99999997952" which is 11 digits instead of 12.
            // We can use a lookup table to detect this and adjust the total length.
            static constexpr u8 adjustment[39] = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
                0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1,
            };
            total_fixed_length -= adjustment[ryu_exponent];
            // whole_digits doesn't need to be adjusted because these cases won't refer to it later.
        }
    }
    else if (whole_digits > 0)
    {
        // case "17.29"
        total_fixed_length = olength + 1;
    }
    else
    {
        // case "0.001729"
        total_fixed_length = cast(u32, 2 - ryu_exponent);
    }

    if (last - first < cast(i64, total_fixed_length))
    {
        return { last, false };
    }

    char * mid;
    if (ryu_exponent > 0)
    {
        // case "172900"
        bool can_use_ryu;

        if (ryu_exponent > 10)
        {
            // 10^10 is the largest power of 10 that's exactly representable as a float.
            can_use_ryu = false;
        }
        else
        {
            // Ryu generated X: v.mantissa * 10^ryu_exponent
            // v.mantissa == 2^trailing_zero_bits * (v.mantissa >> trailing_zero_bits)
            // 10^ryu_exponent == 2^ryu_exponent * 5^ryu_exponent

            // trailing_zero_bits is [0, 29] (aside: because 2^29 is the largest power of 2
            // with 9 decimal digits, which is float's round-trip limit.)
            // ryu_exponent is [1, 10].
            // Normalization adds [2, 23] (aside: at least 2 because the pre-normalized mantissa
            // is at least 5). This adds up to [3, 62], which is well below float's maximum
            // binary exponent 127.

            // Therefore, we just need to consider (v.mantissa >> trailing_zero_bits) *
            // 5^ryu_exponent.

            // If that product would exceed 24 bits, then X can't be exactly represented as a
            // float. (That's not a problem for round-tripping, because X is close enough to the
            // original float, but X isn't mathematically equal to the original float.) This
            // requires a high-precision fallback.

            // If the product is 24 bits or smaller, then X can be exactly represented as a
            // float (and we don't need to re-synthesize it; the original float must have been
            // X, because Ryu wouldn't produce the same output for two different floats X and
            // Y). This allows Ryu's output to be used (zero-filled).

            // (2^24 - 1) / 5^0 (for indexing), (2^24 - 1) / 5^1, ..., (2^24 - 1) / 5^10
            static constexpr u32 max_shifted_mantissa[11] = {
                16777215, 3355443, 671088, 134217, 26843, 5368, 1073, 214, 42, 8, 1
            };

            // v.mantissa is guaranteed nonzero
            u32 trailing_zero_bits = __builtin_ctz(v.mantissa);
            u32 shifted_mantissa = v.mantissa >> trailing_zero_bits;
            can_use_ryu = shifted_mantissa <= max_shifted_mantissa[ryu_exponent];
        }

        if (!can_use_ryu)
        {
            // 1. restore implicit bit
            // 2. bias and normalization
            u32 mantissa2 = ieeeMantissa | (1u << FLOAT_MANTISSA_BITS);
            i32 exponent2 = cast(i32, ieeeExponent) - FLOAT_BIAS - FLOAT_MANTISSA_BITS;

            // Performance note: We've already called Ryu, so this will redundantly perform
            // buffering and bounds checking.
            return large_integer_to_chars(first, last, mantissa2, exponent2);
        }

        // can_use_ryu
        // Print the decimal digits, left-aligned within [first, first + total_fixed_length).
        mid = first + olength;
    }
    else
    {
        // cases "1729", "17.29", and "0.001729"
        // Print the decimal digits, right-aligned within [first, first + total_fixed_length).
        mid = first + total_fixed_length;
    }

    while (output >= 10000)
    {
#ifdef __clang__ // TRANSITION, LLVM-38217
        u32 c = output - 10000 * (output / 10000);
#else
        u32 c = output % 10000;
#endif
        output /= 10000;
        u32 c0 = (c % 100) << 1;
        u32 c1 = (c / 100) << 1;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c0], 2 * sizeof(char));
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c1], 2 * sizeof(char));
    }
    if (output >= 100)
    {
        u32 c = (output % 100) << 1;
        output /= 100;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    if (output >= 10)
    {
        u32 c = output << 1;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    else
    {
        *--mid = cast(char, '0' + output);
    }

    if (ryu_exponent > 0)
    {
        // case "172900" with can_use_ryu
        // Performance note: it might be more efficient to do this immediately after setting
        // _Mid.
        util::raw_fill_range(first + olength, ryu_exponent, '0');
    }
    else if (ryu_exponent == 0)
    {
        // case "1729"
        // Done!
    }
    else if (whole_digits > 0)
    {
        // case "17.29"
        // Performance note: moving digits might not be optimal.
        util::raw_copy_range(first, first + 1, cast(u64, whole_digits) * sizeof(char));
        first[whole_digits] = '.';
    }
    else
    {
        // case "0.001729"
        // Performance note: a larger memset() followed by overwriting '.' might be more
        // efficient.
        first[0] = '0';
        first[1] = '.';
        util::raw_fill_range(first + 2, -whole_digits, '0');
    }

    return { first + total_fixed_length, true };
}

internal macro pair<char *, bool> internal_to_chars(
    char * first,
    char * last,
    floating_decimal_64 v,
    double value
)
{
    // Step 5: Print the decimal representation.
    u64 output = v.mantissa;
    i32 ryu_exponent = v.exponent;
    u32 olength = decimalLength17(output);
    i32 scientific_exponent = ryu_exponent + cast(i32, olength) - 1;

    // Example: output == 1729, olength == 4

    // ryu_exponent  | Printed  | whole_digits  | total_fixed_length   | Notes
    // --------------|----------|---------------|----------------------|---------------------------------------
    //             2 | 172900   |  6            | whole_digits         | Ryu can't be used for printing
    //             1 | 17290    |  5            | (sometimes adjusted) | when the trimmed digits are nonzero.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //             0 | 1729     |  4            | whole_digits         | Unified length cases.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //            -1 | 172.9    |  3            | olength + 1          | This case can't happen for
    //            -2 | 17.29    |  2            |                      | olength == 1, but no additional
    //            -3 | 1.729    |  1            |                      | code is needed to avoid it.
    // --------------|----------|---------------|----------------------|---------------------------------------
    //            -4 | 0.1729   |  0            | 2 - ryu_exponent     | C11 7.21.6.1 "The fprintf function"/8:
    //            -5 | 0.01729  | -1            |                      | "If a decimal-point character appears,
    //            -6 | 0.001729 | -2            |                      | at least one digit appears before it."

    i32 whole_digits = cast(i32, olength) + ryu_exponent;

    u32 total_fixed_length;
    if (ryu_exponent >= 0)
    { // cases "172900" and "1729"
        total_fixed_length = cast(u32, whole_digits);
        if (output == 1)
        {
            // Rounding can affect the number of digits.
            // For example, 1e23 is exactly "99999999999999991611392" which is 23 digits instead of 24.
            // We can use a lookup table to detect this and adjust the total length.
            static constexpr u8 adjustment[309] {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
                0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
                1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
                0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1,
                1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0,
                1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
                0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1,
                0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1,
                0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0,
                0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0
            };
            total_fixed_length -= adjustment[ryu_exponent];
            // whole_digits doesn't need to be adjusted because these cases won't refer to it later.
        }
    }
    else if (whole_digits > 0)
    {
        // case "17.29"
        total_fixed_length = olength + 1;
    }
    else
    {
        // case "0.001729"
        total_fixed_length = cast(u32, 2 - ryu_exponent);
    }

    if (last - first < cast(i64, total_fixed_length))
    {
        return { last, false };
    }

    char * mid;
    if (ryu_exponent > 0)
    { // case "172900"
        bool can_use_ryu;

        if (ryu_exponent > 22)
        { // 10^22 is the largest power of 10 that's exactly representable as a double.
            can_use_ryu = false;
        }
        else
        {
            // Ryu generated X: v.mantissa * 10^ryu_exponent
            // v.mantissa == 2^_Trailing_zero_bits * (v.mantissa >> trailing_zero_bits)
            // 10^ryu_exponent == 2^ryu_exponent * 5^ryu_exponent

            // trailing_zero_bits is [0, 56] (aside: because 2^56 is the largest power of 2
            // with 17 decimal digits, which is double's round-trip limit.)
            // ryu_exponent is [1, 22].
            // Normalization adds [2, 52] (aside: at least 2 because the pre-normalized mantissa is at least 5).
            // This adds up to [3, 130], which is well below double's maximum binary exponent 1023.

            // Therefore, we just need to consider (v.mantissa >> trailing_zero_bits) * 5^ryu_exponent.

            // If that product would exceed 53 bits, then X can't be exactly represented as a double.
            // (That's not a problem for round-tripping, because X is close enough to the original double,
            // but X isn't mathematically equal to the original double.) This requires a high-precision fallback.

            // If the product is 53 bits or smaller, then X can be exactly represented as a double (and we don't
            // need to re-synthesize it; the original double must have been X, because Ryu wouldn't produce the
            // same output for two different doubles X and Y). This allows Ryu's output to be used (zero-filled).

            // (2^53 - 1) / 5^0 (for indexing), (2^53 - 1) / 5^1, ..., (2^53 - 1) / 5^22
            static constexpr u64 max_shifted_mantissa[23] {
                9007199254740991u,
                1801439850948198u,
                360287970189639u,
                72057594037927u,
                14411518807585u,
                2882303761517u,
                576460752303u,
                115292150460u,
                23058430092u,
                4611686018u,
                922337203u,
                184467440u,
                36893488u,
                7378697u,
                1475739u,
                295147u,
                59029u,
                11805u,
                2361u,
                472u,
                94u,
                18u,
                3u,
            };

            // v.mantissa is guaranteed nonzero
            u32 trailing_zero_bits = __builtin_ctzll(v.mantissa);
            u64 shifted_mantissa = v.mantissa >> trailing_zero_bits;
            can_use_ryu = shifted_mantissa <= max_shifted_mantissa[ryu_exponent];
        }

        if (!can_use_ryu)
        {
            // Print the integer exactly.
            // Performance note: This will redundantly perform bounds checking.
            // Performance note: This will redundantly decompose the IEEE representation.
            return d2fixed_buffered_n(first, last, value, 0);
        }

        // can_use_ryu
        // Print the decimal digits, left-aligned within [first, first + total_fixed_length).
        mid = first + olength;
    }
    else
    { // cases "1729", "17.29", and "0.001729"
        // Print the decimal digits, right-aligned within [first, first + total_fixed_length).
        mid = first + total_fixed_length;
    }

    // We prefer 32-bit operations, even on 64-bit platforms.
    // We have at most 17 digits, and u32 can store 9 digits.
    // If _Output doesn't fit into u32, we cut off 8 digits,
    // so the rest will fit into u32.
    if ((output >> 32) != 0)
    {
        // Expensive 64-bit division.
        u64 q = div1e8(output);
        u32 output2 = cast(u32, output - 100000000 * q);
        output = q;

        u32 c = output2 % 10000;
        output2 /= 10000;
        u32 d = output2 % 10000;
        u32 c0 = (c % 100) << 1;
        u32 c1 = (c / 100) << 1;
        u32 d0 = (d % 100) << 1;
        u32 d1 = (d / 100) << 1;

        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c0], 2 * sizeof(char));
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c1], 2 * sizeof(char));
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[d0], 2 * sizeof(char));
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[d1], 2 * sizeof(char));
    }
    u32 output2 = cast(u32, output);
    while (output2 >= 10000)
    {
#ifdef __clang__ // TRANSITION, LLVM-38217
        u32 c = output2 - 10000 * (output2 / 10000);
#else
        u32 c = output2 % 10000;
#endif
        output2 /= 10000;
        u32 c0 = (c % 100) << 1;
        u32 c1 = (c / 100) << 1;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c0], 2 * sizeof(char));
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c1], 2 * sizeof(char));
    }
    if (output2 >= 100)
    {
        u32 c = (output2 % 100) << 1;
        output2 /= 100;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    if (output2 >= 10)
    {
        u32 c = output2 << 1;
        util::raw_copy_range(mid -= 2, &DIGIT_TABLE[c], 2 * sizeof(char));
    }
    else
    {
        *--mid = static_cast<char>('0' + output2);
    }

    if (ryu_exponent > 0)
    { // case "172900" with _Can_use_ryu
        // Performance note: it might be more efficient to do this immediately after setting _Mid.
        util::fill_range(first + olength, ryu_exponent, '0');
    }
    else if (ryu_exponent == 0)
    { // case "1729"
        // Done!
    }
    else if (whole_digits > 0)
    { // case "17.29"
        // Performance note: moving digits might not be optimal.
        util::raw_copy_range(
            first,
            first + 1,
            static_cast<u64>(whole_digits) * sizeof(char)
        );
        first[whole_digits] = '.';
    }
    else
    { // case "0.001729"
        // Performance note: a larger memset() followed by overwriting '.' might be more efficient.
        first[0] = '0';
        first[1] = '.';
        util::raw_fill_range(first + 2, -whole_digits, '0');
    }

    return { first + total_fixed_length, true };
}

internal macro pair<char *, bool> to_chars(char * first, char * last, float value)
{
    // Step 1: Decode the floating-point number, and unify normalized and subnormal cases.
    u32 bits = *cast(u32 *, &value);

    // Case distinction; exit early for the easy cases.
    if (bits == 0)
    {
        if (first == last)
        {
            return { last, false };
        }

        *first = '0';

        return { first + 1, true };
    }

    // Decode bits into mantissa and exponent.
    u32 ieeeMantissa = bits & ((1u << FLOAT_MANTISSA_BITS) - 1);
    u32 ieeeExponent = bits >> FLOAT_MANTISSA_BITS;

    // When the floating-point number is a large integer,
    // it's faster to skip Ryu and immediately print the integer exactly.

    // 1. restore implicit bit
    // 2. bias and normalization

    u32 mantissa2 = ieeeMantissa | (1u << FLOAT_MANTISSA_BITS);
    i32 exponent2 = cast(i32, ieeeExponent) - FLOAT_BIAS - FLOAT_MANTISSA_BITS;

    // Normal values are equal to mantissa2 * 2^exponent2.
    // (Subnormals are different, but they'll be rejected by the Exponent2 test here,
    // so they can be ignored.)

    if (exponent2 > 0)
    {
        return large_integer_to_chars(first, last, mantissa2, exponent2);
    }

    floating_decimal_32 v = f2d(ieeeMantissa, ieeeExponent);
    return internal_to_chars(first, last, v, ieeeMantissa, ieeeExponent);
}

internal macro pair<char *, bool> to_chars(char * first, char * last, double value)
{
    // Step 1: Decode the floating-point number, and unify normalized and subnormal cases.
    u64 bits = *cast(u64 *, &value);

    // Case distinction; exit early for the easy cases.
    if (bits == 0)
    {
        // Print "0" for chars_format::fixed, chars_format::general, and chars_format{}.
        if (first == last)
        {
            return { last, false };
        }

        *first = '0';

        return { first + 1, true };
    }

    // Decode bits into mantissa and exponent.
    u64 ieeeMantissa = bits & ((1ull << DOUBLE_MANTISSA_BITS) - 1);
    u32 ieeeExponent = cast(u32, bits >> DOUBLE_MANTISSA_BITS);
    // 1. restore implicit bit
    // 2. bias and normalization
    // u64 mantissa2 = ieeeMantissa | (1ull << DOUBLE_MANTISSA_BITS);
    i32 exponent2 = cast(i32, ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS;

    // Normal values are equal to mantissa2 * 2^exponent2.
    // (Subnormals are different, but they'll be rejected by the exponent2 test here, so they can be ignored.)

    // For nonzero integers, exponent2 >= -52. (The minimum value occurs when mantissa2 * 2^exponent2 is 1.
    // In that case, mantissa2 is the implicit 1 bit followed by 52 zeros, so exponent2 is -52 to shift away
    // the zeros.) The dense range of exactly representable integers has negative or zero exponents
    // (as positive exponents make the range non-dense). For that dense range, Ryu will always be used:
    // every digit is necessary to uniquely identify the value, so Ryu must print them all.

    // Positive exponents are the non-dense range of exactly representable integers. This contains all of the values
    // for which Ryu can't be used (and a few Ryu-friendly values). We can save time by detecting positive
    // exponents here and skipping Ryu. Calling d2fixed_buffered_n() with precision 0 is valid for all integers
    // (so it's okay if we call it with a Ryu-friendly value).
    if (exponent2 > 0)
    {
        return d2fixed_buffered_n(first, last, value, 0);
    }

    floating_decimal_64 v;
    bool isSmallInt = d2d_small_int(ieeeMantissa, ieeeExponent, &v);
    if (isSmallInt)
    {
        // For small integers in the range [1, 2^53), v.mantissa might contain trailing (decimal) zeros.
        // For scientific notation we need to move these zeros into the exponent.
        // (This is not needed for fixed-point notation, so it might be beneficial to trim
        // trailing zeros in internal_to_chars only if needed - once fixed-point notation output is implemented.)
        for (;;)
        {
            u64 q = div10(v.mantissa);
            u32 r = cast(u32, v.mantissa) - 10 * cast(u32, q);
            if (r != 0)
            {
                break;
            }
            v.mantissa = q;
            ++v.exponent;
        }
    }
    else
    {
        v = d2d(ieeeMantissa, ieeeExponent);
    }

    return internal_to_chars(first, last, v, value);
}

template <typename T>
internal macro pair<char *, bool> to_chars(
    char * first,
    char * last,
    T value,
    int precision
)
{
    // C11 7.21.6.1 "The fprintf function"/5:
    // "A negative precision argument is taken as if the precision were omitted."
    // /8: "f,F [...] If the precision is missing, it is taken as 6"

    if (precision < 0)
    {
        precision = 6;
    }
    else if (precision < 1'000'000'000)
    {
        // _Precision is ok.
    }
    else
    {
        // Avoid integer overflow.
        // (This defensive check is slightly nonconformant; it can be carefully improved in the future.)
        return { last, false };
    }

    return d2fixed_buffered_n(first, last, value, cast(u32, precision));
}