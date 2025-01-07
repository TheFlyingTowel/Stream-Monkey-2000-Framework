#pragma once


namespace SM2K
{

#define DONT_LOCK std::defer_lock
#define	lockGaurd std::lock_guard<mutex> 
#define	lockUnique std::unique_lock<mutex>
#define REQUEST_BUFFER_SIZE 769
#define RESPONCE_BUFFER_SIZE 257


	enum : ID { NONE = 0 };

	namespace Core_layer
	{
		typedef
			enum CommunicationBufferType
		{
			REQUEST = 1, RESPONCE
		}ComBufferType;

		typedef
			enum RequestType : ID
		{
			NULL_REQUEST = 0, CMD_REQUEST, LOCAL_REQUEST, NET_REQUEST
		}ReqType;

		typedef
			enum ResponceType : ID
		{
			NULL_RESPONCE = 0, OK = 0x01, _ERROR = 0x02, WARN = 0x04, PENDING = 0x08, UNHANDLED = 0x10
		}RespType;

#pragma pack(4) // Ensure we have the correct bit alignemt.
		union Request_t // 3.08 KB
		{
			struct
			{
				RequestType type : 5;
				ID id : 10;
				uint : 17;		// Padding
				schar data[2048];
				byte metaData[1024];
			};
			uint raw[REQUEST_BUFFER_SIZE];

			inline operator void* ()
			{
				return raw;
			}


		};


		union Responce_t // 1.03 KB
		{
			struct
			{
				ResponceType results : 16;
				ID id : 10;
				uint : 3;		// Padding
				schar metaData[1024];
			};
			uint raw[RESPONCE_BUFFER_SIZE];

			inline operator void* ()
			{
				return raw;
			}


			inline const Responce_t& operator=(const Responce_t& _ref)
			{
				memcpy(raw, _ref.raw, RESPONCE_BUFFER_SIZE);
				return *this;
			};

		};



		typedef
			union CommunicationBuffer_t // 4.2 MB
		{
			struct
			{
				Request_t request[1024]; // Request region of buffer
				Responce_t responce[1024]; // Responce region of buffer
			};
			uint raw[0x100800]; // raw size is 1,050,624 ints
		}ComBuff_t;
#pragma pack(pop)


		typedef Request_t* RequestPtr;
		typedef Responce_t* ResponcePtr;
		typedef Responce_t				Responce;

		using ComBuff = std::shared_ptr<CommunicationBuffer_t>; // The Communication Buffer. Size: 4,202,496 bytes [4.2 MB]
		using osstream = std::ostringstream;
		using isstream = std::istringstream;





	}
};

