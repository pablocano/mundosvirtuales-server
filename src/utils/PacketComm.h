#pragma once

#include "security\Cryptography.h"
#include <cstring>
#include <cstdlib>
#include <memory>
#include <algorithm>

#define PORT_SERVERPLANT 4321
#define MAYOR_NUM_VERSION_PACKET 0x1
#define MENOR_NUM_VERSION_PACKET 0x0
#define VERSION_PACKET (((MAYOR_NUM_VERSION_PACKET) << 4) | (MENOR_NUM_VERSION_PACKET))

#define MAX_ERROR_COMM 5

// #define COMM_ENCRYPTED
#define KEY_CRYPT { 56324394, 73576, 12030122, 56 }

typedef enum _Command
{
	NONE,
	RESPONSE_COMMAND,
	GET_INFO_PLANT,
	GET_MACHINES,
	GET_LIST_MACHINES,
	GET_MODEL_MACHINES,
	GET_INFO_MACHINE,
	GET_MAINTENANCE_MACHINE,
	GET_LIST_SENSORS,
	GET_SENSORS,
	CLOSE_CONNECTION
} Command;

typedef enum _StatusServer
{
	NORMAL,
	WRONG_COMMAND,
	WRONG_VERSION,
	FULL_CONNECTION,
	ERROR_CONNECTION
} StatusServer;

typedef struct _HeaderPacketComm
{
	uint8_t version;
	Command m_command;
	StatusServer m_statusComm;
	uint32_t m_idResponse;
	uint32_t m_size; // Size content

	_HeaderPacketComm() : version(VERSION_PACKET), m_command(Command::NONE), m_statusComm(StatusServer::NORMAL), m_idResponse(0), m_size(0) {};

	bool _HeaderPacketComm::operator ==(const _HeaderPacketComm &header) const
	{
		return (version == header.version) && (m_command == header.m_command) && (m_idResponse == header.m_idResponse) && (m_size == header.m_size);
	}
} HeaderPacketComm;

#define SIZE_HEADER_PACKET (sizeof(HeaderPacketComm))

typedef struct _PacketComm
{
	HeaderPacketComm m_header;
	char* m_lpContent;

	_PacketComm() : m_header(), m_lpContent(nullptr) {}

	int size() const
	{
		return SIZE_HEADER_PACKET + sizeContent();
	}

	int sizeContent() const
	{
		int _sizeContent = 0;
		if (m_lpContent)
			_sizeContent = ((int)strlen(m_lpContent)) + 1;

		return _sizeContent > 1 ? _sizeContent : 0;
	}

	std::unique_ptr<char[]> packing()
	{
		std::unique_ptr<char[]> p(new char[size()]);
		uint32_t key[4] = KEY_CRYPT;
		m_header.m_size = sizeContent();
		std::memcpy(p.get(), &m_header, SIZE_HEADER_PACKET);

#ifdef COMM_ENCRYPTED
		if (m_header.m_size > 0)
		{
			char* lpContent = p.get() + SIZE_HEADER_PACKET;
			std::memcpy(lpContent, m_lpContent, m_header.m_size);
			Cryptography::encrypt((uint32_t*)lpContent, (m_header.m_size - 1) / 4, key);
		}
#else
		std::memcpy(p.get() + SIZE_HEADER_PACKET, m_lpContent, m_header.m_size);
#endif

		return p;
	}

	static _PacketComm unpacking(HeaderPacketComm header, char* lpContent)
	{
		uint32_t key[4] = KEY_CRYPT;
		_PacketComm packet;
		std::memcpy(&packet.m_header, &header, SIZE_HEADER_PACKET);

		packet.m_lpContent = lpContent;
		packet.m_header.m_size = packet.sizeContent();
#ifdef COMM_ENCRYPTED
		if (packet.m_header.m_size > 0)
		{
			Cryptography::decrypt((uint32_t*)packet.m_lpContent, (packet.m_header.m_size - 1) / 4, key);
		}
#endif

		return packet;
	}

	bool _PacketComm::operator ==(const _PacketComm &packet) const
	{
		uint32_t _sizeContent = std::min<int>(sizeContent(), packet.sizeContent());
		return (m_header.m_command == Command::RESPONSE_COMMAND && (m_header.m_idResponse == packet.m_header.m_idResponse)) ||
			(m_header == packet.m_header && (sizeContent() == _sizeContent && strncmp(m_lpContent, packet.m_lpContent, _sizeContent - 1)));
	}
} PacketComm;


