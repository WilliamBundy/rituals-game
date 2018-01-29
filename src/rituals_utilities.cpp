
// Random number generators
static inline u64 _splitmix64(u64* x)
{
	*x += UINT64_C(0x9E3779B97F4A7C15);
	u64 z = *x;
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);	
}

static inline u64 _rotate_left(const u64 t, i64 k)
{
	return (t << k) | (t >> (64 - k));
}

#ifndef WirmphtEnabled
struct Random
{
	u64 x, y;
};
#endif

u64 next_random_u64(Random* r)
{
	u64 a = r->x;
	u64 b = r->y;
	u64 result = a + b;

	b ^= a;
	r->x = _rotate_left(a, 55) ^ b ^ (b << 14);
	r->y = _rotate_left(b, 36);
	return result;
}

void init_random(Random* r, u64 seed) 
{
	_splitmix64(&seed);
	r->x = _splitmix64(&seed);
	r->y = _splitmix64(&seed);
	next_random_u64(r);
}

f64 next_random_double(Random* r)
{
	u64 x = next_random_u64(r);
	return (f64)x / UINT64_MAX;
}

f32 next_random_float(Random* r)
{
	return (f32)(next_random_double(r));
}

f32 next_random(Random* r) 
{
	return (f32)(next_random_double(r));
}

f32 rand_range(Random* r, f32 min, f32 max)
{
	return next_random_double(r) * (max - min) + min;
}

i32 rand_range_int(Random* r, f32 min, f32 max)
{
	return (i32)(next_random_double(r) * (max - min) + min);
}


#define GenerateQuicksortForType(func_name, T, Member_Macro) \
void func_name(T* array, isize count) \
{ \
	if(count > 1) { \
		T tmp = array[0]; \
		array[0] = array[count / 2]; \
		array[count / 2] = tmp; \
		isize pivot = 0; \
		for(isize i = 1; i < count; ++i) { \
			if(Member_Macro(array[i]) < Member_Macro(array[0])) { \
				tmp = array[++pivot]; \
				array[pivot] = array[i]; \
				array[i] = tmp; \
			} \
		} \
		tmp = array[0]; \
		array[0] = array[pivot]; \
		array[pivot] = tmp; \
		func_name(array, pivot); \
		func_name(array + pivot + 1, count - (pivot + 1)); \
	} \
}

#define GenerateInsertionSortForType(func_name, T, Member_Macro, M_T) \
void func_name(T* array, isize count) \
{ \
	for(isize i = 1; i < count; ++i) { \
		T x = array[i]; \
		isize j = i - 1; \
		while((j >= 0) && (Member_Macro(array[j]) > Member_Macro(x))) { \
			array[j + 1] = array[j]; \
			j--; \
		} \
		array[j+1] = x; \
	} \
}
#define GenerateIntrosortForType(func_name, T, Cutoff, Member_Macro) \
void func_name(T* array, isize count) \
{ \
	if(count > 1) \
	if(count > Cutoff) { \
		T tmp = array[0]; \
		array[0] = array[count / 2]; \
		array[count / 2] = tmp; \
		isize pivot = 0; \
		for(isize i = 1; i < count; ++i) { \
			if(Member_Macro(array[i]) < Member_Macro(array[0])) { \
				tmp = array[++pivot]; \
				array[pivot] = array[i]; \
				array[i] = tmp; \
			} \
		} \
		tmp = array[0]; \
		array[0] = array[pivot]; \
		array[pivot] = tmp; \
		func_name(array, pivot); \
		func_name(array + pivot + 1, count - (pivot + 1)); \
	} else for(isize i = 1; i < count; ++i) { \
		T x = array[i]; \
		isize j = i - 1; \
		while((j >= 0) && (Member_Macro(array[j]) > Member_Macro(x))) { \
			array[j + 1] = array[j]; \
			j--; \
		} \
		array[j+1] = x; \
	} \
}

#define GenerateQsortForType(func_name, T, Cutoff, Member_Macro) \
int _cmp_##func_name##_##t(const void* a, const void* b) \
{ \
	T* ta = (T*)a; \
	T* tb = (T*)b; \
	return Member_Macro(ta) - Member_Macro(tb); \
} \
void func_name(T* array, isize count) \
{ \
	qsort(array, count, sizeof(T), _cmp_##func_name##_##t); \
} 

 
// Returns -1 on fail to find.
#define GenerateBinarySearchForType(func_name, T, K, Member_Key_Macro) \
isize func_name(K key, T* array, isize count) \
{ \
	if(count == 0) return -1; \
	isize min = 0, max = count - 1, mid = 0; \
	K localkey; \
	while(min <= max) { \
		mid = (min + max) / 2; \
		localkey = Member_Key_Macro(array[mid]); \
		if(localkey == key) { \
			return mid; \
		} else if(localkey < key) { \
			min = mid + 1; \
		} else { \
			max = mid - 1; \
		} \
	} \
	return -1; \
} 
 

#define _passthru_macro(x) (x) 
#define _generate_sort_and_search_for_numeric_type(t) \
	GenerateQuicksortForType(t##_sort, t, _passthru_macro) \
	GenerateBinarySearchForType(t##_search, t, t, _passthru_macro) 
_generate_sort_and_search_for_numeric_type(f32);

