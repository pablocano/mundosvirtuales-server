#pragma once

#include "security\Cryptography.h"
#include <cstring>
#include <cstdlib>
#include <memory>

#define PORT_SERVERPLANT 4321
#define MAYOR_NUM_VERSION_PACKET 0x1
#define MENOR_NUM_VERSION_PACKET 0x0
#define VERSION_PACKET (((MAYOR_NUM_VERSION_PACKET) << 4) | (MENOR_NUM_VERSION_PACKET))

#define KEY_CRYPT { 56324394,73576,12030122,56 }

typedef enum _Command
{
	NONE,
	GET_INFO_PLANT,
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
	const char version = VERSION_PACKET;
	Command m_command;
	StatusServer m_statusComm;
	int m_size; // Size content
} HeaderPacketComm;

#define SIZE_HEADER_PACKET (sizeof(HeaderPacketComm))

typedef struct _PacketComm
{
	HeaderPacketComm m_header;
	char* m_lpContent;

	int size()
	{
		return SIZE_HEADER_PACKET + sizeContent();
	}

	int sizeContent()
	{
		int _sizeContent = 0;
		if (m_lpContent)
			_sizeContent = ((int)strlen(m_lpContent)) + 1;

		return _sizeContent > 1 ? _sizeContent : 0;
	}

	std::unique_ptr<char[]> packing()
	{
		// char *p = (char *) std::malloc(size());
		std::unique_ptr<char[]> p(new char[size()]);
		uint32_t key[4] = KEY_CRYPT;
		m_header.m_size = sizeContent();
		std::memcpy(p.get(), &m_header, SIZE_HEADER_PACKET);
		if (m_header.m_size > 0)
		{
			char* lpContent = (char *) std::malloc(m_header.m_size);
			std::memcpy(lpContent, m_lpContent, m_header.m_size);
			Cryptography::encrypt((uint32_t*)lpContent, (unsigned)(m_header.m_size / 4), key);
			std::memcpy(p.get() + SIZE_HEADER_PACKET, lpContent, m_header.m_size);
			std::free(lpContent);
		}

		return p;
	}

	static _PacketComm unpacking(HeaderPacketComm header, char* lpContent)
	{
		uint32_t key[4] = KEY_CRYPT;
		_PacketComm packet;
		std::memcpy(&packet.m_header, &header, sizeof(HeaderPacketComm));

		packet.m_lpContent = lpContent;
		header.m_size = packet.sizeContent();
		if (packet.m_lpContent)
		{
			Cryptography::decrypt((uint32_t*)packet.m_lpContent, (unsigned)(strlen(packet.m_lpContent) / 4), key);
		}

		return packet;
	}
} PacketComm;


