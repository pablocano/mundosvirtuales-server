#pragma once

#include "../Position.h"
#include "../security/Cryptography.h"
#include "../serialization/json.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <algorithm>

#define PORT_SERVERPLANT			4321 /* Define port for TCP communication.*/
#define MAYOR_NUM_VERSION_PACKET	0x1  /* Define mayor number of version. */
#define MENOR_NUM_VERSION_PACKET	0x0  /* Define menor number of version. */
#define VERSION_PACKET (((MAYOR_NUM_VERSION_PACKET) << 4) | (MENOR_NUM_VERSION_PACKET))  /* Define version of communication packet. */

#define MAX_ERROR_COMM 5 /* Define max number of error supported before it closed communication. */

#define COMM_ENCRYPTED /* If is defined, the packets are encrypted. */
#define KEY_CRYPT { 56324394, 73576, 12030122, 56 } /* Private key for encrypted. */

#ifdef WIN32
	#define STRUCT_PACKET struct
	#pragma pack(push)
	#pragma pack(1)
#else
	#define STRUCT_PACKET struct __attribute__((packed))
#endif

/// <summary>
/// Enum that contain different commands.  
/// </summary>
typedef enum _Command
{
	NONE,
	RESPONSE_COMMAND,
	CLOSE_CONNECTION,
	GET_INFO_PLANT,
	GET_ASSEMBLIES,
	GET_LIST_ASSEMBLIES,
	GET_MODEL_ASSEMBLIES,
	GET_INFO_ASSEMBLIES,
	GET_MAINTENANCE_ASSEMBLY,
	GET_LIST_SENSORS,
	GET_SENSORS,
	GET_VERSION_ID,
	NEW_ASSEMBLY,
	UPDATE_ASSEMBLY,
	GET_PLANT
} Command;

/// <summary>
/// Enum that contain Server Status.
/// </summary>
typedef enum _StatusServer
{
	NORMAL,
	WRONG_COMMAND,
	WRONG_VERSION,
	OK_RESPONSE,
	ERROR_RESPONSE,
	FULL_CONNECTION,
	ERROR_CONNECTION
} StatusServer;

/// <summary>
/// Header of Communication Packet.
/// </summary>
typedef STRUCT_PACKET _HeaderPacketComm
{
	uint8_t version;			/* Version of packet. */
	Command m_command;			/* Command */
	StatusServer m_statusComm;	/* Status Server */
	uint32_t m_idResponse;		/* Identificator for Response Packet. */
	uint32_t m_size;			/* Size content of packet. */

	/// <summary>
	/// Constructor
	/// </summary>
	_HeaderPacketComm() : version(VERSION_PACKET), m_command(Command::NONE), m_statusComm(StatusServer::NORMAL), m_idResponse(0), m_size(0) {};

	/// <summary>
	/// Operator equals.
	/// </summary>
	/// <param name="header">Header for comparing.</param>
	/// <returns>Returns true if all elements of header are equals, false otherwise.</returns>
	bool _HeaderPacketComm::operator ==(const _HeaderPacketComm &header) const
	{
		return (version == header.version) && (m_command == header.m_command) && (m_idResponse == header.m_idResponse) && (m_size == header.m_size);
	}
} HeaderPacketComm;

/// <summary>
/// Size of header.
/// </summary>
#define SIZE_HEADER_PACKET (sizeof(HeaderPacketComm))

/// <summary>
/// Packet for communication.
/// </summary>
typedef STRUCT_PACKET _PacketComm
{
	HeaderPacketComm m_header; /* Header of Packet. */
	char* m_lpContent; /* Payload of Packet. */

	/// <summary>
	/// Constructor.
	/// </summary>
	_PacketComm() : m_header(), m_lpContent(nullptr) {}

	/// <summary>
	/// Size of Packet.
	/// </summary>
	/// <returns>Size of Packet (header + payload).</returns>
	int size() const
	{
		return SIZE_HEADER_PACKET + sizeContent();
	}

	/// <summary>
	/// Size of Payload or Content.
	/// </summary>
	/// <returns>Size Payload.</returns>
	int sizeContent() const
	{
		int _sizeContent = 0;
		if (m_lpContent)
		{
			int n = (int)strlen(m_lpContent);
			if (n > 1) // discard empty string.
			{
				_sizeContent = n + 1; // Added '\0' character.
				int m = _sizeContent % 4;
				_sizeContent += (m == 0 ? 0 : (4 - m)); // Adds padding for encryptation.
			}
		}

		return _sizeContent;
	}

	/// <summary>
	/// Packing Communication Packet. 
	/// </summary>
	/// <returns>Returns smart pointer with data Packet (header + payload).</returns>
	std::unique_ptr<char[]> packing()
	{
		std::unique_ptr<char[]> p(new char[size()]());
		uint32_t key[4] = KEY_CRYPT;
		m_header.m_size = sizeContent();
		std::memcpy(p.get(), &m_header, SIZE_HEADER_PACKET);

#ifdef COMM_ENCRYPTED
		if (m_header.m_size > 0)
		{
			char* lpContent = p.get() + SIZE_HEADER_PACKET;
			std::memcpy(lpContent, m_lpContent, (int)strlen(m_lpContent));
			Cryptography::encrypt((uint32_t*)lpContent, m_header.m_size / 4, key);
		}
#else
		std::memcpy(p.get() + SIZE_HEADER_PACKET, m_lpContent, (int)strlen(m_lpContent) + 1);
#endif

		return p;
	}

	/// <summary>
	/// Unpacking Communication Packet.
	/// </summary>
	/// <param name="header">Header</param>
	/// <param name="lpContent">Content or Payload</param>
	/// <returns>New Communication Packet (header + payload)</returns>
	static _PacketComm unpacking(HeaderPacketComm header, char* lpContent)
	{
		uint32_t key[4] = KEY_CRYPT;
		_PacketComm packet;
		std::memcpy(&packet.m_header, &header, SIZE_HEADER_PACKET);

		packet.m_lpContent = lpContent;
#ifdef COMM_ENCRYPTED
		if (packet.m_header.m_size > 0)
		{
			Cryptography::decrypt((uint32_t*)packet.m_lpContent, packet.m_header.m_size / 4, key);
		}
#endif

		return packet;
	}

	/// <summary>
	/// Operator equals.
	/// </summary>
	/// <param name="packet">Packet for comparing.</param>
	/// <returns>Returns true if headers are equals or the idResponses are equals in both packets, this last condition is for doing to match the response packets. Returns false otherwise.</returns>
	bool _PacketComm::operator ==(const _PacketComm &packet) const
	{
		uint32_t _sizeContent = std::min<int>(sizeContent(), packet.sizeContent());
		return (m_header.m_command == Command::RESPONSE_COMMAND && (m_header.m_idResponse == packet.m_header.m_idResponse)) ||
			(m_header == packet.m_header && (sizeContent() == _sizeContent && strncmp(m_lpContent, packet.m_lpContent, _sizeContent - 1)));
	}
} PacketComm;

#ifdef WIN32
	#pragma pack(pop)
#endif
