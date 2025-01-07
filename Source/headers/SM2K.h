#pragma once


namespace SM2K
{
#define list(_type) std::list<_type>
#define vector(_type) std::vector<_type>
#define Shared(_type) std::shared_ptr<_type>
#define make(_type) std::make_shared<_type>()
#define make_a(_type, ...) std::make_shared<_type>(__VA_ARGS__)
#define make_s(_type, _size) std::make_shared<_type>(_size)
#define make_empty(_size) std::make_shared_for_overwrite<_type>(_size)
#define make_singleton(_type) Shared(_type)(new _type)
#define make_array(_type, _size) Shared(_type)(new _type[_size])
#define make_singleton_a(_type, ...) Shared(_type)(new _type(__VA_ARGS__))
#define swap_endian(_16bitBuffer) ((_16bitBuffer << 8) | (_16bitBuffer >> 8)) 
#define null_buffer(_buffer, _size) memset(_buffer, 0x0, _size)
#define set_str(_sharedPtr, _cstr) _sharedPtr = make_s(schar, strlen(_cstr)+1);\
												null_buffer(_sharedPtr.get(), strlen(_cstr)+1);\
												memcpy(_sharedPtr.get(), _cstr, strlen(_cstr))

	typedef signed __int8				s8;
	typedef unsigned __int8				u8;
	typedef signed __int16				s16;
	typedef unsigned __int16			u16;
	typedef signed __int32				s32;
	typedef unsigned __int32			u32;
	typedef  __int64					s64;
	typedef unsigned __int64			u64;
	
	typedef s8							sbyte;
	typedef u8							byte;
	typedef s16							sshort;
	typedef u16							ushort;
	typedef s32							sint;
	typedef u32							uint;
	typedef s64							slong;
	typedef u64							ulong;

	typedef u64							Hash;

	typedef std::thread					thread;
	typedef std::mutex					mutex;
	typedef std::condition_variable		cv;
	typedef	char						schar;
	typedef const schar*				cstring;
	typedef std::string					string;
	typedef uint						ID;
	typedef entt::registry				_Registry;
	typedef entt::entity				_Entity;
	

	using ComponentFunc = std::function<void(_Registry&)>;

	struct _REGENT
	{
		_Registry* registry = nullptr;
		_Entity	entity;
	};

	inline ID operator"" _id(ulong _id)
	{
		return ID{ uint(_id) };
	}

	inline ID operator"" _nullid(ulong _id)
	{
		return ID{ 0xffff };
	}


	struct AutoID 
	{
		ID id = 0_id;

		explicit AutoID() 
		{
			id = count++;
		}

		explicit AutoID(ID _id) 
		{
			id = _id;
		}

		static inline void Reset() { count = 0; }

		/*void operator()() // May use later
		{
			id = count++;
		};*/

		inline const AutoID& operator=(const AutoID& _ref)
		{
			id = _ref.id;
			return *this;
		}
		
		inline const ID& operator=(const ID& _ref)
		{
			return id = _ref;
		}

		operator ID() 
		{
			return id;
		}

		
		operator ulong()
		{
			return id;
		}

	private:
		static ID count;

	};

#ifdef AUTO_ID
#define AUTO_ID 1

	ID AutoID::count = 0;


	namespace AutoID_Literals 
	{
		inline AutoID operator"" _incrementID(ulong _id) // This is used to update the id count
		{
			return AutoID();
		}

		inline AutoID operator"" _aid(ulong _id) // Set an id
		{
			return AutoID(_id);
		}

	};

#endif // !AUTO_ID
};
