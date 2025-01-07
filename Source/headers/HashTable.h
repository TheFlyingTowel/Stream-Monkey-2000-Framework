#pragma once



namespace SM2K 
{
//#define HASH_SIZE_IN_BACK(cstr)\
//		Hash hash = strlen(cstr) & 0xFFFFFFFF;\
//		 return (static_cast<Hash>(swap_endian((*reinterpret_cast<const u16*>(cstr) & 0xFFFF) << 16) | swap_endian(*reinterpret_cast<const u16*>(&cstr[hash - 2]) & 0xFFFF)) << 32) | hash
//
//#define HASH_SIZE_IN_FRONT(cstr)\
//		Hash hash = strlen(_key) & 0xFFFFFFFF;\
//		return (hash << 32) | (((*reinterpret_cast<const u16*>(_key) & 0xFFFF) << 16) | (*reinterpret_cast<const u16*>(&_key[hash - 2]) & 0xFFFF))


	namespace HashingFunctions
	{
		constexpr inline Hash DefaultHash(cstring _key)
		{
			Hash size = 0x0;
			while (_key[size]) ++size; // Get the len
			if (size < 2) return 0;

			return (size << 32) |
				(((*reinterpret_cast<const u16*>(_key) & 0xFFFF) << 16) |
					(*reinterpret_cast<const u16*>(&_key[size - 2]) & 0xFFFF));
		};

		constexpr inline Hash fnv1a_hash(cstring str, Hash hash = 0xcbf29ce484222325) {
			return (*str == 0) ? hash : fnv1a_hash(str + 1, (hash ^ *str) * 0x100000001b3);
		};


		
	};

	struct SM2K_HashingFunc 
	{
		inline size_t operator()(cstring _key) const 
		{
			return HashingFunctions::DefaultHash(_key);
		}
	};

	struct cstr_equal {
		bool operator()(cstring lhs, cstring rhs) const {
			return std::strcmp(lhs, rhs) == 0;
		}
	};

	template<typename ObjectType>
	class StringTable
		:public std::unordered_map<cstring, ObjectType, SM2K_HashingFunc, cstr_equal>
	{
	public:

		StringTable();

	};




	template<typename ObjectType>
	inline StringTable<ObjectType>::StringTable() {};

	  



};
