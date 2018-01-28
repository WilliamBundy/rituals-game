/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// Sort/Search macros
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

#define GenerateInsertionSortForType(func_name, T, Member_Macro) \
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

